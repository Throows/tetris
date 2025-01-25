#pragma once
#include <random>
#include "State.hpp"

#include "Tetromino.hpp"

class TetrisState : public State
{
public:
    TetrisState(StatesContext& context, RessourceManager& ressource_manager);
    ~TetrisState() = default;

    void Init(sf::Vector2u window_size);

    void MoveTetromino(Movement movement);

    void ProcessEvents(sf::Event& event) override;
    void Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
    static constexpr float SIZE = 25.0f;
    std::vector<Tetromino> fixed_tetrominos;
    sf::Vector2f m_board_position = { 50, 50 };
    std::random_device r;

    sf::Time elapsed_time = sf::Time::Zero;
    uint16_t update_number = 0;
    sf::Time speed_time = sf::milliseconds(500);
    bool is_game_over = false;
    int score = 0;

    sf::Text game_over_text;
    sf::Text score_text;
    Tetromino tetromino;
    Tetromino next_tetromino;
    sf::Sprite background;

    bool IsColliding(Tetromino& new_tetromino);
    void CheckLines();

    void UpdateScore(uint8_t lines);

    bool CanTetrominoMove(Movement movement);
    Tetromino CreateTetromino();

};
