'use strict'

class ContextConstants {
    get GAME_CONTEXT_NAME() {
        return `game_context`;
    }

    get defaultGetNumberOfPlayersContext() {
        return {name: 'get_number_of_players', lifespan: '4'};
    }

    get VERIFY_PLAYERS_CONTEXT_NAME() {
        return `get_number_of_players_verify`;
    }

    get DEFAULT_LIFESPAN() {
        return 5;
    }

    get NICKNAMES_CONTEXT_NAME() {
        return `get_player_nicknames`;
    }
}

module.exports = ContextConstants;