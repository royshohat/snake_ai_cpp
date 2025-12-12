#include <vector>
#include <memory>
#include <SDL2/SDL.h>

typedef enum {
    empty,
    snake,
    apple
} cell_type;

struct Cell{
    cell_type type;
    SDL_Rect rect;
};

class Game {
    const int rows_;
    const int columns_;
    std::vector<std::vector<Cell>> cells_;

public:
    Game(int rows, int columns) : rows_(rows), columns_(columns) {
        cells_.resize(rows_, std::vector<Cell>(columns_));

        for (int i=0; i!=rows_; ++i){
            auto& row = cells_[i];
            for (int j=0; j!=columns_; ++j){
                cells_[i][j] = {empty, {i*50, j*50, 50, 50}};
            }
        }
        cells_[3][3].type = apple;
        cells_[5][5].type = snake;
        cells_[5][6].type = snake;
        cells_[5][7].type = snake;
    }

    int getRows(){return rows_; }     
    int getColumns(){return columns_; }     

    std::vector<std::vector<Cell>>& getCells(){return cells_; }
};


