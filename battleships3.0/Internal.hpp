//
//  Internal.hpp
//  battleships3.0
//
//  Created by Bennet Lum on 9/5/23.
//

#ifndef Internal_hpp
#define Internal_hpp

#include <stdio.h>
#include <string>
#include <array>

using namespace std;

enum Orientation : unsigned char {horizontal = 0, vertical};

enum Difficulty : unsigned char {easy, normal, impossible};

enum Ship_name : unsigned char {Carrier, Bship, Cruiser, Sub, PT};


//Function to print the board. Input is an array with 100 elements containing the board state.


string print_board(const array<char,100>board){
    string print;
    print += "   A B C D E F G H I J\n";
    for (int i=0; i<9; i++){
        print += " ";
        print += to_string(i+1);
        print += " ";
        for (int j=0; j<10; j++){
            print += board[j+10*i];
            print += " ";
        }
        print += "\n";
    }
    print += "10 ";
    for (int i=0; i<10; i++){
        print += board[90+i];
        print += " ";
    }
    print += "\n";
    return print;
}

/*

void print_board(const array<char,100>board){
    cout << "   A B C D E F G H I J\n";
    for (int i=0; i<9; i++){
        cout <<" " << i+1 << " ";
        for (int j=0; j<10; j++){
            cout << board[j+10*i] << " ";
        }
        cout <<"\n";
    }
    cout << "10 ";
    for (int i=0; i<10; i++){
        cout << board[90+i] << " ";
    }
    cout <<"\n";
}
 */

/*Bot place ship. Function to randomly select a squares for the bot to place a ship. Inputs: board[] is an array of length 100 and is the board that ships will be placed on to. ship_length is the size of the ship between 2-5. ship_char is the character used to denote the ship being placed.
*/
void bot_place_ship(array<char,100> &bottom_board, const Ship boat) {
    bool valid = 0; //is the potential position of the ship allowed
    int col = 0;
    int row = 0;
    Orientation orientation = horizontal; //determines vertical or horizontal orientation
    while (valid == 0){
        valid = 1;
        if (rand()%2 == 0) {
            orientation = horizontal;
        }
        else {
            orientation = vertical;
        }
        
        // if orientation is horizontal
        if (orientation == horizontal){
            col = rand()%(11-boat.length); //randomly choose column where ship will not go off edge
            row = rand()%10; //any row allowed
            //check that ship being placed doesnt overlap existing ship on board
            for (int i=0; i<boat.length; i++){
                if (bottom_board[row*10+col+i]!='.') {valid = 0;}
            }
        }
        // if orientation is vertical
        else {
            col = rand()%10; //any column allowed
            row = rand()%(11-boat.length); //limits rows to prevent ship going off edge
            //check that ship being placed doesnt overlap existing ship on board
            for (int i=0; i<boat.length; i++){
                if (bottom_board[(row+i)*10+col]!='.') {valid = 0;}
            }
        }
    }
    
    //change board characters to add ship to board
    if (orientation == horizontal){
        for (int i=0; i<boat.length; i++){
            bottom_board[row*10+col+i] = boat.ch_name;
        }
    }
    else {
        for (int i=0; i<boat.length; i++){
            bottom_board[(row+i)*10+col] = boat.ch_name;
        }
    }
}

//Simple bot guess. Randomly guess a valid square. Input board containing guessed opponent squares (top board). Output int between 0-99
int bot_guess(const array<char,100>bot_board){
    bool valid = 0;
    int bot_guess = 0; //guess between 0-99
    
    while (valid == 0) {
        bot_guess = rand()%100; //chooses random square of 100 total
        //allows guess if the square hasnt been guessed before
        if (bot_board[bot_guess]=='.') {
            valid = 1;
        }
    }
    return bot_guess;
}

