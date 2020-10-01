class Player {
    /**
     * 
     * @param {String} nickname chosen student nickname.
     * @param {number} id student player number for conversational turns.
     * @param {number} correct number of correct answers for the current game.
     * @param {number} incorrect number of incorrect answers for the current game.
     * @param {number} hints total number of hints.
     */
    constructor(nickname, id, correct, incorrect, hints) {
        //console.log("constructor called");
        this.nickname = nickname;
        this.id = id;
        this.correct = correct;
        this.incorrect = incorrect;
        this.hints = hints;

        this.raiseCorrect = function() {
            this.correct++;
        }
 
    }
    toString() {
        return (`Nickname: ` + this.nickname + `.\nId: ` + this.id + `.\nCorrect answers: ` + this.correct + `.\nIncorrect answers: ` + this.incoorect + `.\nHints recieved: ` + this.hints);
    }



    incIncorrect() {
        this.incorrect++;
    }

    usedHint() {
        this.hints++;
    }

    get nickname() {
        console.log("nickname getter called");
        return this.m_nickname;
    }

    get id() {
        return this.m_id;
    }

    get correct() {
        return this.m_correct;
    }

    get incoorect() {
        return this.m_incorrect;
    }

    get hints() {
        return this.m_hints;
    }

    set nickname(nickname) {
        if (nickname == undefined) throw new Error(`Error! - Class Player property nickname setter recieved undefined parameter`);
        if (nickname.length == 0) throw new Error(`Error! - Class Player property nickname setter recieved empty string nickname`);
        this.m_nickname = nickname;
    }

    set id(id) {
        if (id < 0) throw new Error(`Error! - Class Player property id setter recieved id < 0`);
        this.m_id = id;
    }

    set correct(correct) {
        this.m_correct = correct;
    }
    
    set incorrect(incorrect) {
        this.m_incorrect = incorrect;
    }

    set hints(hints) {
        this.m_hints = hints;
    }

}

module.exports = Player;