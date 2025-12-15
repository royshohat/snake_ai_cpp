#include <iostream>
#include <SDL2/SDL.h>
#include <vector>

#include "ai.h"
#include "game.h"

SDL_Window* gWindow = nullptr;

void drawGrid(Game game, SDL_Renderer* renderer);
SDL_Renderer* init_window(int windowWidth, int windowHeight);


int main() {
    Game game(20, 14); 
    SDL_Renderer* renderer = init_window(game.getColumns()*50, game.getRows()*50);


    Ai brain({game.getRows() * game.getColumns(), 10, 10, 4});


    bool running = true;
    SDL_Event event;

    direction dir = up;

    while (running){
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                running = false;
            }
        }

        std::vector<float> input_layer;        
        for(auto& row : game.getCells()){
            for(auto& cell : row){
                input_layer.push_back(sigmoid(cell.type));
            }
        }
        std::vector<float> output_layer = brain.think(input_layer);

        
        // i will arbitrary say that the first output is up, second is down, third is right, and the fourfe will be left

        int biggest_index = 0;
        float biggest_output = output_layer[0];
        for(int i = 1; i!=output_layer.size(); ++i){
            if (output_layer[i] > biggest_output){
                biggest_index = i;
                biggest_output = output_layer[i];
            } 
        }

        switch(biggest_index){
            case(0):
                dir = up;
                break;
            case(1):
                dir = down;
                break;
            case(2):
                dir = right;
                break;
            case(3):
                dir = left;
                break;
            default:
                throw std::runtime_error("what is this move!!??");
        }

        drawGrid(game, renderer);
        if(!game.snake_update(dir)) running = false;

        SDL_Delay(190);

    }

    std::cout << "you failed!!" << std::endl;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();

   
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

    gWindow = SDL_CreateWindow("snake game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, 0);

    if(!gWindow){
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);


    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(gWindow);
        SDL_Quit();
    }
    return renderer;

}