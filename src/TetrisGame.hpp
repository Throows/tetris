#pragma once 
#include <vector>

#include "Tetromino.hpp"
#include "RessourceManager.hpp"

class TetrisGame
{
public:
    TetrisGame(RessourceManager& ressource_manager);
    ~TetrisGame() = default;

    void InitGame(uint width, uint height, float size);

    void NewTetromino(TetrominoType type);
    Tetromino& GetTetromino() { return this->m_tetromino; }
    void AddScore(uint fast_moves, uint completed_lines);
    void MoveTetromino(Movement move);

    bool UpdateBoard();
    void RenderBoard(sf::Vector2f position, float gridScale, sf::RenderWindow& window);

    uint GetScore() const { return this->m_score; }
    bool IsGameOver() const { return this->m_game_over; }
    bool IsColliding(Tetromino& new_tetromino) const;

    sf::IntRect GetTextureRect(TetrominoType type) {
        return sf::IntRect(sf::Vector2i(static_cast<int>(type) * ASSET_SIZE, 0), sf::Vector2i(ASSET_SIZE, ASSET_SIZE));
    }

private:
    int m_width = 0;
    int m_height = 0;
    std::vector<TetrominoType> m_board;
    uint m_score = 0;
    bool m_game_over = false;

    sf::Sprite m_tetromino_sprite;
    Tetromino m_tetromino;

    void FixTetromino();
    void CheckLines();
    bool CanTetrominoMove(Movement movement);
};
