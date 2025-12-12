#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#include "game.h"

void drawGrid(Game game, SDL_Renderer* renderer);
   
SDL_Renderer* init_window(int windowWidth, int windowHeight);

int main() {
    Game game(20, 20); 
    SDL_Renderer* renderer = init_window(game.getRows()*50, game.getColumns()*50);

    bool running = true;
    SDL_Event event;

    while (running){
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }


        drawGrid(game, renderer);

        SDL_Delay(50);

    }


    
    return 0;
}

void drawGrid(Game game, SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);


    std::vector<std::vector<Cell>>& cells = game.getCells();
    for (auto& row : cells){
        for(auto& cell : row){
            if(cell.type == empty) SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
            else if(cell.type == snake) SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); 
            else if(cell.type == apple) SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
            SDL_RenderFillRect(renderer, &cell.rect);
        }
    }
    SDL_RenderPresent(renderer);
}
SDL_Renderer* init_window(int windowWidth, int windowHeight){
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    }

    SDL_Window* window = SDL_CreateWindow("snake game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);

    if(!window){
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);


    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    return renderer;

}