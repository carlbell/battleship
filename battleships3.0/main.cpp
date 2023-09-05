//
//  main.cpp
//
//  Created by Bennet Lum on 8/5/23.
//
//  Battleship game! player can place ships and play against a bot. Bot has 3 difficulty levels.

/* available functions:
 game_set_difficulty -ip
 game_start -ip
 game_player_turn -ip
 game_turn -complete
 game_end -ip
 */

#include <iostream>
#include <string>
#include <array>
#include "GameState.hpp"
#include "Interface.hpp"

using namespace std;


//consider making a function to validate coordinates are A-J and 1-10 format. will reduce some repetition

int main() {
    Game game_state_1;
    
    cout << set_difficulty_u(game_state_1).print;    //test default
    cout << set_difficulty_u (game_state_1, 4).print; //test invalid
    cout << set_difficulty_u(game_state_1, 1).print;  //test valid
    
    bot_place_ships_u(game_state_1);    //place bot ships
    player_place_ships_u(game_state_1); //place player ships
    
    //17 round game
    /*
     for (int i=0; i<18; i++) {
     cout << player_guess_u(game_state_1);
     cout << bot_guess_u(game_state_1);
     }
     */
    while (end_game_u(game_state_1).check == Invalid) {
        cout << player_guess_u(game_state_1);
        cout << bot_guess_u(game_state_1);
        cout <<end_game_u(game_state_1).print;
    }
    
    return 0;
}
    
    /*
    int play = 0; //does player want to keep playing
    string input;
    bool valid = 0;
    
    //ask player if ready to play
    cout << "Ready to Play Sinky-Ships?\n" << "1. Yes\n" << "2. No" << endl;
    cin >> play;
    
    Game game_state_1;
    
    
    bot_place_ships_u(game_state_1);
    player_place_ships_u(game_state_1);
    for (int i=0; i<18; i++) {
        cout << player_guess_u(game_state_1);
        cout << bot_guess_u(game_state_1);
    }
    
    
    
    while (play == 1){
        
        srand((unsigned)time(NULL)); //set random seed based on time
        //symbols used:
        //hit 'X', miss 'O', unknown/ '.'
        //ships: carrier 'A', battleship 'B', sub 'S', cuiser 'C', pt boat, 'P'
        
        //variables for game
        int player_hits = 0; //number of hits player made
        int bot_hits = 0; //number of hits bot made
        int guess = 0; //coordinate being guessed
        Difficulty mode = easy;
       
        //ask player for difficulty level
        while (valid == 0) {
            cout << "Select difficulty (1. easy, 2. normal, 3. impossible): ";
            cin >> input;
            if (input[0] == '1') {
                mode = easy;
                valid = 1;
            }
            else if (input[0] == '2') {
                mode = normal;
                valid = 1;
            }
            else if (input[0] == '3') {
                mode = impossible;
                valid = 1;
            }
            else {
                cout << "Invalid difficulty." << endl;
            }
        }
            
        
        //define ships here
        int num_ships = 5; //number of ships in play
        //make ship object with values of long name, short name, length, hits taken, methods of .issunk() if hits taken=length return 1 else return 0
        array<Ship_name, 5> ships_play = {Carrier, Bship, Sub, Cruiser, PT}; //ships in play
        
        //array of ship structures: long_name, ch_name, length, hits, sunk
        array<Ship,5> ship_type = {{
            {"Aircraft Carrier" , 'A', 5, 0, 0},
            {"Battleship"       , 'B', 4, 0, 0},
            {"Cruiser"          , 'C', 3, 0, 0},
            {"Submarine"        , 'S', 3, 0, 0},
            {"Patrol Boat"      , 'P', 2, 0, 0}
        }};
        
        
        //boards
        //char board_1_top[100];  //player top board (contains guesses)
        array<char, 100> board_1_top;
        board_1_top.fill('.'); //initialize all to unknown '.'
        
        array<char, 100> board_1_bottom; //player bottom board (contains ships)
        board_1_bottom.fill('.');
        
        array<char, 100> board_2_top;  //bot top board (contains guesses)
        board_2_top.fill('.');
        
        array<char, 100> board_2_bottom;   //bot bottom board (contains ships)
        board_2_bottom.fill('.');
        
        //set bot ship locations. 5 ships set randomly
        for (int i=0; i<num_ships; i++) {
            bot_place_ship(board_2_bottom, ship_type[ships_play[i]]);
        }
        
        cout << print_board(board_1_bottom);

        
        /*
        //randomly set player ship locations
         bot_place_ship(board_1_bottom, ship_type[Carrier]);
         bot_place_ship(board_1_bottom, ship_type[Bship]);
         bot_place_ship(board_1_bottom, ship_type[Sub]);
         bot_place_ship(board_1_bottom, ship_type[Cruiser]);
         bot_place_ship(board_1_bottom, ship_type[PT]);
        */
        
    /*
     //allow player to set player ship locations
     cout << print_board(board_1_bottom);
     cout << "Place your ships." << endl;
     for (int i=0; i<num_ships; i++) {
     player_place_ship(board_1_bottom, ship_type[ships_play[i]]);
     cout << print_board(board_1_bottom);
     }
     
     //calculate max number of hits for end game purposes
     int max_hits = 0; //max number of hits
     //sum of lengths of ships in play
     for (int i=0; i<num_ships; i++) {
     max_hits = max_hits+ship_type[ships_play[i]].length;
     }
     
     //Begin game
     cout << "Begin!\n\n";
     
     //Game mechanism
     //play continues while max number of hits not achieved by either player or bot
     while (player_hits<max_hits && bot_hits<max_hits){
     //display top and bottom board to player
     cout << print_board(board_1_top);
     cout << "-----------------------\n";
     cout << print_board(board_1_bottom);
     //player takes turn and makes guess
     
     //see bot board
     //cout << print_board(board_2_bottom);
     
     cout << "Your turn. ";
     guess = valid_guess(board_1_top); //input player guessed square
     cout << update_board(board_1_top, board_2_bottom, guess, ship_type); //update board based on guess
     //add hit if player hit
     if (board_1_top[guess]=='X'){
     player_hits++;
     }
     
     //Bot turn
     //bot only takes turn if has any remaining ships
     if (player_hits<max_hits) {
     cout << "Computer turn: ";
     switch (mode) {
     case easy:
     guess = bot_guess(board_2_top);
     break;
     case normal:
     guess = bot_guess_2(board_2_top);
     break;
     case impossible:
     if (player_hits>13 && rand()%2==0) {
     guess = bot_guess_3(board_1_bottom);
     }
     else {
     guess = bot_guess_2(board_2_top);
     }
     break;
     }
     cout << guess << endl;
     cout << update_board(board_2_top, board_1_bottom, guess, ship_type); //update boards
     //add hit if bot hit
     if (board_2_top[guess]=='X'){
     bot_hits++;
     }
     }
     
     }
     
     //display player boards at end of game
     cout << print_board(board_1_top);
     cout << "-----------------------\n";
     cout << print_board(board_1_bottom);
     
     //display win/loss message
     if (player_hits==max_hits) {
     cout << "\nYOU WIN!\n\n";
     }
     else {
     cout << "Enemy Ships:" << endl;
     cout << print_board(board_2_bottom); //show location of bot ships if player lost
     cout << "\nYou Lost.\n" << endl;
     
     }
     
     //ask player to play another round
     cout << "Another Round?\n" << "1. Yes\n" << "2. No\n";
     cin >> play;
     
     valid = 0; //reset valid difficulty
     //reset ship healths
     for (int i=0; i<num_ships; i++) {
     ship_type[ships_play[i]].hits = 0;
     ship_type[ships_play[i]].sunk = 0;
     }
     }
     cout << "OK. Goodbye!" << endl; //goodbye message
     
     return 0;
     }
     
     */
