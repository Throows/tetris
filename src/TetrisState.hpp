#pragma once
#include <random>
#include "State.hpp"
#include <list>

#include "TetrisGame.hpp"

struct ActionHistory {
    enum ActionType {
        MOVEMENT,
        GAME,
        NEW_PIECE
    } type;
    union {
        TetrominoType block_type;
        Movement move;
        uint8_t data;
    };
};

std::ostream& operator<<(std::ostream& os, const ActionHistory& action);

class TetrisState : public State
{
public:
    TetrisState(StatesContext& context, RessourceManager& ressource_manager, sf::RenderWindow& window);
    ~TetrisState() = default;

    void MoveTetromino(Movement movement);

    bool ProcessEvents(sf::Event& event) override;
    bool Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
    static constexpr int BOARD_WIDTH = 10;
    static constexpr int BOARD_HEIGHT = 20;
    float SIZE = 50.0f;
    static constexpr int NEXT_TETROMINO_COUNT = 5;

    TetrisGame m_game;

    std::random_device r;
    std::array<TetrominoType, NEXT_TETROMINO_COUNT> falling_tetrominos;

    Tetromino m_next_tetromino;

    sf::Time elapsed_time = sf::Time::Zero;
    uint16_t update_number = 0;
    sf::Time speed_time = sf::milliseconds(500);

    const sf::Vector2f m_board_position = { SIZE * 2, 0 };
    
    sf::Text m_score_text;
    sf::Sprite m_tetromino_sprite;

    std::list<ActionHistory> m_action_history;
    void SaveActions();

    TetrominoType GetRandomTetromino();
};
