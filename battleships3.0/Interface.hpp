// Functions:
// bot_place_ships_u
// Parameters:
//   GameState
// Returns:
//   string
//

// player_place_ships_u
// Parameters:
//   GameState
//  Returns:
//   string

// player_guess_u
// Parameters:
//   GameState
// Returns:
//   string

// bot_guess_u
// Parameters:
//   GameState
// Returns:
//   string

// set_difficulty_u
// Parameters:
//   GameState, difficulty(optional)
// Returns:
//   Output

// end_game_u
// Parameters:
//   GameState
// Returns:
//   Output
//   (returns valid if game is over)



#ifndef Interface_hpp
#define Interface_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <array>
#include "Internal.hpp"

using namespace std;

enum Allow : unsigned char {Valid = 0, Invalid};

// Structs below here:
struct Output {
    string print;
    Allow check;
};


//for web use
Output set_difficulty_u (Game &game_state, int difficulty = 0) {
    Output answer;
    
    switch (difficulty) {
        case 0:
            answer.print += "Select difficulty (1. easy, 2. normal, 3. impossible): ";
            answer.check = Invalid;
            break;
        case 1:
            game_state.difficuly = 1;
            answer.print += "Easy Mode\n";
            answer.check = Valid;
            break;
        case 2:
            game_state.difficuly = 2;
            answer.print += "Normal Mode\n";
            answer.check = Valid;
            break;
        case 3:
            game_state.difficuly = 3;
            answer.print += "Impossible Mode\n";
            answer.check = Valid;
            break;
        default:
            answer.print += "Invalid difficulty.\nSelect difficulty (1. easy, 2. normal, 3. impossible): ";
            answer.check = Invalid;
            break;
    }

    return answer;
}

//for web use
void bot_place_ships_u(Game &game_state) {
    for (int i=0; i<5; i++) {
        bot_place_ship(game_state.board_2_bottom, game_state.ship_type[i]);
    }
}

//for web use
//update when cin works
void player_place_ships_u(Game &game_state) {
    for (int i=0; i<5; i++) {
        bot_place_ship(game_state.board_1_bottom, game_state.ship_type[i]);
    }
}

string bot_guess_u(Game &game_state) {
    string print;
    int guess = 0;
    //Bot turn
    //bot only takes turn if has any remaining ships
    if (game_state.player_hits<game_state.max_hits) {
        print += "Computer turn: ";
        switch (game_state.difficuly) {
            case 1:
                guess = bot_guess(game_state.board_2_top);
                break;
            case 2:
                guess = bot_guess_2(game_state.board_2_top);
                break;
            case 3:
                if (game_state.player_hits>13 && rand()%2==0) {
                    guess = bot_guess_3(game_state.board_1_bottom);
                }
                else {
                    guess = bot_guess_2(game_state.board_2_top);
                }
                break;
        }
        print += to_string(guess); //adds ascii number to letter to string
        print += "\n";
        print += update_board(game_state.board_2_top, game_state.board_1_bottom, guess, game_state.ship_type); //update boards
        //add hit if bot hit
        if (game_state.board_2_top[guess]=='X'){
            game_state.bot_hits++;
        }
    }
    return print;
}

string player_guess_u(Game &game_state) {
    string print;
    int guess = 0;
    
    //display top and bottom board to player
    print += print_board(game_state.board_1_top);
    print += "-----------------------\n";
    print += print_board(game_state.board_1_bottom);
    //player takes turn and makes guess

    print += "Your turn. ";
    
    //temp bot guess for player
    guess = bot_guess_2(game_state.board_1_top);

    print += to_string(guess);
    /* implement player guessing system when cin works
    guess = valid_guess(game_state.board_1_top); //input player guessed square
     */
     
    print += update_board(game_state.board_1_top, game_state.board_2_bottom, guess, game_state.ship_type); //update board based on guess
    
    //add hit if player hit
    if (game_state.board_1_top[guess]=='X'){
        game_state.player_hits++;
    }
    
    return print;
}

Output end_game_u(Game &game_state) {
    Output answer;
    
    if (game_state.player_hits < game_state.max_hits && game_state.bot_hits < game_state.max_hits) {
        answer.print += "Next Turn:\n";
        answer.check = Invalid;
    }
    else {
        //display player boards at end of game
        answer.print += print_board(game_state.board_1_top);
        answer.print += "-----------------------\n";
        answer.print += print_board(game_state.board_1_bottom);
        
        answer.check = Valid;
        
        //display win/loss message
        if (game_state.player_hits==game_state.max_hits) {
            answer.print += "\nYOU WIN!\n\n";
        }
        else {
            answer.print += "Enemy Ships:\n";
            answer.print += print_board(game_state.board_2_bottom); //show location of bot ships if player lost
            answer.print += "\nYou Lost.\n\n";
            
        }
    }
    return answer;
}





#endif /* Interface_hpp */
