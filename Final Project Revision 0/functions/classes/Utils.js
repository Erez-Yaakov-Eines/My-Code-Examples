
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

const Player = require('./Player');
const GameConstants = require('../constants/GameConstants');

/**
 * Utility class for "English Fun and Games"
 */
class Utils {

    /**
     * 
     * @param {PlainPlayerObjects[]} usersArray 
     * @param {string} name 
     * @returns {Boolean}
     * 
     *  true - if a player object
     *      in the input array has a user name
     *      matching input string 'name',
     *  false - o.w
     */
    static isUserNameTaken(usersArray, name) {
        let lowerName = name.toLowerCase();
        let userNameTaken = false;
        usersArray.forEach(user => {
            let player = new Player(user);
            if (player.userName.toLowerCase() === lowerName)
                userNameTaken = true;
        });
        return userNameTaken;
    }

    /**
     * 
     * @param {string} str
     * @returns {number} The count of single space delimited words in a string
     * 
     * Trims leading and trailing white spaces,
     * Replaces all white spaces with single space,
     * then, returns the number of space delimited words in the string.
     */
    static wordCount(str) { 
        let strTrimWhiteSpaces = str.replace(/\s+/g, ' ').trim();
        if (strTrimWhiteSpaces === "") return 0;
        else return strTrimWhiteSpaces.split(" ").length;
    }

    /**
     * 
     * @param {string} str 
     */
    static tooShort(str) {
        return (this.wordCount(str) < GameConstants.TOO_SHORT_LEN);
    }

    /**
     * 
     * @param {string} phrase the string to search through
     * @param {string} toFind the substring to find
     * 
     * @returns {boolean}
     * 
     * Return true if phrase has a sustring match with toFind
     * and false o.w
     */
    static phraseContains(phrase, toFind) {
        if (typeof phrase !== 'string' || typeof toFind !== 'string') throw new Error('Error - phraseContains recieved non string parameter.');
        return (phrase.indexOf(toFind) !== -1 ? true:false)
    }

    /**
     * 
     * @param {string[]} responseArray 
     * @returns {string} A randomly chosen string from the array
     */
    static randResponse(responseArray) {
        if (Array.isArray(responseArray) === false) throw new Error('Error - randResponse recieved non array object.');
        if (responseArray.length === 0) throw new Error('Error - randResponse recieved empty array.');
        let chosenElement = responseArray[this.getRandomInt(responseArray.length)];
        if (typeof chosenElement !== 'string') throw new Error('Error - randResponse recieved array with non-string element.');

        return chosenElement; 
    }

    /**
     * 
     * @param {number} max 
     * @returns {number} An integer between 1 and max, inclusive.
     */
    static getRandomInt(max) {
        if (typeof max !== 'number') throw new Error(`Error! - getRandomInt(max) recieved non numeric parameter`);
        if (isNaN(max)) throw new Error(`Error! - NaN parameter to getRandomInt(max)`);
        return Math.floor(Math.random() * Math.floor(max));
    }

    /**
     * 
     * @param {number} index 
     * @param {number} modulo
     * @returns {number} The cyclic increment of index modulo the modulo input
     */
    static modularIncrement(index, modulo) {
        if (typeof index !== 'number' || typeof modulo !== 'number')
            throw new Error('Error - modularIncrement recieved non numeric input');
        if (isNaN(index) || isNaN(modulo)) throw new Error('Error - modularIncrement recieved NaN input');
        return ((index + 1) % modulo);
    }

    /**
     * 
     * @param {string} recipient The intended recipient of the Email.
     * @param {string} text The message body.
     * @param {string} subject The message subject.
     * 
     * Sends an email using a hard coded google account.
     * NOTES -  1 - the  sender account has to enable less secure app access.
     *          2 - Hard coded account removed from shared source. repopulate with your own.
     * 
     */
    static sendMail(recipient, text, subject) {
        let nodemailer = require('nodemailer');

        let transporter = nodemailer.createTransport({
            service: 'gmail',
            auth: {
                user: '########@#####.#####',
                pass: '####################'
                }
            });

        let mailOptions = {
            from: 'mtesty467@gmail.com',
            to: recipient,
            subject: subject + this.getFullDate(),
            text: text
        };

        transporter.sendMail(mailOptions, (error, info) => {
            if (error) {
            console.log(error);
            } else {
            console.log('Email sent: ' + info.response);
            }
        });
    }

    /**
     * @returns {string} 
     * 
     * Returns date & time in DD-MM-YYYY HH:MM:SS format
     */
    static getFullDate() {
        let date_ob = new Date();

        // current date
        // adjust 0 before single digit date
        let day = ("0" + date_ob.getDate()).slice(-2);

        // current month
        let month = ("0" + (date_ob.getMonth() + 1)).slice(-2);

        // current year
        let year = date_ob.getFullYear();

        // current hours
        let hours = date_ob.getHours();

        // current minutes
        let minutes = date_ob.getMinutes();

        // current seconds
        let seconds = date_ob.getSeconds();

        return (day + "-" + month + "-" + year + " " + hours + ":" + minutes + ":" + seconds);
    }
}

module.exports = Utils;