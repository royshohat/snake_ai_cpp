#include "game.h"

void Game::snake_push_back(pos p){
    snake_arr.push_back(cells_[p.row][p.column]);
    snake_arr.back().type = snake;
    cells_[p.row][p.column].type = snake;
}

void Game::snake_remove_tail(){
    int tailRow = snake_arr.begin()->row;
    int tailColumn = snake_arr.begin()->column;
    cells_[tailRow][tailColumn].type = empty;
    snake_arr.erase(snake_arr.begin());
}

void Game::add_apple(){
    std::vector<Cell*> empty_cells;        

    for(auto& row : cells_){
        for(auto& cell : row)
            if(cell.type == empty) empty_cells.push_back(&cell);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, empty_cells.size() - 1);
    
    int random_index = distrib(gen);

    empty_cells[random_index]->type = apple;
    apple_.row = empty_cells[random_index]->row;
    apple_.column = empty_cells[random_index]->column;
}

bool Game::check_apple(){
    if (snake_arr.back().row == apple_.row && snake_arr.back().column == apple_.column)
        return true;
    return false;
}

bool Game::snake_update(direction dir){
    if(!check_apple()) snake_remove_tail();
    pos currentPos = {snake_arr.back().row, snake_arr.back().column};
    pos newPos = currentPos;

    switch(dir){
        case up:
            newPos.row--;
            break;
        case down:
            newPos.row++;
            break;
        case left:
            newPos.column--;
            break;
        case right:
            newPos.column++;
            break;
        default:
            throw std::runtime_error("wtf in switch");
    }
    if (!check_for_collision(newPos)){return false;}
    snake_push_back(newPos);
    return true;
}

// returns true if its a valid move
bool Game::check_for_collision(pos p){
    if(p.row >= rows_ || p.row < 0 || p.column >= columns_ || p.column < 0)
        return false;
    return true;
}

