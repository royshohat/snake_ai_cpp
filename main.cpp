#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

#include "ai.h"
#include "game.h"

#define GAME_ROWS 14
#define GAME_COLUMNS 20
#define INPUTS 14
#define GENRENTIONS 100

SDL_Window* gWindow = nullptr;
SDL_Renderer* renderer = nullptr;

void drawGrid(Game game, SDL_Renderer* renderer);
float ai_scorer(Ai brain, float generation, bool render);
SDL_Renderer* init_window(int windowWidth, int windowHeight);


struct brain_score{
    Ai brain;
    float score;
    brain_score(Ai b, float s) : brain(std::move(b)), score(s) {}
};

int main() {

    renderer = init_window(GAME_COLUMNS*50, GAME_ROWS*50);

    std::vector<brain_score> brains;

    std::cout << "Initializing 50 random brains..." << std::endl;
    for(int i=0; i!=50; ++i){
        Ai brain({INPUTS, 10, 10, 4}, 0.5);
        brains.emplace_back(std::move(brain), ai_scorer(brain, 0, false));
    }

    int generation = 0;

    while(true){
        std::sort(brains.begin(), brains.end(), [](const brain_score& a, const brain_score& b) {
            return a.score > b.score;
        });

        std::cout << "Generation " << generation << " | Best score: "
                  << brains[0].score << std::endl;


        for(int i=0; i!=5; ++i){
            for(int j=5+9*i; j!=5+9*(i+1); ++j){
                brains[j] = brain_score{Ai(brains[i].brain), 0};
                brains[j].brain.mutate();
            }
        }
        
        bool render = generation%100 == 0;

        for(int i = 0; i < brains.size(); ++i){
            bool should_render = render && (i == 0); // Only render the first brain when rendering
            brains[i].score = ai_scorer(brains[i].brain, generation, should_render);
        }
        
        generation++;
    }

    //SDL_DestroyRenderer(renderer);
    //SDL_DestroyWindow(gWindow);
    //SDL_Quit();

    
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


// takes Ai and returns an avrage score after 10 games
float ai_scorer(Ai brain, float generation, bool render){

    bool running = true;

    float scoreSum = 0;
    float steps = 0;
    float distanceX = 0;
    float distanceY = 0;

    
    for(int i=0; i!=10; ++i){

        Game game(GAME_COLUMNS, GAME_ROWS); 
        running = true;
        steps = 0;
        bool should_render_this_game = render && (i == 0); // Only render the first game

        if(should_render_this_game){
            drawGrid(game, renderer);
            SDL_Event event;
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    // Handle quit if needed
                }
            }
            SDL_Delay(30);
        }

        while(running){
            std::vector<float> input_layer;        

            input_layer.push_back(game.check_for_collision({game.get_snake_pos().row+1, game.get_snake_pos().column}) ? 0 : 1);            
            input_layer.push_back(game.check_for_collision({game.get_snake_pos().row-1, game.get_snake_pos().column}) ? 0 : 1);            
            input_layer.push_back(game.check_for_collision({game.get_snake_pos().row, game.get_snake_pos().column+1}) ? 0 : 1);            
            input_layer.push_back(game.check_for_collision({game.get_snake_pos().row, game.get_snake_pos().column-1}) ? 0 : 1);            
            input_layer.push_back(game.get_apple_pos().row > game.get_snake_pos().row ? 0 : 1);
            input_layer.push_back(game.get_apple_pos().column > game.get_snake_pos().column ? 0 : 1);
            input_layer.push_back(game.get_apple_pos().row < game.get_snake_pos().row ? 0 : 1);
            input_layer.push_back(game.get_apple_pos().column < game.get_snake_pos().column ? 0 : 1);
            input_layer.push_back(game.get_snake_pos().row/game.getRows());
            input_layer.push_back(game.get_snake_pos().column/game.getColumns());

            input_layer.push_back(distanceX);
            input_layer.push_back(distanceY);
            distanceX = (game.get_snake_pos().row - game.get_apple_pos().row)/game.getRows();
            distanceY = (game.get_snake_pos().column - game.get_apple_pos().column)/game.getColumns();
            input_layer.push_back(distanceX);
            input_layer.push_back(distanceY);

            std::vector<float> output_layer = brain.think(input_layer);

            
            // i will arbitrary say that the first output is up, second is down, third is right, and the fourth will be left

            int biggest_index = 0;
            float biggest_output = output_layer[0];
            for(int i = 1; i!=output_layer.size(); ++i){
                if (output_layer[i] > biggest_output){
                    biggest_index = i;
                    biggest_output = output_layer[i];
                } 
            }

            direction dir = up;

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

            if(!game.snake_update(dir)) {
                running = false;
                break;
            }

            if(should_render_this_game){
                drawGrid(game, renderer);
                SDL_Event event;
                while(SDL_PollEvent(&event)) {
                    if(event.type == SDL_QUIT) {
                        // Handle quit if needed
                    }
                }
                SDL_Delay(30);
            }
            scoreSum+=0.1;

            steps++;
            if(steps>170) {
                running = false;
                break;
            }
            if(game.check_apple()) steps = 0;

        }

        scoreSum += 100*std::pow(game.snake_size()-4, 3);
    }
    return scoreSum/10;


}