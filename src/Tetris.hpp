#pragma once
#include "Tetromino.hpp"

class Tetris
{
public:
    Tetris() = default;
    ~Tetris() = default;

    void Init(sf::Vector2u window_size);

    void MoveTetromino(Movement movement);

    void Update(sf::Time elapsed);
    void Render(sf::RenderWindow &window);

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
    static constexpr float SIZE = 25.0f;
    Tetromino tetromino = Tetromino();
    std::vector<Tetromino> fixed_tetrominos;
    bool is_game_over = false;

    sf::Font font;
    sf::Text game_over_text = sf::Text(font, "Game Over", 50);

    bool IsColliding(Tetromino& new_tetromino);
    void CheckLines();

    bool CanTetrominoMove(Movement movement);

};
