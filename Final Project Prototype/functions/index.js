/******************** ANIMALS & COLORS GAME - 4th GRADE ********************/
//TODO - Refactor all functions using classes of class diagram objects.
//TODO - seperate file for phrase definitions + context definitions
//TODO - Figure out how to randomize String[] to add in agent instead of randomizing my own.
//TODO - type all parameters.
//TODO - Move all constant strings and numbers to global constant definitions in a seperate file.
//TODO - migrate all deprecated agent methods
//TODO - Document all functions.
//TODO - Validate user JWT token and save statistics to DB.
//TODO - Implement hints and repeats.
//TODO - check if async await is actually necessary for nodemailer to properly send email.

'use strict';

/***************  CONSTANTS ***************/

const Player = require(`./classes/Player.js`);
const PhraseConst = require(`./constants/PhraseConstants.js`);
const PhraseConstants = new PhraseConst();
const GameConst = require(`./constants/GameConstants.js`);
const GameConstants = new GameConst();
const ParameterConst = require(`./constants/ParameterConstants.js`);
const ParameterConstants = new ParameterConst();
const ContextConst = require(`./constants/ContextConstants.js`);
const ContextConstants = new ContextConst();

/*************** END - CONSTANTS ***************/


//const dialogflow = require(`actions-on-google`);
const functions = require('firebase-functions');
const {WebhookClient} = require('dialogflow-fulfillment');
const jwt = require('jsonwebtoken');

 //TODO - replace hard coded certificate for testing purposes with OAuth kid matching
  //TODO - add Token iss and aud verification and alg=none attack checks.
  var googleCert = "-----BEGIN CERTIFICATE-----\nMIIDJjCCAg6gAwIBAgIIIXsXccCiLZgwDQYJKoZIhvcNAQEFBQAwNjE0MDIGA1UE\nAxMrZmVkZXJhdGVkLXNpZ25vbi5zeXN0ZW0uZ3NlcnZpY2VhY2NvdW50LmNvbTAe\nFw0yMDA4MTIwNDI5MzdaFw0yMDA4MjgxNjQ0MzdaMDYxNDAyBgNVBAMTK2ZlZGVy\nYXRlZC1zaWdub24uc3lzdGVtLmdzZXJ2aWNlYWNjb3VudC5jb20wggEiMA0GCSqG\nSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCimsh/XiCNzhCMjhhQLQI6SfgoCOQF5oRi\nI+bqOjUb2CeAoZ+WaZ+y+JHwzesgqz3oPOC6szZTVfZNQgOGoG80e1Q41yVXpqcB\nHbbQ9xLvaX/+sKj21FDGTulQNBJnFtuLprS3nDfkXIGgXpqA9Irf6cR86TX+/APW\nmK+jlfuWu8dK7/SVn4h9o0X4OESsZN5ZvZm7h84y1p1Z9EmtOndR1pYW8dnGRVks\nhAY0V4RwpqlWkAHZ7LxubPPWtgfHQsD4NYzGF+IG8DzV7rxLZ0Py6QeMO2DOHaWR\nYl98n4fudwXtKWL9EG5ZKFaXSrQK0h5bk8GTBUKauS1m0g09XuHlAgMBAAGjODA2\nMAwGA1UdEwEB/wQCMAAwDgYDVR0PAQH/BAQDAgeAMBYGA1UdJQEB/wQMMAoGCCsG\nAQUFBwMCMA0GCSqGSIb3DQEBBQUAA4IBAQChE2pNEWNYtm56IUNuDGrv7JWL5D0n\nIwiW3mhVbKNDGDaiVBExdPEKiWakHjSuJ8NUsDtHYlYWZMNxkmc59MCDGU4pSsaq\nfjaETd1xh+TSvhA073YYsK9kEkmoghKGmyX4GjpM+CrQKNiJ8p+k/1X8nm5708LN\n7U+B458hHT5Lk7EEyPsE5CExzifLDtXNppSTYq35lJnK7W9x9YYIoWugSFY3D9W6\nZvkmjhQCwJcHGkZJQ6l+naQQgH6hkwg36wmBniiFpNz8Fw4JElDWBv9OgyncDqhg\nEQLKBdoaJUwL07E2czgY8G2lomdrXs5TTyJwEvP61Ma5+KU1VuZutWjr\n-----END CERTIFICATE-----\n"


