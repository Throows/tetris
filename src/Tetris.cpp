#include "Tetris.hpp"
#include <spdlog/spdlog.h>

Tetris::Tetris()
{
    this->tetromino_texture = sf::Texture("assets/image/tetrominos.png");
    this->background.setScale({this->SIZE / 384.0f, this->SIZE / 384.0f});
    this->background.setTextureRect(sf::IntRect({TetrominoType::SHAPE_MAX * 384, 0}, {384, 384}));
}

void Tetris::Init(sf::Vector2u window_size)
{
    if (!font.openFromFile("assets/font/CheeseMarket.ttf")) {
        spdlog::error("Cannot load font !");
    }

    this->game_over_text = sf::Text(font, "Game Over", 50);
    this->game_over_text.setFillColor(sf::Color::Red);

    sf::Vector2f position = {window_size.x / 2 - this->game_over_text.getGlobalBounds().size.x / 2,
                             window_size.y / 2 - this->game_over_text.getGlobalBounds().size.y / 2};
    this->game_over_text.setPosition(position);

    this->score_text.setPosition({350, 300});
}

void Tetris::MoveTetromino(Movement direction)
{
    if (direction == Movement::BOTTOM) {
        uint8_t movement = 0;
        while (Tetris::CanTetrominoMove(Movement::DOWN)) {
            tetromino.Move(direction);
            movement++;
        }
        score += movement + 1;
    } else {
        if (Tetris::CanTetrominoMove(direction)) {
            tetromino.Move(direction);
        }
    } 
}

void Tetris::Update(sf::Time elapsed)
{
    if (elapsed.asMilliseconds() > 800 && !is_game_over) {

        if (Tetris::CanTetrominoMove(Movement::DOWN)) {
            tetromino.Update();
        } else {
            this->fixed_tetrominos.push_back(tetromino);
            tetromino = Tetromino(this->tetromino_texture);
            Tetris::CheckLines();
            if (Tetris::IsColliding(tetromino)) {
                is_game_over = true;
            }
        }
    }
    this->score_text.setString("Score: " + std::to_string(score));
}

void Tetris::Render(sf::RenderWindow &window)
{
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            this->background.setPosition({i * this->SIZE + 50, j * this->SIZE + 50});
            window.draw(this->background);
        }
    }

    for (auto &fixed_tetromino : this->fixed_tetrominos) {
        fixed_tetromino.Render(window); 
    }

    if (is_game_over) {
        window.draw(this->game_over_text);
    } else {
        tetromino.Render(window);
    }
    window.draw(this->score_text);
}

bool Tetris::IsColliding(Tetromino& new_tetromino)
{
    for (const Tetromino& fixed_tetromino : fixed_tetrominos) {   
        if (new_tetromino.IsColliding(fixed_tetromino)) {
            return true;
        }
    }
    return false;
}

void Tetris::CheckLines()
{
    uint8_t block_per_lines[BOARD_HEIGHT] = {0};

    for (const Tetromino& fixed_tetromino : fixed_tetrominos) {
        for (const sf::Vector2i& position : fixed_tetromino.GetAbsoluteCoordinates()) {
            block_per_lines[position.y]++;
        }
    }

    uint8_t lines_full = 0;
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++)
    {
        if (block_per_lines[y] == BOARD_WIDTH)
        {
            lines_full++;
            for (Tetromino& fixed_tetromino : fixed_tetrominos) {
                fixed_tetromino.RemoveLine(y);
                fixed_tetromino.MovePartsDown(y);
            } 
        }
    }
    this->fixed_tetrominos.erase(
                std::remove_if(this->fixed_tetrominos.begin(),
                            this->fixed_tetrominos.end(),
                            [](Tetromino fixed_tetromino) {
                                return fixed_tetromino.IsEmpty();
                            }),
                            this->fixed_tetrominos.end());
    Tetris::UpdateScore(lines_full);
}

void Tetris::UpdateScore(uint8_t lines)
{
    if (lines == 1) {
        score += 40;
    } else if (lines == 2) {
        score += 100;
    } else if (lines == 3) {
        score += 300;
    } else if (lines == 4) {
        score += 1200;
    }
}

bool Tetris::CanTetrominoMove(Movement movement)
{
    Tetromino next_position = tetromino;
    next_position.Move(movement);

    return !Tetris::IsColliding(next_position) && !next_position.IsOutOfBoard(BOARD_WIDTH, BOARD_HEIGHT);
}