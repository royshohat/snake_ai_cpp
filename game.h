#include <vector>
#include <memory>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <random>

typedef enum {
    left=0,
    right=1,
    up=2,
    down=3
} direction;

typedef enum {
    empty,
    snake,
    apple
} cell_type;

struct Cell{
    int row;
    int column;
    cell_type type;
    SDL_Rect rect;
};

struct pos{
    int row;
    int column;
};


class Game {
    const int rows_;
    const int columns_;
    pos apple_;
    std::vector<std::vector<Cell>> cells_;
    std::vector<Cell> snake_arr; // the first element will be the tail and the last will be the head

public:
    Game(int columns, int rows) : rows_(rows), columns_(columns) {
        cells_.resize(rows_, std::vector<Cell>(columns_));

        for (int i=0; i!=rows_; ++i){
            for (int j=0; j!=columns_; ++j){
                cells_[i][j] = {i, j, empty, {j*50, i*50, 50, 50}};
            }
        }
        snake_push_back({9, 5});
        snake_push_back({9, 6});
        snake_push_back({9, 7});
        snake_push_back({9, 8});
        snake_push_back({9, 9});

        add_apple();
    }

    int getRows(){return rows_; }     
    int getColumns(){return columns_; }     

    std::vector<std::vector<Cell>>& getCells(){return cells_; }

    void snake_push_back(pos p);
    void snake_remove_tail();
    void add_apple();
    bool check_apple();
    bool snake_update(direction dir);
    bool check_for_collision(pos p); //returns true for vaild


};