//Advanced bot guess. Guesses along line of hits or randomly around a single hit. If none then guesses randomly. Input board with previous bot guesses. Output int between 0-99.
int bot_guess_2(const array<char,100>bot_board) {
    bool pot_sunk = 0; //potentially sunk
    int guess = 0;
    int change = 1;
    //int random = 1;
    
    while (guess<100) {
        while (bot_board[guess]!='X' && guess<100) {
            guess++;
        }
        if (guess%10!=0 && bot_board[guess-change]=='X') {
            while ((guess-change)%10!=0 && bot_board[guess-change]=='X') {
                change++;
            }
            if (bot_board[guess-change] == '.') {
                guess = guess-change;
                return guess;
            }
            else {
                change = 1;
                pot_sunk = 1;
            }
        }
        if (guess%10!=9 && bot_board[guess+change]=='X') {
            while ((guess+change)%10!=9 && bot_board[guess+change]=='X') {
                change++;
            }
            if (bot_board[guess+change] == '.') {
                guess = guess+change;
                return guess;
            }
            else {
                change = 1;
                pot_sunk = 1;
            }
        }
        if (guess>9 && bot_board[guess-change*10]=='X') {
            while ((guess-change*10)>9 && bot_board[guess-change*10]=='X') {
                change++;
            }
            if (bot_board[guess-change*10] == '.') {
                guess = guess-change*10;
                return guess;
            }
            else {
                change = 1;
                pot_sunk = 1;
            }
        }
        if (guess<90 && bot_board[guess+change*10]=='X') {
            while ((guess+change*10)>9 && bot_board[guess+change*10]=='X') {
                change++;
            }
            if (bot_board[guess+change*10] == '.') {
                guess = guess+change*10;
                return guess;
            }
            else {
                change = 1;
                pot_sunk = 1;
            }
        }
        
        if (pot_sunk==0 && guess<100){
            if(rand()%2==0) {
                if (guess%10!=9 && bot_board[guess+1]=='.') {
                    guess++;
                    return guess;
                }
                else if (guess%10!=0 && bot_board[guess-1]=='.') {
                    guess--;
                    return guess;
                }
                else if (guess>9 && bot_board[guess-10]=='.') {
                    guess=guess-10;
                    return guess;
                }
                else if (guess<90 && bot_board[guess+10]=='.') {
                    guess=guess+10;
                    return guess;
                }
            }
            else {
                if (guess>9 && bot_board[guess-10]=='.') {
                    guess=guess-10;
                    return guess;
                }
                else if (guess<90 && bot_board[guess+10]=='.') {
                    guess=guess+10;
                    return guess;
                }
                else if (guess%10!=9 && bot_board[guess+1]=='.') {
                    guess++;
                    return guess;
                }
                else if (guess%10!=0 && bot_board[guess-1]=='.') {
                    guess--;
                    return guess;
                }
            }
        }
        pot_sunk=0;
        guess++;
    }
    guess = bot_guess(bot_board);
    return guess;
}

//Cheating bot guess. Bot looks at player board and returns a guess where there is an unsunk ship.
int bot_guess_3(const array<char,100>player_board) {
    bool valid = 0;
    int bot_guess = 0;
    
    while (valid == 0) {
        if (player_board[bot_guess]!='.' && player_board[bot_guess]!='O' && player_board[bot_guess]!='X') {
            bot_guess++;
        }
        else {
            valid = 1;
        }
    }
    return bot_guess;
}

//Validates player guess. Input board with guesses player made (top board)
int valid_guess(const array<char,100>board){
    bool valid = 0;
    string player_guess; //player input. letter A-J (not case sensitive) and number 1-10
    int guess_number = 0; //guess between 0-99
   
    while (valid == 0){
        cout << "Guess: ";
        cin >> player_guess; //player input
        //if guess contains number 10, change 10 to ':' since it is ASCII character after 9. Allows subtraction later.
        if (player_guess.length()==3 && player_guess[1]=='1' && player_guess[2]=='0') {
            player_guess.pop_back();
            player_guess[1]=':';
        }
        //Checks that a guess is 2 characters A-J and 1-10. If does not meet criteria, asks for new guess
        if (player_guess.length()!=2 || isalpha(player_guess[0])==0 || (isdigit(player_guess[1])==0 && player_guess[1]!=':') || int(toupper(player_guess[0]))>74 || player_guess[1]=='0'){
            cout << "Invalid coordinates. Choose (A-J) and (1-10). Ex: D4" << endl;
        }
        //If not invalid, then changes guess into a number value between 0-99
        else {
            guess_number = (int(toupper(player_guess[0]))-65)+(int(player_guess[1])-49)*10;
            //Checks that the guessed square hasnt already been guessed
            if (board[guess_number]=='.') {
                valid = 1; //guess is valid
            }
            //If already guessed, will ask for new guess
            else {
                cout << "Coordinate alreay guessed. Choose another coordinate." << endl;
            }
        }
    }
    return guess_number; //returns a valid guess
}