process.env.DEBUG = 'dialogflow:debug'; // enables lib debugging statements

//init firebase realtime database with admin privileges
var db = getFirebaseRT_DB();

/**
 * This is the exported method (hosted on firebase cloude functions) that is called 
 * upon request by a proper webhook object,
 * and is an annonymous function inside `onRequest`, meaning the closing right bracket of
 * onRequest parameters is the last closing bracket of this file.
 */
exports.dialogflowFirebaseFulfillment = functions.https.onRequest((request, response) => {
  //agent holds all parameters passed in the conversation including contexts, dialogflow identified
  //parameter keywords, matched user intents and so on...
  const agent = new WebhookClient({ request, response });

  // console.log('Dialogflow Request headers: ' + JSON.stringify(request.headers));
  // console.log('Dialogflow Request body: ' + JSON.stringify(request.body));
 
  /************************ INTENT METHODS *******************************/
  /**
   * This is the initiating MAIN Intent of the conversation.
   * Users trigger this method when they invoke this conversational app.
   * @param {WebhookClient} agent 
   * @returns {void} - Conversational Fullfilment.
   */
  function welcome(agent)
  {
      agent.context.set(ContextConstants.defaultGetNumberOfPlayersContext); 
      agent.add(randomizeResponse(PhraseConstants.welcomePhrases));
  }
 
  /**
   * 
   * @param {WebhookClient} agent 
   */
  function getNumberOfPlayers(agent)
  {
    try {
      //retrieve user number from context.
      let numberOfPlayers = parseInt(agent.parameters['numberOfPlayers']);
      //error handling
      if (isNaN(numberOfPlayers)) {
        agent.context.set(ContextConstants.defaultGetNumberOfPlayersContext);
        agent.add(randomizeResponse(PhraseConstants.numberOfPlayersNaNPlayers));
      }//verify number is "reasonable"
      else if (numberOfPlayers <= 0 || numberOfPlayers > GameConstants.MAX_PLAYERS) {
        agent.context.set(ContextConstants.defaultGetNumberOfPlayersContext);
        agent.add(randomizeResponse(PhraseConstants.numberOfPlayersFaultPhrases));
      }//"all" ok, proceed to verify number with player.
      else {
        agent.context.set({name: 'get_number_of_players_verify', lifespan: '4', parameters: {numberOfPlayers: numberOfPlayers} });
        agent.add(`You chose ${numberOfPlayers} players. is that correct?`);
      }
    }
    catch (err) {
      console.log(err.message);
      close(randomizeResponse(PhraseConstants.faultExitPhrases));
    }
  }

  /**
   * 
   * @param {WebhookClient} agent 
   */
  function getNumberOfPlayersVerifyPossitive(agent)
  {
    //retrieve relevant context
    const playersVerifyContext = agent.context.get(ContextConstants.VERIFY_PLAYERS_CONTEXT_NAME);
    //error handling
    if (playersVerifyContext == undefined) {
      console.log(`Error! - context ` + ContextConstants.VERIFY_PLAYERS_CONTEXT_NAME + ` is undefined in function 'getNumberOfPlayersVerifyPossitive'`);
      close(randomizeResponse(PhraseConstants.faultExitPhrases));
    }
    //retrieve number of players parameter and pass to next context.
    const numberOfPlayers = playersVerifyContext.parameters[ParameterConstants.numberOfPlayers];
    const nicknameContext = {name: ContextConstants.NICKNAMES_CONTEXT_NAME,
                             lifespan: ContextConstants.DEFAULT_LIFESPAN,
                             parameters: {numberOfPlayers: numberOfPlayers,
                                          //nicknames: [],
                                          currentPlayerIndex: 0,
                                          playerObjects: []}
                            };

    clearAllCurrentConversationalContexts(agent)
    agent.context.set(nicknameContext);
    agent.add(randomizeResponse(PhraseConstants.startNicknamePhrases));
  }

  /**
   * Resets conversational flow to get number of players
   * @param {WebhookClient} agent 
   */
  function getNumberOfPlayersVerifyNegative(agent)
  {
    clearAllCurrentConversationalContexts(agent);
    agent.context.set(ContextConstants.defaultGetNumberOfPlayersContext);
    agent.add(randomizeResponse(PhraseConstants.restartNumberOfPlayersPhrases));
  }

  /**
   * 
   * @param {WebhookClient} agent 
   */
  function setNicknames(agent)
  {
    try 
    {
      const currNickname = agent.parameters[ParameterConstants.convParamUserNickname];// get user nickname parameter from the webhook request
      let currNicknameContext = agent.context.get(ContextConstants.NICKNAMES_CONTEXT_NAME);
      //let nicknames = currNicknameContext.parameters[ParameterConstants.nicknames];
      let players = currNicknameContext.parameters[ParameterConstants.playerObjects];
      //check for duplicate nicknames for this game instance (will later have database entries of useable nicknames per teacher as well)
      if (!isNicknameAvailiable(currNickname,players))
      {
        agent.add(randomizeResponse(PhraseConstants.nicknameNotAvailiable));
        currNicknameContext.lifespan += 1;
        agent.context.set(currNicknameContext);
        return;
      }
      //update next player index, save nickname in string array and prompt next user.
      let currentPlayerIndex = parseInt(currNicknameContext.parameters[ParameterConstants.currentPlayerIndex]) + 1;
      let currentPlayerObject = new Player(currNickname, currentPlayerIndex, 0, 0, 0);
      currNicknameContext.parameters[ParameterConstants.playerObjects].push(currentPlayerObject);
      //nicknames.push(currNickname);
      agent.add(`player ` + currentPlayerIndex + `, your nickname is ` + currNickname);
      //currNicknameContext.parameters[ParameterConstants.nicknames] = nicknames;
      currNicknameContext.parameters[ParameterConstants.currentPlayerIndex] = currentPlayerIndex;
      //check if all players recieved nicknames and set gameplay context
      if (currentPlayerIndex == currNicknameContext.parameters[ParameterConstants.numberOfPlayers]) 
      {
        let firstPlayerNickname = ((currNicknameContext.parameters[ParameterConstants.playerObjects])[0]).m_nickname;
        let nextResponse = randomizeResponse(PhraseConstants.gameStartingPhrases) + `. ` + 
          randomizeResponse(PhraseConstants.instructionPhrases) + `. ` +
          firstPlayerNickname + `, It's your turn. answer question 1 in the worksheet`;
        agent.add(nextResponse);
        clearAllCurrentConversationalContexts(agent);
        let gameContext = {name: ContextConstants.GAME_CONTEXT_NAME,
                           lifespan: 99,
                            parameters: {numberOfPlayers: currNicknameContext.parameters[ParameterConstants.numberOfPlayers], currentQuestion: 0, playerObjects: players}};
        agent.context.set(gameContext);
      }
      //continue getting player nicknames.
      else
      {
        agent.add(`Player ` + (parseInt(currentPlayerIndex) + 1) + `, choose your nickname`);
        currNicknameContext.lifespan = 5;
        agent.context.set(currNicknameContext);
      }
    }
    catch (err)
    {
      agent.add("Error! - " + err.message);
    }
  }

  /**
   * 
   * @param {WebhookClient} agent 
   * @throws Will throw an error if question index overflows maximum number of questions for this game. 
   */
  function gameplay(agent) {//TODO - change context object and then overrun with new context instead of manualy populating

    // This usage of classes will replace the messy code inside the try/catch blocks.
    /* const gameContext = agent.context.get(ContextConstants.GAME_CONTEXT_NAME);
    if (gameContext == undefined) throw new Error(`Error! - gameContext undefined in function gameplay`);
    
    let gameTurn = new Game(gameContext);

    gameTurn.play(); */
    try {
      let conv = agent.conv();
      let currentGameContext = agent.context.get(ContextConstants.GAME_CONTEXT_NAME);
      //const nicknames = currentGameContext.parameters[ParameterConstants.nicknames];
      let players = currentGameContext.parameters[ParameterConstants.playerObjects];
      let numberOfPlayers = parseInt(currentGameContext.parameters[ParameterConstants.numberOfPlayers]);
      let currentQuestion = parseInt(currentGameContext.parameters[ParameterConstants.currentQuestion]);
      let currentPlayerIndex = (currentQuestion % numberOfPlayers);
      console.log("DEBUG LOG: " + players[currentPlayerIndex] + "\n" + "currentPlayerIndex = " + currentPlayerIndex);
      const currentNickname = players[currentPlayerIndex].m_nickname;
      let responseSsml = `<speak>`;

      if (agent.query.toLowerCase() != PhraseConstants.correctAnswers[currentQuestion].toLowerCase())
      {
        responseSsml = responseSsml + `<audio src="https://firebasestorage.googleapis.com/v0/b/x4th-grad-game1-akmqmt.appspot.com/o/wah_wah_sound.ogg?alt=media">
        <desc>Ding sound.</desc>
        Audio resource for a Ding sound failed to load.
      </audio>`
        responseSsml = responseSsml + currentNickname + `, ` + randomizeResponse(PhraseConstants.almostCorrect);//agent.add(currentNickname + `, ` + randomizeResponse(PhraseConstants.almostCorrect));
        responseSsml = responseSsml + `here's a hint: ` + randomizeResponse(PhraseConstants.hints[currentQuestion]) + `</speak>`;//agent.add(`here's a hint: ` + randomizeResponse(PhraseConstants.hints[currentQuestion]));
        players[currentPlayerIndex].m_incorrect++;
        players[currentPlayerIndex].m_hints++;// TODO - add hint used in hint/help intents
        clearAllCurrentConversationalContexts(agent);
        currentGameContext.parameters[ParameterConstants.playerObjects] = players;
        currentGameContext.lifespan = 99;
        agent.context.set(currentGameContext);
        conv.ask(responseSsml);
        agent.add(conv);
        return;
      }
      else

      responseSsml = responseSsml + `<audio src="https://firebasestorage.googleapis.com/v0/b/x4th-grad-game1-akmqmt.appspot.com/o/Ding_Sound_Effect.ogg?alt=media">
        <desc>Ding sound.</desc>
        Audio resource for a Ding sound failed to load.
      </audio>
      ` + `You are correct ` + currentNickname + `. `;          
        players[currentPlayerIndex].m_correct++;

      currentQuestion = currentQuestion + 1;
      if (currentQuestion > (GameConstants.NUMBER_OF_QUESTIONS - 1))
      {
        responseSsml = responseSsml + `this concludes our game.` + 
        `<audio src="https://firebasestorage.googleapis.com/v0/b/x4th-grad-game1-akmqmt.appspot.com/o/Audience_Clapping.ogg?alt=media">
        <desc>Audience clapping sound.</desc>
        Audio resource for Audience clapping sound failed to load.
      </audio>`
        +
         ` Thank you all for playing. I hope you'll come again soon. goodbye!` + `</speak>`;
        conv.close(responseSsml);//agent.add(`this concludes our game. Thank you all for playing. I hope you'll come again soon. goodbye!`);
        for (let i in currentGameContext.parameters[ParameterConstants.playerObjects])
        {
          db.ref("testing").push(currentGameContext.parameters[ParameterConstants.playerObjects][parseInt(i)]);
        }
        agent.add(conv);
      }

      responseSsml = responseSsml + players[currentQuestion % numberOfPlayers].m_nickname + `, it's your turn to answer question ` + (currentQuestion + 1) + `</speak>`;//agent.add(players[currentQuestion % numberOfPlayers].m_nickname + `, it's your turn to answer question ` + (currentQuestion + 1));
      clearAllCurrentConversationalContexts(agent);
      currentGameContext.parameters[ParameterConstants.currentQuestion] = currentQuestion;
      currentGameContext.parameters[ParameterConstants.playerObjects] = players;
      currentGameContext.lifespan = 99;
      agent.context.set(currentGameContext);
      conv.ask(responseSsml);
      agent.add(conv);
    }
    catch (err) {
      agent.add(`gameplay function:` + err.message);
    }
  }

  /**
   * 
   * @param {WebhookClient} agent 
   */
  function giveHint(agent) {
    let currentGameContext = agent.context.get(ContextConstants.GAME_CONTEXT_NAME);
    let currentQuestion = parseInt(currentGameContext.parameters[ParameterConstants.currentQuestion]);
    let players = currentGameContext.parameters[ParameterConstants.playerObjects];
    let numberOfPlayers = parseInt(currentGameContext.parameters[ParameterConstants.numberOfPlayers]);
    let currentPlayerIndex = (currentQuestion % numberOfPlayers);

    agent.add(randomizeResponse(PhraseConstants.hints[currentQuestion]));
    players[currentPlayerIndex].m_hint++;

  }

 /**
   * 
   * @param {WebhookClient} agent 
   */
  function giveRepeat(agent) {
    let currentGameContext = agent.context.get(ContextConstants.GAME_CONTEXT_NAME);
    let players = currentGameContext.parameters[ParameterConstants.playerObjects];
    let numberOfPlayers = parseInt(currentGameContext.parameters[ParameterConstants.numberOfPlayers]);
    let currentQuestion = parseInt(currentGameContext.parameters[ParameterConstants.currentQuestion]);
    let currentPlayerIndex = (currentQuestion % numberOfPlayers);
    const currentNickname = players[currentPlayerIndex].m_nickname;

    agent.add(randomizeResponse(PhraseConstants.repeatPhrases));
    agent.add(players[currentQuestion % numberOfPlayers].m_nickname + `, it's your turn to answer question ` + (currentQuestion + 1));
    clearAllCurrentConversationalContexts(agent);
    currentGameContext.parameters[ParameterConstants.currentQuestion] = currentQuestion;
    currentGameContext.lifespan = 99;
    agent.context.set(currentGameContext);  }

  /**
   * 
   * @param {WebhookClient} agent 
   */
  function gameplayFallback(agent) {
    let currentGameContext = agent.context.get(ContextConstants.GAME_CONTEXT_NAME);
    //const nicknames = currentGameContext.parameters[ParameterConstants.nicknames];
    let players = currentGameContext.parameters[ParameterConstants.playerObjects];
    let numberOfPlayers = parseInt(currentGameContext.parameters[ParameterConstants.numberOfPlayers]);
    let currentQuestion = parseInt(currentGameContext.parameters[ParameterConstants.currentQuestion]);
    let currentPlayerIndex = (currentQuestion % numberOfPlayers);
    const currentNickname = players[currentPlayerIndex].nickname;

    agent.add(currentNickname + `, ` + randomizeResponse(PhraseConstants.gameplayFallbacks));
  }

  /**
   * 
   * @param {WebhookClient} agent 
   */
  function fallback(agent) {
    agent.add(`I didn't understand`);
    agent.add(`I'm sorry, can you try again?`);
  }
/********************* END - INTENT METHODS ************************/

/***************  UTILITY FUNCTIONS  *******************/
  /**
   * 
   * @param {String[]} responseArray 
   * @returns {String}
   */
  function randomizeResponse(responseArray) {
    return responseArray[getRandomInt(responseArray.length)];
  }

  /**
   * 
   * @param {number} max 
   * @returns {number}
   */
  function getRandomInt(max) {
    return Math.floor(Math.random() * Math.floor(max));
  }

  /**
   * 
   * @param {String} message 
   */
  function close(message){
    let conv = agent.conv();
    conv.close(message); 
    agent.add(conv);
  }

  /**
   * 
   * @param {WebhookClient} agent 
   */
  function clearAllCurrentConversationalContexts(agent) {
    agent.contexts.forEach((context) => {
      context.lifespan = `-1`;
      agent.context.set(context);
    })
  }

  /**
   * 
   * @param {WebhookClient} agent 
   * @param {String} contextName 
   * @throws Will throw an error if input context does not exist in the conversation.
   */
  function clearSpecificContext(agent, contextName) {
    let contextToClear = agent.context.get(contextName);
    if (contextToClear == undefined) throw new Error(`Error! - tried to clear undefined Context`);
    contextToClear.lifespan = `-1`;
    agent.context.set(contextToClear);
  }

 /**
  * 
  * @param {String} nickname 
  * @param {Player[]} playerObjectsArray 
  * @returns {boolean}
  * @throws Will throw an error if input parameters are not a string and a string array.
  */
  function isNicknameAvailiable(nickname, playerObjectsArray) {
    if (typeof(nickname) != `string` || !Array.isArray(playerObjectsArray)) 
      throw new Error(`Error! - isNicknameAvailiable invalid parameters`);

    return (playerObjectsArray.findIndex(p => p.nickname === nickname) === -1 );
  }

  async function testMail() {
    let nodemailer = require('nodemailer');

    let transporter = nodemailer.createTransport({
      service: 'gmail',
      auth: {
        user: 'mtesty466@gmail.com',
        pass: 'sr6!&uo8yMJJalVOGmCMvk6WV3&nA2ff'
      }
    });

    let mailOptions = {
      from: 'mtesty466@gmail.com',
      to: 'ori.it.92@gmail.com',
      subject: 'Sending Email test from Erez & Ori firebase functions conversational fulfillment',
      text: 'That was not easy!'
    };

    await transporter.sendMail(mailOptions, function(error, info){// await might be unnecessary here because of callback function...
      if (error) {
        console.log(error);
      } else {
        console.log('Email sent: ' + info.response);
      }
    });
  }

  function testDB() {
    var reference = db.ref("teachers");
    var email = getTeacherGmail();
    if (email != null) {
      try {
        let path = pathifyGmail(email);
        reference.child(path).set(
        
        );
        console.log('testDB() called...');
      }
      catch(err) {
        console.log(err);
      }
    }
    else {
      console.log("email returned null.");
    }
  }

  function getTeacherGmail() {
    try {
      let idToken = request.body.originalDetectIntentRequest.payload.user.idToken;
      let verifiedToken = jwt.verify(idToken, googleCert, 'RS256');
      console.log(verifiedToken.email + " - JWT Signature Verified");
      return verifiedToken.email;
    }
    catch (err) {
      console.log(err);
      return null;
    }
  }

  /**
   * 
   * @param {String} address 
   */
  function pathifyGmail(address) {
    let res = address.replace(/\./g, "_DOT_")
				.replace(/#/g, "_HASH_")
				.replace(/\$/g, "_DOLLAR_")
				.replace(/\[/g, "_LBRACKET_")
				.replace(/]/g, "_RBRACKET_");
    return res;
}

/**
 * returns the current player object in a gameplay turn.
 * assumes the game is in active gameplay and does not check context
 */
function getCurrentGamePlayer(agent) {
  let currentGameContext = agent.context.get(ContextConstants.GAME_CONTEXT_NAME);
  const nicknames = currentGameContext.parameters[ParameterConstants.nicknames];
  let numberOfPlayers = parseInt(currentGameContext.parameters[ParameterConstants.numberOfPlayers]);
  let currentQuestion = parseInt(currentGameContext.parameters[ParameterConstants.currentQuestion]);
  let currentPlayerIndex = (currentQuestion % numberOfPlayers);
  const currentNickname = nicknames[currentPlayerIndex];
  return new Player(currentNickname, currentPlayerIndex);
}
  /**************  END - UTILITY FUNCTIONS ****************/

  /********************** INTENT <--> METHOD MAPPING *******************************/

  // Run the proper function handler based on the matched Dialogflow intent name
  let intentMap = new Map();
  intentMap.set('Default Welcome Intent', welcome);
  intentMap.set('Default Fallback Intent', fallback);
  intentMap.set('GetNumberOfPlayers', getNumberOfPlayers);
  intentMap.set('GetNumberOfPlayersVerifyPossitive', getNumberOfPlayersVerifyPossitive);
  intentMap.set('GetNumberOfPlayersVerifyNegative', getNumberOfPlayersVerifyNegative);
  intentMap.set('SetNickNames', setNicknames);
  intentMap.set(`GameplayFallback`, gameplayFallback);
  intentMap.set(`Gameplay`, gameplay);
  intentMap.set(`GameplayRepeat`, giveRepeat);
  intentMap.set(`Hint`, giveHint);

  agent.handleRequest(intentMap);
});

function getFirebaseRT_DB() {
  var admin = require("firebase-admin");
  var serviceAccount = require("./config/x4th-grad-game1-akmqmt-firebase-adminsdk-d3lu5-1a7728fa17.json");
  admin.initializeApp({
    credential: admin.credential.cert(serviceAccount),
    databaseURL: "https://x4th-grad-game1-akmqmt.firebaseio.com/"
  });
  var db = admin.database();
  return db;
}

