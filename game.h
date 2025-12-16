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

struct pos{
    int row;
    int column;
};

struct Cell{
    cell_type type;
    SDL_Rect rect;
    pos position;
};



class Game {
    const int rows_;
    const int columns_;
    pos apple_;
    std::vector<std::vector<Cell>> cells_;
    std::vector<Cell> snake_arr; // the first element will be the tail and the last will be the head

public:
    Game(int columns, int rows);
    

    int getRows(){return rows_; }     
    int getColumns(){return columns_; }     
    pos get_apple_pos(){return apple_; }
    pos get_snake_pos(){return snake_arr.back().position; }

    std::vector<std::vector<Cell>>& getCells(){return cells_; }

    void snake_push_back(pos p);
    int snake_size(){return snake_arr.size();};
    void snake_remove_tail();
    void add_apple();
    bool check_apple();
    bool snake_update(direction dir);
    bool check_for_collision(pos p); //returns true for vaild


};