//updates guess board (top) and ship board (bottom). Inputs: update board is board with guesses (top). reference board is opponent board with ships (bottom), guess is an int between 0-99
string update_board(array<char,100> &top_board, array<char,100> &bottom_board, const int guess, array<Ship,5> &ship_table) {
    string print;
    
    if (bottom_board[guess]=='.') {
        print += "Miss.\n"; //display miss
        top_board[guess] = 'O'; //change board (top) to miss
        bottom_board[guess] = 'O'; //change opponent board (bottom) to miss
    }
    else {
        print += "Hit!\n"; //display hit
        for (int i=0; i<ship_table.size(); i++) {
            if (ship_table[i].ch_name == bottom_board[guess]) {
                ship_table[i].hits++;
                if (ship_table[i].hits==ship_table[i].length) {
                    print += ship_table[i].long_name;
                    print += " sunk!\n";
                    ship_table[i].hits++;
                }
            }
        }
        top_board[guess] = 'X'; //(change board (top) to hit
        bottom_board[guess] = 'X'; //change opponent board (bottom) to hit
    }
    return print;
}

//player place ship. Checks and places a ship in a valid location based on player input. Ships will orient themselves down or to the right of coordinate indicated by player. Inputs: board containing ships (bottom). ship_length is length of ship being placed. ship_char is the character used to represent the ship type.
void player_place_ship(array<char,100> &board, const Ship boat){
    bool valid = 0;
    int col = 0;
    int row = 0;
    Orientation orientation = horizontal; //horizontal or vertical
    string direction; //takes player input
    string coords; //takes player input
    
    cout << "Place " << boat.long_name << ". Length: " << boat.length << ". Symbol: " << boat.ch_name << "\n";
    
    while (valid == 0) {
        cout << "Choose orientation (h/v): ";
        cin >> direction;
        //sets orientation to be first letter of player input. done to prevent issues if player inputs more than one character.
        if (direction[0] == 'h' || direction[0] == 'H') {
            orientation = horizontal;
            valid = 1;
        }
        else if (direction[0] == 'v' || direction[0] == 'V') {
            orientation = vertical;
            valid = 1;
        }
        else {
            cout << "Invalid orientation." << endl; //reset loop asking for new input if invalid
        }
    }
    
    valid = 0; //reset to invalid to check for coordinates
    
    //same logic as valid_guess function combined with place_ship function
    while (valid == 0){
        valid = 1;
        cout << "Choose coordinates. Ship will face down/right: ";
        cin >> coords;
        if (coords.length()==3 && coords[1]=='1' && coords[2]=='0') {
            coords.pop_back();
            coords[1]=':';
        }
        if (coords.length()!=2  || isalpha(coords[0])==0 || (isdigit(coords[1])==0 && coords[1]!=':') || int(toupper(coords[0]))>74 || coords[1]=='0'){
            cout << "Invalid coordinates. Choose (A-J) and (1-10). Ex: D4" << endl;
            valid = 0;
        }
        else {
            col = toupper(int(coords[0]))-65;
            row = int(coords[1])-49;
            
            if (orientation == horizontal){
                if (col > 10-boat.length) {
                    cout << "Invalid coordinates. Ship must fit on the board." << endl;
                    valid = 0;
                }
                else {
                    for (int i=0; i<boat.length; i++){
                        if (board[row*10+col+i]!='.') {valid = 0;}
                    }
                    if (valid == 0) {
                        cout << "Invalid coordinates. Ships cannot overlap." << endl;
                    }
                }
            }
            else {
                if (row > 10-boat.length) {
                    cout << "Invalid coordinates. Ship must fit on the board." << endl;
                    valid = 0;
                }
                else {
                    for (int i=0; i<boat.length; i++){
                        if (board[(row+i)*10+col]!='.') {valid = 0;}
                    }
                    if (valid == 0) {
                        cout << "Invalid coordinates. Ships cannot overlap." << endl;
                    }
                }
            }
        }
    }
    //places ships on board
    if (orientation == horizontal){
        for (int i=0; i<boat.length; i++){
            board[row*10+col+i] = boat.ch_name;
        }
    }
    else {
        for (int i=0; i<boat.length; i++){
            board[(row+i)*10+col] = boat.ch_name;
        }
    }
}

#endif /* Internal_hpp */
