#pragma once
#include "Tetromino.hpp"

class Tetris
{
public:
    Tetris();
    ~Tetris() = default;

    void Init(sf::Vector2u window_size);

    void MoveTetromino(Movement movement);

    void Update(sf::Time elapsed);
    void Render(sf::RenderWindow &window);

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
    static constexpr float SIZE = 25.0f;
    std::vector<Tetromino> fixed_tetrominos;

    sf::Time elapsed_time = sf::Time::Zero;
    uint16_t update_number = 0;
    sf::Time speed_time = sf::milliseconds(500);
    bool is_game_over = false;

    sf::Font font;
    sf::Text game_over_text = sf::Text(font, "Game Over", 50);
    sf::Text score_text = sf::Text(font, "Score: 0", 20);
    sf::Texture tetromino_texture;
    Tetromino tetromino = Tetromino(tetromino_texture, SIZE);
    Tetromino next_tetromino = Tetromino(tetromino_texture, SIZE);
    sf::Sprite background = sf::Sprite(tetromino_texture);
    int score = 0;

    bool IsColliding(Tetromino& new_tetromino);
    void CheckLines();

    void UpdateScore(uint8_t lines);

    bool CanTetrominoMove(Movement movement);

};
