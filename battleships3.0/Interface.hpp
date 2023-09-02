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

#ifndef Interface_hpp
#define Interface_hpp

#include <stdio.h>
#include <iostream>
#include <string>
#include <array>

using namespace std;

enum Allow : unsigned char {Valid = 0, Invalid};

// Structs below here:
struct Output {
    string print;
    Allow Validity;
};






#endif /* Interface_hpp */
