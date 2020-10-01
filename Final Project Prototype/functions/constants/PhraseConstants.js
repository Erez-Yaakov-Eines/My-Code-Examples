'use strict';

const GameConst = require(`./GameConstants.js`);
const GameConstants = new GameConst();

class PhraseConstants {

    get welcomePhrases() {
        return [`Welcome to Animals & Colors game for the 4th grade! How many players are playing today?`
        , `Welcome to The Animals & Colors worksheet for the 4th grade! How many players will be playing?`
        , `Welcome to Animals & Colors for the 4th grade! tell me, how many players want to participate?`];
    }

    get numberOfPlayersFaultPhrases() {
        return [`The number of players is limited to ` + GameConstants.MAX_PLAYERS + ` players, can you try again?`,
        `The number of players can only be a number between 1 and ` + GameConstants.MAX_PLAYERS + `. Can you please try again?` ,
        `That many players won't work. Choose a number between 1 and ` + GameConstants.MAX_PLAYERS +`. Can you try again?`];
    }

    get numberOfPlayersNaNPlayers() {
        return [`i'm pretty sure that's not a number. can you try again?`,
        `That probably won't work if I tried. can you pick a number please?`];
    }

    get faultExitPhrases() {
        return [`I'm very sorry, but an unexpected error has occured. you can try activating me again after this conversation ends. goodbye!`];
    } 

    get startNicknamePhrases() {
        return  [`Very well. Let's start with your nicknames. Player 1, what is your nickname?`,
        `great! Player 1, what nickname would you like to use?`,
        `Ok! let's hear everyone's nickname. Player 1, How should I call you?`];
    }

    get restartNumberOfPlayersPhrases() {
        return  [`Sure. Let's try again. How many players would like to play?`,
        `No worries, we can correct it. How many players are playing today?`];
    }
    
    get gameStartingPhrases() {
        return  [`Let's start the game!`, `Are you ready? let's go!`, `everyone ready? let's start!`, `I'm ready to start playing!`];
    }
    
    get instructionPhrases() {
        return  [`when I call your name, answer a question on the app`, `each player in his turn will answer a question from the worksheet`];
    }
    
    get correctAnswers() {
        return [`the dog is brown`, `the cat is white`, `the fish is blue`, `the bird is yellow`, `the shark is grey`, `the flamingo is pink`, `the horse is black`];
    }

    get nicknameNotAvailiable() {
        return [`I'm sorry, but this nickname is already taken. can you choose another?`];
    }

    get hints() {
        return [[`the dog`,`is brown`],[`the cat`,`is white`],[`is blue`,`the fish`],[`the bird`,`is yellow`],[`the shark`,`is grey`],[`the flamingo`,`is pink`],[`the horse`,`is black`]];
    }

    get gameplayFallbacks() {
        return [`I didn't quite get that. can you answer again slower?`, `I couldn't hear that. one more time but slower?`];
    }

    get almostCorrect() {
        return [`that's almost correct. can you try again?`, `that is close, but not quite right. you can do it! try again?`];
    }

    get repeatPhrases() {
        return [`Repeating: `, `Sure, I will Repeat: `, `One more time: `, `Here it is: `];
    }
}

module.exports = PhraseConstants;