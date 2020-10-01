'use strict';

const ParameterConst = require(`../constants/ParameterConstants.js`);
const ParameterConstants = new ParameterConst();

class Game {

    constructor(numberOfPlayers, playerObjects) {

    }

    get currentNickname() {
      return this.nicknames[this.currentPlayerIndex];
    }
}

module.exports = Game;