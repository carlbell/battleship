//
//  GameState.hpp
//  battleships3.0
//
//  Created by Bennet Lum on 8/21/23.
//

#ifndef GameState_hpp
#define GameState_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <array>

using namespace std;

struct Ship {
    string long_name;
    char ch_name;
    int length;
    int hits;
    bool sunk;
};

class Game {
    public:
        /*
        //record trackers
         int wins
         int win percentage
         int losses
         int total games
         int tot hits
         int tot guess
         int own ships sunk
         int opp ships sunk
         */
    
    
        //game trackers
        int difficuly;  //difficulty level
        int player_hits = 0; //number of hits player made
        int computer_hits = 0; //number of hits bot made
        int max_hits = 17;
    
        //table of ships
        array<Ship,5> ship_type = {{
            {"Aircraft Carrier" , 'A', 5, 0, 0},
            {"Battleship"       , 'B', 4, 0, 0},
            {"Cruiser"          , 'C', 3, 0, 0},
            {"Submarine"        , 'S', 3, 0, 0},
            {"Patrol Boat"      , 'P', 2, 0, 0}
        }};
        
        //boards
        array<char, 100> board_1_top;       //player top board (contains guesses)
        array<char, 100> board_1_bottom;    //player bottom board (contains ships)
        array<char, 100> board_2_top;       //bot top board (contains guesses)
        array<char, 100> board_2_bottom;    //bot bottom board (contains ships)
           
        //game constructor
        Game () {
            board_1_top.fill('.'); //initialize all to unknown '.'
            board_1_bottom.fill('.');
            board_2_top.fill('.');
            board_2_bottom.fill('.');
            difficuly = 3;
        }
        
    
        //consider making more setters and getters
        int get_difficulty() {return difficuly;}
        void set_difficulty(int diff) {difficuly = diff;}
    
    
};


#endif /* GameState_hpp */
