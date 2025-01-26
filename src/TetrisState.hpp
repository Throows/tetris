#pragma once
#include <random>
#include "State.hpp"

#include "Tetromino.hpp"

class TetrisState : public State
{
public:
    TetrisState(StatesContext& context, RessourceManager& ressource_manager);
    ~TetrisState() = default;

    void MoveTetromino(Movement movement);

    bool ProcessEvents(sf::Event& event) override;
    bool Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
    static constexpr float SIZE = 25.0f;
    static constexpr int NEXT_TETROMINO_COUNT = 5;

    std::random_device r;
    std::array<TetrominoType, BOARD_WIDTH * BOARD_HEIGHT> m_board = { TetrominoType::SHAPE_MAX };
    std::array<TetrominoType, NEXT_TETROMINO_COUNT> falling_tetrominos;

    sf::Time elapsed_time = sf::Time::Zero;
    uint16_t update_number = 0;
    sf::Time speed_time = sf::milliseconds(500);
    int score = 0;
    bool game_over = false;

    const sf::Vector2f m_board_position = { 50, 0 };
    const sf::Vector2f m_next_tetromino_position = { 425, 175 };
    sf::Sprite tetromono_sprite;
    sf::Text score_text;
    Tetromino tetromino;
    Tetromino next_tetromino;

    bool IsColliding(Tetromino& new_tetromino);
    void CheckLines();

    void UpdateScore(uint8_t lines);

    bool CanTetrominoMove(Movement movement);
    void FixTetromino();

    sf::IntRect GetTextureRect(TetrominoType type);

    TetrominoType GetRandomTetromino();
};
