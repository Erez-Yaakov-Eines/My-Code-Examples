
/**
 * Copyright 2021 Erez Yaakov Eines & Ori Itzkovitch.
 * 
 * Permission is hereby granted, free of charge, 
 * to any person obtaining a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.`
 */

/**
 * For documentation and reference, consider the following links:
 * 
 * Actions builder YouTube Tutorials - https://www.youtube.com/playlist?list=PLOU2XLYxmsIJ5qQKAYt45zZNMU9h1Grpm
 * Firebase Console - https://console.firebase.google.com/
 * Actions Console - https://console.actions.google.com/
 * Google Cloud Console - https://console.cloud.google.com/
 * @assistant/conversation - https://actions-on-google.github.io/assistant-conversation-nodejs/3.1.0/index.html
 * Actions Builder - https://developers.google.com/assistant/console/builder
 * Actions Console UI - https://developers.google.com/assistant/console/ui
 * Firebase Admin SDK - https://firebase.google.com/docs/admin/setup?authuser=0#initialize-sdk
 * Firebase Firestore - https://firebase.google.com/docs/firestore?authuser=0
 * Firebase Cloud Storage - https://firebase.google.com/docs/storage?authuser=0
 * Firebase Cloud Functions - https://firebase.google.com/docs/functions?authuser=0
 * gactions CLI - https://developers.google.com/assistant/conversational/df-asdk/actions-sdk/gactions-cli
 * Firebase CLI - https://firebase.google.com/docs/cli
 * NodeJS Package Manager - https://www.npmjs.com/
 * 
 */

//IMPORTS   **********************************************************************
const { cloneDeep, isUndefined } = require('lodash');
const Game = require('./classes/Game');//the main object that handles player interaction during gameplay
const Player = require('./classes/Player');// A supporting class for saving user statistics
const Utils = require('./classes/Utils');// General utilities
const ConvParams = require('./constants/ConvParams');//holds common parameter names defined in actions builder gui
const PhraseConstants = require('./constants/PhraseConstants');//holds string literals and methods for prompts and responses
const AudioConstants = require('./constants/AudioConstants');// holds audio cue definition strings
const { conversation } = require('@assistant/conversation');// google conversation object
const functions = require('firebase-functions');// google cloud firebase infrastructure object
const admin = require('firebase-admin');// google firebase databse administraor object
var serviceAccount = require("./config/learning-english-final-project-firebase-adminsdk-rhi94-e3220c772c.json");//certificate for firebase project

//INITS     **********************************************************************

//authorize use of firebase services
admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://learning-english-final-project.firebaseio.com"
  });

const db = admin.firestore();// get firestore DB reference
const app = conversation({
    //secret, unique google provided ID sent in token, to verify request to this function.
    //Requests with different ID are rejected.
    clientId: '603505684853-bnqnee8m60pfnvqjklk17jdceo614p56.apps.googleusercontent.com',
});

//APPLICATION WEBHOOK HANDLING  **********************************************************************

/**
 * this handler is implicitly called on account linking flow.
 * do not change handler name from 'linkAccount',
 * do not invoke on your own.
 */
app.handle('linkAccount', conv => {
    let payload = conv.headers.authorization;
    // case authorization header string is not undefined.
    // this should be replaced since user email is always available
    // after auth through user storage in all requests, so should not be stored in session.
    if (payload) {
      const email = payload.email;
      conv.session.params.email = email;
    }
  });

/**
 * Intent handler for game initialization.
 * 
 * Returns a Promise.
 * 
 * Note - requesting the size of a collection can be time consuming if collection is large,
 * however, Game collection should always remain small (<< 1000)
 */
app.handle('initApp', (conv) => {
   return db.collection('Games')
    .get()
    .then(snap => {
        size = snap.size // will return the collection size
        let plainGameObject = Game.startingGameObject(size);//get initial game object through static method.
        conv.session.params.plainGameObject = plainGameObject;//store initial game in session.
        conv.session.params.email = conv.user.params.tokenPayload.email;//probably redundent. use conversation user storage instead.
        return null;
     })
    .catch((err) => {
        conv.add(err.message);
    });
    
});

