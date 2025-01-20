#include "Tetris.hpp"
#include <spdlog/spdlog.h>

void Tetris::MoveTetromino(Movement direction)
{
    if (Tetris::CanTetrominoMove(direction)) {
        tetromino.Move(direction);
    }
}

void Tetris::Update(sf::Time elapsed)
{
    if (elapsed.asMilliseconds() > 800) {

        if (Tetris::CanTetrominoMove(Movement::DOWN)) {
            tetromino.Update();
        } else {
            Tetris::FixTetromino();
            tetromino = Tetromino();
            Tetris::CheckLines();
        }
    }
}

void Tetris::Render(sf::RenderWindow &window)
{
    tetromino.Render(window, 50);
    for (auto &block : blocks) {
        if (block.color == sf::Color::Black) {
            continue;
        }
        block.Render(window, 50); 
    }
}

bool Tetris::IsColliding(Tetromino& tetromino)
{
    for (auto &block : blocks)
    {
        for (auto &tetromino_block : tetromino.GetBlocks())
        {
            if ((block.position == tetromino_block.position) && (block.color != sf::Color::Black)) {
                return true;
            }
        }
    }
    return false;
}

bool Tetris::IsOutOfBoard(Tetromino& tetromino)
{
    for (auto &block : tetromino.GetBlocks())
    {
        if (block.position.x < 0 || block.position.x >= BOARD_WIDTH || block.position.y >= BOARD_HEIGHT) {
            return true;
        }
    }
    return false;
}

void Tetris::CheckLines()
{
    uint8_t line_total_blocks[BOARD_HEIGHT];
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        line_total_blocks[i] = 0;
    }

    for (int y = 0; y < BOARD_HEIGHT; y++) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (blocks.at(y * BOARD_WIDTH + x).color != sf::Color::Black) {
                line_total_blocks[y]++;
            }
        }
        if (line_total_blocks[y] == BOARD_WIDTH) {
            for (int i = y; i > 0; i--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    blocks.at(i * BOARD_WIDTH + x) = blocks.at((i - 1) * BOARD_WIDTH + x);
                    blocks.at(i * BOARD_WIDTH + x).position.y++;
                }
            }
            for (int x = 0; x < BOARD_WIDTH; x++) {
                blocks.at(x) = Block{sf::Vector2f(x, 0), sf::Color::Black};
            }
        }
    }
}

void Tetris::FixTetromino()
{
    for (const Block& block : tetromino.GetBlocks()) {
        int x = block.position.x;
        int y = block.position.y;
        blocks.at(y * BOARD_WIDTH + x) = block;
    }
}

bool Tetris::CanTetrominoMove(Movement movement)
{
    Tetromino next_position = tetromino;
    next_position.Move(movement);

    return !Tetris::IsColliding(next_position) && !Tetris::IsOutOfBoard(next_position);
}