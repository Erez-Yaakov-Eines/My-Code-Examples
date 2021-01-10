
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

const ConvParams = require('../constants/ConvParams');
const Utils = require('../classes/Utils');
const AudioConstants = require('../constants/AudioConstants');
const {cloneDeep} = require('lodash');
const Player = require('./Player');
const PhraseConstants = require('../constants/PhraseConstants');
const { ConversationV3 } = require('@assistant/conversation');

//Weak mappings between a game object and privatized properties
const _shortAnswers = new WeakMap();
const _longAnswers = new WeakMap();
const _hints = new WeakMap();
const _name = new WeakMap();
const _questions = new WeakMap();
const _instructions = new WeakMap();
const _users = new WeakMap();

/**
 * Class Game is used to save game state between conversational turns
 */
class Game {
    /**
     * 
     * @param {object} plainGameObject a flat game object
     * @param {ConversationV3} conv Reference to the current conversation object
     */
    constructor(plainGameObject, conv) {
        let { userSaid, resolvedParam } = getCommonGameParameters(conv);

        _shortAnswers.set(this, cloneDeep(plainGameObject.answers.short));
        _longAnswers.set(this, cloneDeep(plainGameObject.answers.long));
        _hints.set(this, cloneDeep(plainGameObject.hints));     
        _name.set(this, cloneDeep(plainGameObject.name));
        _questions.set(this, cloneDeep(plainGameObject.questions));
        _instructions.set(this, cloneDeep(plainGameObject.instructions));
        _users.set(this, cloneDeep(plainGameObject.users));

        //TODO - Privatize all properties
        this.numberOfPlayers = plainGameObject.numberOfPlayers;
        this.numberOfQuestions = plainGameObject.numberOfQuestions;
        this.userSaid = userSaid;
        this.resolvedParam = resolvedParam;
        this.currentQuestion = plainGameObject.currentQuestion;
        this.currentPlayerIndex = plainGameObject.currentPlayerIndex;
        this.attempts = plainGameObject.attempts;
        this.conv = conv;
    }

    //Class constant
    static get MAX_PLAYERS() {
        return 3;
    }

    /**
     * @param {number} size the number of distinct games availiable for play in the database
     * @returns an initialized new flat game object.
     */
    static startingGameObject(size) {
        return {
            MAX_PLAYERS : this.MAX_PLAYERS,
            currentUser : 1,
            currentQuestion : 0,
            currentPlayerIndex : 0,
            attempts : 0,
            MAX_GAME : size,
            users: []
            };
    }

    //Privatized getters
    get shortAnswers() {
        return cloneDeep(_shortAnswers.get(this));
    }

    get correctShortAnswer() {
        return this.shortAnswers[this.currentQuestion].toLowerCase();
    }

    get longAnswers() {
        return cloneDeep(_longAnswers.get(this));
    }

    get correctLongAnswer() {
        return this.longAnswers[this.currentQuestion].toLowerCase();
    }

    get hints() {
        return cloneDeep(_hints.get(this));
    }

    get name() {
        return cloneDeep(_name.get(this));
    }

    get questions() {
        return cloneDeep(_questions.get(this));
    }

    get instructions() {
        return cloneDeep(_instructions.get(this));
    }

    get users() {
        return cloneDeep(_users.get(this));
    }
    
    /**
     * Saves a flat game object back to conversation parameters
     */
    toPlainObject() {
       this.conv.session.params.plainGameObject =  {
            answers: {short: this.shortAnswers, long: this.longAnswers},
            users: this.users,
            hints: this.hints,
            instructions: this.instructions,
            name: this.name,
            questions: this.questions,
            numberOfQuestions : this.numberOfQuestions,
            currentQuestion : this.currentQuestion,
            numberOfPlayers : this.numberOfPlayers,
            currentPlayerIndex : this.currentPlayerIndex,
            attempts : this.attempts,
        };
    }

    /**
     * 
     * @param {string} sceneName 
     * 
     * Sets the next conversational scene.
     * Note - the scene changes immediately after this webhook response
     */
    nextScene(sceneName) {
        this.conv.scene.next.name = sceneName;
    }
    
    /**
     * @returns {boolean} true if current question is last game question, else false
     */
    isLastAnswer() {
        return (this.numberOfQuestions === this.currentQuestion);
    }

    /**
     * Moves to the next question.
     * 
     * Increments current question and current player,
     *  resets attempts and saves game object back to conversation.
     */
    nextQuestion() {
        this.currentQuestion++;
        this.attempts = 0;
        this.currentPlayerIndex = Utils.modularIncrement(this.currentPlayerIndex, this.numberOfPlayers);
        //case last question, move to wrap up
        if (this.isLastAnswer())
            this.nextScene(ConvParams.wrapUpScene);
        this.toPlainObject();
    }

    /**
     * prompts user with hint for current question,
     * writes player statistics, then writes game object back to conversation
     */
    giveHint() {
        const player = new Player(this.getCurrentPlayerPlain());
        player.recievedHint();
        this.setCurrentPlayerPlain(player.toPlainObject());
        const currentHintArray = _hints.get(this)['hint ' + (this.currentQuestion + 1)];
        const actualHint = Utils.randResponse(currentHintArray);
        Game.say(`${AudioConstants.HINT} here's a hint: ${actualHint}. `, this.conv);

        this.toPlainObject();
    }