/**
 * 
 */
app.handle('chooseName', (conv) => {
    const chosenName = conv.intent.params.myName;//get name parameter from conversation
    const newUser = cloneDeep(Player.defaultPlayerObject());//get new initial user object via static method

    //no name parameter exist in webhook request
    if (isUndefined(chosenName)) {
        Game.say('that name won\'t work, can you try again?', conv);
        return;
    }
    else {
        let users = conv.session.params.plainGameObject.users;//current players array from conversation session
        let name = chosenName.resolved.toLowerCase();//lowercase everything to prevent euqality errors
        if (Utils.isUserNameTaken(users, name)) {
            Game.say('that name is already taken by a different player. can you try again?', conv);
            return;
        }
        //case name is not taken
        else {
            newUser.userName = chosenName.resolved;
            conv.session.params.plainGameObject.users.push(newUser);//push to user array
            Game.say(`Welcome ${chosenName.resolved}.`, conv);//TODO - move to PhraseConstants and diversify
        }

    }
    conv.session.params.plainGameObject.currentUser++;//moves to next player chooseName flow

});

/**
 * Intent handler for prompting users with questions.
 * 
 * ASSUMES - players finished game setup successfuly and are in gameplay.
 * hooking this handler before gameplay would result in a crash.
 */
app.handle('askQuestion', (conv) => {
    const game = new Game(conv.session.params.plainGameObject, conv);//construct current game state
    const name = game.users[game.currentPlayerIndex].userName;
    const question = game.questions[game.currentQuestion];//uses game implicit getters
    const ssml = PhraseConstants.randAskQuestion(name, game.currentQuestion + 1, question);//generates diverse response           
    Game.say(ssml, conv);
});

/**
 * Intent handler for checking user answers during gameplay when a user utterance matches a game answer
 */
app.handle('checkAnswer', (conv) => {
    const game = new Game(conv.session.params.plainGameObject, conv);
    //case a correct answer parameter is located in user utterance
    if (game.isUserCorrect())
    {
        //case utterance is too short
        if (Utils.tooShort(game.userSaid))
        {
            game.correctButShort();
            return;
        }
        //utterance is of sufficient length
        else 
            game.correct();
    }
    //no correct parameters found in user utterance
    else
    {
        if (Utils.tooShort(game.userSaid))
            game.incorrect(true);
        else
            game.incorrect(false);
    }
});

/**
 * Intent Handler for retrieving chosen game data.
 * 
 * this handler is called when game setup flow finishes.
 */
app.handle('startGame', (conv) => {
    const gameNumber = parseInt(conv.session.params.gameNumber);//retrieve game number from conversation parameters
    //
    return db.collection('Games')
    .where('id', '==', gameNumber)
    .get()
    .then((snapshot) => {
        if (snapshot.size !== 1) throw new Error('Error! - too many games from database. only 1 should return');
        conv.session.params.gameNumber = null;//remove parameter from conversation
        //for each syntax used for simplicity. single doc object retrieved
        snapshot.forEach((doc) => {
            let data = doc.data();
            conv.session.params.plainGameObject.numberOfQuestions = data.questions.length;
            for(let key in data) {
                conv.session.params.plainGameObject[key] = cloneDeep(data[key]);//clone all game data into game object
            }
            Game.say('Welcome to ' + data.name + '. ' + data.instructions + '.', conv);//TODO - move to phrase constants and diversify
        });
        
        return null;
    })
    .catch((err) => {
        conv.add(err.message);//TODO - switch to logging on firebase using console.log() and prompt user with friendly message
    });
});

/**
 * A global Intent handler that matches a user's intent to get an example phrase.
 * 
 * matches current scene for example context.
 */
