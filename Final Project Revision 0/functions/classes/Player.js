
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

const Answer = require('./Answer');
const {cloneDeep} = require('lodash');

//Weak mappings between player objects and privatized properties
const _userName = new WeakMap();
const _correct = new WeakMap();
const _incorrect = new WeakMap();
const _hints = new WeakMap();
const _shortAns = new WeakMap();
const _longAns = new WeakMap();
const _answers = new WeakMap();

/**
 * Class Player is used for saving the state of users between conversational turns.
 */
class Player {
    constructor(plainPlayerObject) {
        _userName.set(this, plainPlayerObject.userName);
        _correct.set(this, plainPlayerObject.correct);
        _incorrect.set(this, plainPlayerObject.incorrect);
        _hints.set(this, plainPlayerObject.hints);
        _shortAns.set(this, plainPlayerObject.shortAns);
        _longAns.set(this, plainPlayerObject.longAns);
        _answers.set(this, cloneDeep(plainPlayerObject.answers));
    }

    //Getters - all getters return copies of privatized properties
    get userName() {
        return _userName.get(this);
    }

    get correct() {
        return _correct.get(this);
    }

    get incorrect() {
        return _incorrect.get(this);
    }

    get hints() {
        return _hints.get(this);
    }

    get shortAns() {
        return _shortAns.get(this);
    }

    get longAns() {
        return _longAns.get(this);
    }

    get answers() {
        return cloneDeep(_answers.get(this));
    }

    /**
     * @returns an initialized new flat player object.
     */
    static defaultPlayerObject() {
        return {
            correct: 0,
            incorrect: 0,
            hints: 0,
            shortAns: 0,
            longAns: 0,
            answers: []
        }
    }

    /**
     * 
     * @param {boolean} isShort true means correct short answer, else long
     * @param {string} answer the user utterance detected as correct by the NLU
     * @param {number} questionNum the question the user tried to answer 
     * 
     * Sets the appropriate statistics values into the player object.
     * 
     * TODO - combine correct and incorrect methods into single method to prevent code duplication
     */
    correctAnswer(isShort, answer, questionNum) {
        _answers.get(this).push(new Answer(answer, questionNum));
        if (isShort)
        {
            _shortAns.set(this, _shortAns.get(this) + 1);
        }
        else
        {
            _longAns.set(this, _longAns.get(this) + 1);
            _correct.set(this,_correct.get(this) + 1 );
        }
    }

    /**
     * 
     * @param {boolean} isShort true means incorrect short answer, else long
     * @param {string} answer the user utterance detected as incorrect by the NLU
     * @param {number} questionNum the question the user tried to answer
     * 
     * Sets the appropriate statistics values into the player object.
     * 
     * TODO - combine correct and incorrect methods into single method to prevent code duplication
     */
    incorrectAnswer(isShort, answer, questionNum) {
        _answers.get(this).push(new Answer(answer, questionNum));
        _incorrect.set(this, _incorrect.get(this) + 1);
        if (isShort)
        {
            _shortAns.set(this, _shortAns.get(this) + 1);
        }
        else
        {
            _longAns.set(this, _longAns.get(this) + 1);
        }
    }

    /**
     * A very simplistic incrementer of hint counter
     */
    recievedHint() {
        _hints.set(this, _hints.get(this) + 1);
    }

    /**
     * @returns a flat player object to save back to conversation parameters
     */
    toPlainObject() {
        return {
            userName: this.userName,
            correct: this.correct,
            incorrect: this.incorrect,
            hints: this.hints,
            shortAns: this.shortAns,
            longAns: this.longAns,
            answers: this.answers
        }
    }

    /**
     * @returns a simple string representation of player statistics
     */
    printSummary() {
        let summary = '';
        summary += 'Summary for player ' + this.userName + '\n';
        summary += '---------------------------------------------' + '\n'
        summary += 'Correct answers: ' + this.correct + '.\n';
        summary += 'Incorrect answers: ' + this.incorrect + '.\n';
        summary += 'hints: ' + this.hints + '.\n';
        summary += 'short answers: ' + this.shortAns + '.\n';
        summary += 'long answers: ' + this.longAns + '.\n';
        summary += '\nanswers:\n------\n';
        this.answers.forEach((element, i) => {
            let answer = new Answer(element.rawText, element.questionNum)
            summary += '    answer ' + i + '\n'
                +   '       ' + answer.toString() + '.\n';
        });
        return summary;
    }

    toString() {
        return this.printSummary();
    }
}

module.exports = Player;