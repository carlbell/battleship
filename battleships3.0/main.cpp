//
//  main.cpp
//
//  Created by Bennet Lum on 8/5/23.
//
//  Battleship game! User can place ships and play against a bot. Bot has 3 difficulty levels.

//consider fixing array for loops that add '.' with .fill('.') function
//change c style array to std array
//remove endls as endl clears buffer
//add const to vars and functions


//allows time delays
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <string>
#include <array>

using namespace std;

enum Orientation : unsigned char {horizontal = 0, vertical};

enum Difficulty : unsigned char {easy = 0, normal, impossible};

enum Ship_name : unsigned char {Carrier, Bship, Cruiser, Sub, PT};


//ship structure to store ship type info
struct Ship {
    string long_name;
    char ch_name;
    int length;
    int hits;
    bool sunk;
};


//Function to print the board. Input is an array with 100 elements containing the board state.
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

//Validates user guess. Input board with guesses user made (top board)
int valid_guess(const array<char,100>board){
    bool valid = 0;
    string player_guess; //player input. letter A-J (not case sensitive) and number 1-10
    int guess_number = 0; //guess between 0-99
   
    while (valid == 0){
        cout << "Guess: ";
        cin >> player_guess; //user input
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
void update_board(array<char,100> &top_board, array<char,100> &bottom_board, const int guess) {
    
    if (bottom_board[guess]=='.') {
        cout << "Miss." << endl; //display miss
        top_board[guess] = 'O'; //change board (top) to miss
        bottom_board[guess] = 'O'; //change opponent board (bottom) to miss
    }
    else {
        cout << "Hit!" << endl; //display hit
        top_board[guess] = 'X'; //(change board (top) to hit
        bottom_board[guess] = 'X'; //change opponent board (bottom) to hit
        
    }
}

//User place ship. Checks and places a ship in a valid location based on user input. Ships will orient themselves down or to the right of coordinate indicated by user. Inputs: board containing ships (bottom). ship_length is length of ship being placed. ship_char is the character used to represent the ship type.
void user_place_ship(array<char,100> &board, const Ship boat){
    bool valid = 0;
    int col = 0;
    int row = 0;
    Orientation orientation = horizontal; //horizontal or vertical
    string direction; //takes user input
    string coords; //takes user input
    
    cout << "Place " << boat.long_name << ". Length: " << boat.length << ". Symbol: " << boat.ch_name << "\n";
    
    while (valid == 0) {
        cout << "Choose orientation (h/v): ";
        cin >> direction;
        //sets orientation to be first letter of user input. done to prevent issues if user inputs more than one character.
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

//consider making a function to validate coordinates are A-J and 1-10 format. will reduce some repetition

int main() {
    
    int play = 0; //does user want to keep playing
    string input;
    bool valid = 0;
    
    //ask user if ready to play
    cout << "Ready to Play Sinky-Ships?\n" << "1. Yes\n" << "2. No" << endl;
    cin >> play;
    
    while (play == 1){
        
        srand((unsigned)time(NULL)); //set random seed based on time
        //symbols used:
        //hit 'X', miss 'O', unknown/ '.'
        //ships: carrier 'A', battleship 'B', sub 'S', cuiser 'C', pt boat, 'P'
        
        //variables for game
        int player_hits = 0; //number of hits player made
        int computer_hits = 0; //number of hits bot made
        int guess = 0; //coordinate being guessed
        Difficulty mode = easy;
       
        //ask user for difficulty level
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
        
        
        /*
        bot_place_ship(board_2_bottom, ship_type[Carrier]);
        bot_place_ship(board_2_bottom, ship_type[Bship]);
        bot_place_ship(board_2_bottom, ship_type[Sub]);
        bot_place_ship(board_2_bottom, ship_type[Cruiser]);
        bot_place_ship(board_2_bottom, ship_type[PT]);
        */
         
        /*
        //randomly set player ship locations
         bot_place_ship(board_1_bottom, ship_type[Carrier]);
         bot_place_ship(board_1_bottom, ship_type[Bship]);
         bot_place_ship(board_1_bottom, ship_type[Sub]);
         bot_place_ship(board_1_bottom, ship_type[Cruiser]);
         bot_place_ship(board_1_bottom, ship_type[PT]);
        */
        
        //allow player to set player ship locations
        print_board(board_1_bottom);
        cout << "Place your ships." << endl;
        for (int i=0; i<num_ships; i++) {
            user_place_ship(board_1_bottom, ship_type[ships_play[i]]);
            print_board(board_1_bottom);
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
        while (player_hits<max_hits && computer_hits<max_hits){
            //display top and bottom board to player
            print_board(board_1_top);
            cout << "-----------------------\n";
            print_board(board_1_bottom);
            //player takes turn and makes guess
            cout << "Your turn. ";
            guess = valid_guess(board_1_top); //input player guessed square
            update_board(board_1_top, board_2_bottom, guess); //update board based on guess
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
                update_board(board_2_top, board_1_bottom, guess); //update boards
                //add hit if bot hit
                if (board_2_top[guess]=='X'){
                    computer_hits++;
                }
                sleep(1); //wait before displaying new board state
            }
            
        }
        
        //display player boards at end of game
        print_board(board_1_top);
        cout << "-----------------------\n";
        print_board(board_1_bottom);
        
        //display win/loss message
        if (player_hits==max_hits) {
            cout << "\nYOU WIN!\n\n";
        }
        else {
            cout << "Enemy Ships:" << endl;
            print_board(board_2_bottom); //show location of bot ships if player lost
            cout << "\nYou Lost.\n" << endl;
            
        }
        
        //ask user to play another round
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