app.handle('example', (conv) => {
    const userSaid = conv.intent.query;//retrieve user utterance from conversation.
    //case utterance is short
    if (Utils.tooShort(userSaid))
        Game.say(Utils.randResponse(PhraseConstants.exampleReqTooShort), conv);
    else
    {
        const currentScene = conv.scene.name;//retrive scene name from conversation.
        const game = new Game(conv.session.params.plainGameObject, conv);
        let ssml = '';//init new ssml string
        //diversify this switch/case and perhaps objectify.
        switch(currentScene)
        {
            case ConvParams.numberOfPlayersScene:
                ssml = "Try saying something like: We are 2 players. ";
                break;
            case ConvParams.userNamesScene:
                ssml = "Try saying: I choose the name, and add your user name";
                break;
            case ConvParams.gameSelectionScene:
                ssml = "Try saying something like: Let's play game number, and the game you want to play";
                break;
            case ConvParams.gameplayScene:
                game.giveHint();
                break;
            default:
                ssml = "I don't have any examples to give right now. ";
                break;
        }

        Game.say(ssml, conv);
    }
});

/**
 * Intent Handler that is hooked when user intent for passing or skipping a question is matched during active gameplay
 */
app.handle('skipQuestion', (conv) => {
    const game = new Game(conv.session.params.plainGameObject, conv);
    game.nextQuestion();
});

/**
 * Intent Handler that is hooked when user intent for help or hint is matched during active gameplay
 */
app.handle('giveHint', (conv) => {
    const game = new Game(conv.session.params.plainGameObject, conv);
    game.giveHint();
});

/**
 * 
 */
app.handle('enterUserNames', (conv) => {
        conv.session.params.plainGameObject.numberOfPlayers = conv.session.params.numberOfPlayers;//save number of players in game object
        conv.session.params.numberOfPlayers = null;//garbage collect old conversation parameter.
});

/**
 * Global Intent handler for pause Intent.
 */
app.handle('pause', (conv) => {
    Game.say(`Alright. I'll pause for 10 seconds. ${AudioConstants.PAUSE}`, conv);
});

/**
 * Intent Handler for mailing game summary when game finishes naturally.
 */
app.handle('printSummary', (conv) => {
    const game = new Game(conv.session.params.plainGameObject, conv);
    game.printSummary();
});

/**
 * Handles printing of unfinished game summaries
 */
app.handle('unfinishedSummary', (conv) => {
    //check if game object exists and gameplay started
    if (isUndefined(conv.session.params.plainGameObject) || conv.scene.name !== ConvParams.gameplayScene)
        return;//do nothing
    //if game play started -> email summary
    else {
        const game = new Game(conv.session.params.plainGameObject, conv);
        game.printSummary();
    }
});

/**
 * Intent Handler for numeric parsing of game number from string to int.
 * a quick and dirty fix for a conversion issue.
 */
app.handle('validateGameNumber', (conv) => {
    conv.session.params.gameNumber = parseInt(conv.intent.params.gamenumber.resolved);
});

/**
 * when Intent.NO_MATCH is matched more than 2 times during player count selection,
 *      the game will default to 2 players.
 * consider removing this and change with better flow.
 */
app.handle('numberOfPlayersNoMatch', (conv) => {
    conv.session.params.numberOfPlayers = 2;
});

/**
 * This Intent handler is hooked when anything other than specific name selection utterance is matched, during name selection scene.
 */
app.handle('nonTypeName', (conv) => {
    const nonTypeName = conv.intent.params.anyType.resolved;// retrieve general user utterance.
    //TODO - move to phrase constants and diversify
    Game.say(Utils.randResponse([`I don't think ${nonTypeName} will work. choose a name from the app list. try again? `,
                                    `I can't use ${nonTypeName}, try again?`, `${nonTypeName} won't work with this game. pick a different one?`]), conv);
});


//connect webhook fulfillment with app handler
exports.fulfillment = functions.https.onRequest(app);
