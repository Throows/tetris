#pragma once
#include "Tetromino.hpp"

class Tetris
{
public:
    Tetris() = default;
    ~Tetris() = default;

    void MoveTetromino(Movement movement);

    void Update(sf::Time elapsed);
    void Render(sf::RenderWindow &window);

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 11;
    Tetromino tetromino = Tetromino();
    std::array<Block, BOARD_WIDTH * BOARD_HEIGHT> blocks = {};

    bool IsColliding(Tetromino& tetromino);
    bool IsOutOfBoard(Tetromino& tetromino);
    void CheckLines();
    void FixTetromino();

    bool CanTetrominoMove(Movement movement);

};
