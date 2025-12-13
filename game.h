#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <stdexcept>

typedef enum {
    left,
    right,
    up,
    down
} direction;

typedef enum {
    empty,
    snake,
    apple
} cell_type;

struct Cell{
    cell_type type;
    SDL_Rect rect;
};

struct snake_Cell{
    Cell* cell;
    int row;
    int column;
};


class Game {
    const int rows_;
    const int columns_;
    std::vector<std::vector<Cell>> cells_;
    std::vector<snake_Cell> snake_arr; // the first element will be the tail and the last will be the head

public:
    Game(int rows, int columns) : rows_(rows), columns_(columns) {
        cells_.resize(rows_, std::vector<Cell>(columns_));

        for (int i=0; i!=columns_; ++i){
            auto& row = cells_[i];
            for (int j=0; j!=rows_; ++j){
                cells_[j][i] = {empty, {i*50, j*50, 50, 50}};
            }
        }
        snake_push_back(19, 5);
        snake_push_back(19, 6);
        snake_push_back(19, 7);
        snake_push_back(19, 8);
        snake_push_back(19, 9);
    }

    int getRows(){return rows_; }     
    int getColumns(){return columns_; }     

    std::vector<std::vector<Cell>>& getCells(){return cells_; }

    void snake_push_back(int row, int column){
        snake_Cell tempCell{&cells_[row][column], row, column};
        snake_arr.push_back(tempCell);
        tempCell.cell->type = snake;
    }
    void snake_remove_tail(){
        snake_arr.begin()->cell->type = empty;
        snake_arr.erase(snake_arr.begin());
    }
    bool snake_update(direction dir){
        snake_remove_tail();
        int tempRow = snake_arr.back().row;
        int tempColumn = snake_arr.back().column;

        switch(dir){
            case up:
                tempRow--;
                break;
            case down:
                tempRow++;
                break;
            case left:
                tempColumn--;
                break;
            case right:
                tempColumn++;
                break;
            default:
                throw std::runtime_error("wtf in switch");
        }
        if (!check_for_collision(tempRow, tempColumn)){return false;}
        snake_push_back(tempRow, tempColumn);
        return true;
    }
    // returns true if its a valid move
    bool check_for_collision(int tempRow, int tempColumn){
        if(tempRow > rows_ || tempRow < 0 || tempColumn > columns_ || tempColumn < 0)
            return false;
        return true;
    }


};


