#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../battleships3.0/GameState.hpp"
#include "../battleships3.0/Interface.hpp"
#include "visualarrangementtools.hpp"

#include <iostream>
const int UNINITIALIZED = -1;
const int FONT_SIZE = 20;

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window * window = SDL_CreateWindow("Battleship Text",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    680, 960,
    0);

  if (!window) {
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return 1;
  }

  SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return 1;
  }

  Game game_state;
  int counter = 0;

  bool quit = false;
  int x = UNINITIALIZED;
  int y = UNINITIALIZED;
  string text = "Loading...";
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent( & event)) {
      if (event.type == SDL_QUIT) {
       quit = true;
      }
      // if click: send to Game process click location
      
      if (event.type == SDL_MOUSEBUTTONDOWN) {
        SDL_GetMouseState(&x, &y);
        std::cout << "click!" << std::endl;
        switch (counter) {
          case 0:
            text = set_difficulty_u(game_state).print;
            counter++;
            break;
          case 1:
            text = set_difficulty_u(game_state, 2).print;
            counter++;
            break;
          case 2:
            text = set_difficulty_u(game_state, 3).print;
            counter++;
            break;
          case 3:
            bot_place_ships_u(game_state);
            text = "Bot placed ships";
            counter++;
            break;
          case 4:
            player_place_ships_u(game_state);
            text = "Player placed ships";
            counter++;
            break;
          case 5:
            text = player_guess_u(game_state);
            counter++;
            break;
          case 6:
            text = bot_guess_u(game_state);
            counter++;
            break;
          case 7:
            Output result = end_game_u(game_state);
            text = result.print;
            if (result.check == Invalid) {
              counter = 5;
            }
            else {
              counter = 0;
            }
            break;
        }
      }
    }
    
    //clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("res/COMIC.TTF", FONT_SIZE);
    SDL_Color textColor = { 255, 255, 255, 255 };
    int max_lines = window_height / TTF_FontHeight(font);
    const char* to_print = text.c_str();
    char* lines[max_lines];
    int numLines;
    splitTextIntoLines(to_print, window_width, font, max_lines, lines, &numLines);

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, to_print, textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect rect;
    rect.x = 0;    // X-coordinate of the top-left corner
    rect.y = 0;    // Y-coordinate of the top-left corner
    rect.w = window_width;         // Width of the rectangle
    rect.h = window_height;       // Height of the rectangle
    SDL_Rect textRect;
    
    for (int i = 0; i < numLines; i++) {
      SDL_Surface* textSurface = TTF_RenderText_Solid(font, lines[i], textColor);
      SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

      // Render the texture in your SDL_Rect
      
      textRect.x = rect.x;
      textRect.y = rect.y + (i * TTF_FontHeight(font)); // Adjust for line spacing
      textRect.w = textSurface->w;
      textRect.h = textSurface->h;

      SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

      // Free resources
      SDL_FreeSurface(textSurface);
      SDL_DestroyTexture(textTexture);

      // Free the allocated line memory
      free(lines[i]);
    }
    TTF_CloseFont(font);
    SDL_RenderPresent(renderer);
  }

  // Clean up and exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
    
  return 0;
}
