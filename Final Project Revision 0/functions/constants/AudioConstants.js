
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

class AudioConstants {

    static get PAUSE() {
        return `<audio src="https://firebasestorage.googleapis.com/v0/b/learning-english-final-project.appspot.com/o/Ticking_Clock_10Sec.ogg?alt=media">
        <desc>Ticking Clock Sound.</desc>
         Audio resource for a Pause sound failed to load.
        </audio>`;
    }

    static get WRONG_ANSWER() {
        return `<audio src="https://firebasestorage.googleapis.com/v0/b/learning-english-final-project.appspot.com/o/Error.ogg?alt=media">
        <desc>Error sound.</desc>
         Audio resource for a error sound failed to load.
        </audio>`;
    }

    static get CORRECT() {
        return `<audio src="https://firebasestorage.googleapis.com/v0/b/learning-english-final-project.appspot.com/o/Usb-connection-sound-effect.ogg?alt=media">
        <desc>Correct sound.</desc>
         Audio resource for a correct sound failed to load.
        </audio>`;
    }

    static get HINT() {
        return `<audio src="https://firebasestorage.googleapis.com/v0/b/learning-english-final-project.appspot.com/o/Gleam-sound-effect.ogg?alt=media">
        <desc>Hint sound.</desc>
         Audio resource for a hint sound failed to load.
        </audio>`;
    }
    

}

module.exports = AudioConstants;