    /**
     * @returns {boolean} true if resolved parameters from conversation utterance exist in correct answer data,
     *                      else false
     */
    isUserCorrect() {
        if (this.resolvedParam === undefined) return false;
        return (Utils.phraseContains(this.correctShortAnswer, this.resolvedParam)
         || Utils.phraseContains(this.correctLongAnswer, this.resolvedParam));
    }

    /**
     * Handles Correct short answers.
     * 
     * Writes relevant (correct short answer) statistics to current player,
     * prompts user for correcting short answer, then writes game object back to conversation.
     * 
     * TODO - join into single method similar to incorrect(isShort)
     */
    correctButShort() {
        const player = new Player(this.getCurrentPlayerPlain());
        player.correctAnswer(true, this.userSaid, this.currentQuestion + 1);
        this.setCurrentPlayerPlain(player.toPlainObject());

        Game.say(`your answer might be correct, but you should try to answer a complete sentence. 
            try saying something like: ${this.correctLongAnswer}.`, this.conv);
        this.toPlainObject();
    }

    /**
     * Handles correct long answers.
     * 
     * Writes relevant (correct long answer) statistics to current player,
     * prompts user for success, then writes game object back to conversation.
     * 
     * TODO - join into single method similar to incorrect(isShort)
     */
    correct() {
        const player = new Player(this.getCurrentPlayerPlain());
        player.correctAnswer(false, this.userSaid, this.currentQuestion + 1);
        const name = player.userName;
        this.setCurrentPlayerPlain(player.toPlainObject());
        //TODO - move to Phrase constants
        Game.say(`${AudioConstants.CORRECT}` + Utils.randResponse(['Very good ' + name + '.', 
                name + ', that is correct. ',
                'You are correct ' + name + '. ']), this.conv);
        this.nextQuestion();
    }

    /**
     * 
     * @param {boolean} isShort true indicates short answer, else long
     * 
     * Handles user incorrect answers.
     * 
     * Writes appropriate statistics to current user,
     * prompts accordingly with audio cues, then writes game object back to conversation.
     */
    incorrect(isShort) {
        this.attempts++;
        const player = new Player(this.getCurrentPlayerPlain());
        const name = player.userName;
        if (isShort)
            player.incorrectAnswer(true, this.userSaid, this.currentQuestion + 1);
        else
            player.incorrectAnswer(false, this.userSaid, this.currentQuestion + 1);

        this.setCurrentPlayerPlain(player.toPlainObject());

        if (this.attempts >= 2)
        {
            Game.say(`${AudioConstants.WRONG_ANSWER}` + name + ', '
             + Utils.randResponse(PhraseConstants.wrongAnswerFinal) + '. moving to the next question. ', this.conv);
            this.nextQuestion();
            return;
        }

        Game.say(`${AudioConstants.WRONG_ANSWER}` + name + ', '
        + Utils.randResponse(PhraseConstants.wrongAnswer) , this.conv);
        this.toPlainObject();

    }

    /**
     * Emails the current game summary (finished or unfinished) to the current email registered
     * in User storage (of the linked google account from account linking flow)
     */
    printSummary() {
        let summary = '';
        if (!this.isLastAnswer()) {
            summary += 'Unfinished Game ' + `"` + this.name + `"` + ': ' + (this.currentQuestion + 1) + '/' + this.numberOfQuestions + ' questions.\n';
        }
        else
            summary = `"` + this.name + `"` + ' Game Summary:\n';

        summary += '\n######################################\n';
        this.users.forEach((user) => {
            let currentUser = new Player(user);
            summary += currentUser.printSummary();
            summary += '\n######################################\n';
        });
		
        Utils.sendMail(this.conv.session.params.email, summary, `Game summary from "English Fun & Games"`);
    }

    /**
     * @returns {object} the current player as a flat object.
     */
    getCurrentPlayerPlain() {
        return this.users[this.currentPlayerIndex];
    }

    /**
     * @param {object} currentPlayerPlainObject The current player flat object to write back to conversation parameters
     */
    setCurrentPlayerPlain(currentPlayerPlainObject) {
        _users.get(this)[this.currentPlayerIndex] = currentPlayerPlainObject;
    }

    /**
     * 
     * @param {string} ssml 
     * @param {ConversationV3} conv 
     * 
     * Static Wrapper for delivering prompts at 90% prosody rate
     */
    static say(ssml, conv) {
        ssml = `<speak><prosody rate='90%'>` + ssml + '</prosody></speak>'
        conv.add(ssml);
    }
}

/**
 * 
 * @param {ConversationV3} conv 
 * 
 * @returns {object} the common parameters elicited from user utterance,
 *          namely: the utterance and a parameter.
 */
function getCommonGameParameters(conv) {
    const userSaid = conv.intent.query.toLowerCase();
    let resolvedParam;

    if (conv.intent.params.answerParam !== undefined)
        resolvedParam = (conv.intent.params.answerParam.resolved).toLowerCase();
    else 
        resolvedParam = undefined;

    return {
        userSaid : userSaid,
        resolvedParam : resolvedParam,
    };
}

module.exports = Game;