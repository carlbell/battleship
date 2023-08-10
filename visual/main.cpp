#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <iostream>

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return 1;
  }

  SDL_Window * window = SDL_CreateWindow("Battleship Text",
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    680, 480,
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

  bool quit = false;
  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent( & event)) {
      if (event.type == SDL_QUIT) {
       quit = true;
      }
    }
    
    //clear the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("res/COMIC.TTF", 40);
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "We are playing battleship", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
    SDL_Rect textRect = { (680 - textWidth) / 2, (480 - textHeight) / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    
    SDL_RenderPresent(renderer);
  }

  // Clean up and exit
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
    
  return 0;
}
