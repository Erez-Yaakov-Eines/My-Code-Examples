
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

const Utils = require('../classes/Utils');

class PhraseConstants {

    static get exampleReqTooShort() {
        return [`To ask for an example, Try saying a complete phrase, 
        like: Can you give me an example please?.`];
    }

    static get wrongAnswer() {
        return [`That's not quite right.`, `you're getting close. try again?`,
         `That's not it. try again, you can do it!`, `It's close, but not the right answer. I think you can do it. try again?`];
    }

    static get wrongAnswerFinal() {
        return [`That's not quite right. `, `that was close.`,
         `That's not it.`, `It's close, but not the right answer.`];
    }

    static randAskQuestion(name, questionNum, question) {
        const questionArr = [name + ', ' + 'question number ' + questionNum + ': ' + question,
                            'question ' + questionNum + ', ' + name + ': ' + question,
                            name + ', answer question ' + questionNum + ': ' + question];
        let ssmlResponse = Utils.randResponse(questionArr);
        return ssmlResponse;
    }
}

module.exports = PhraseConstants;