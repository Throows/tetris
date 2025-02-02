#include "TetrisGame.hpp"

TetrisGame::TetrisGame(RessourceManager &ressource_manager)
    : m_tetromino_sprite(ressource_manager.GetTexture(TexturesID::TETROMINO))
{
}

void TetrisGame::InitGame(uint width, uint height, float size)
{
    this->m_board.resize(width * height);
    for (auto& type : this->m_board) type = TetrominoType::SHAPE_MAX;
    this->m_width = width;
    this->m_height = height;

    this->m_tetromino_sprite.setScale(sf::Vector2f{size / ASSET_SIZE, size / ASSET_SIZE});
}

void TetrisGame::NewTetromino(TetrominoType type)
{
    this->m_tetromino.SetType(type);
    this->m_tetromino.SetRotation(Rotation::ROTATION_0);
    this->m_tetromino.ResetPosition();
}

void TetrisGame::AddScore(uint fast_moves, uint completed_lines)
{
    if (fast_moves != 0) {
        this->m_score += fast_moves + 1;
    }

    if (completed_lines == 1) {
        m_score += 40;
    } else if (completed_lines == 2) {
        m_score += 100;
    } else if (completed_lines == 3) {
        m_score += 300;
    } else if (completed_lines == 4) {
        m_score += 1200;
    }

}

void TetrisGame::MoveTetromino(Movement move)
{
    if (move == Movement::BOTTOM) {
        uint moves = 0;
        while (CanTetrominoMove(Movement::DOWN)) {
            this->m_tetromino.Move(move);
            moves++;
        } 
        AddScore(moves, 0);
    } else {
        if (CanTetrominoMove(move)) {
            this->m_tetromino.Move(move);
        }
    } 
}

bool TetrisGame::UpdateBoard()
{
    bool new_tetromino = false;
    if (TetrisGame::CanTetrominoMove(Movement::DOWN)) {
        this->m_tetromino.Move(Movement::DOWN);
    } else {
        if (this->m_tetromino.GetCenterCoordinates().y > 1) {
            TetrisGame::FixTetromino();
            TetrisGame::CheckLines();
        } else {
            this->m_game_over = true;
        }
        new_tetromino = true;
    }
    return new_tetromino;
}

void TetrisGame::RenderBoard(sf::Vector2f position, float gridScale, sf::RenderWindow& window)
{

    uint x = 0, y = 0;
    for (const auto& type : this->m_board) {

        this->m_tetromino_sprite.setTextureRect(GetTextureRect(type));
        sf::Vector2f pos = position + sf::Vector2f(x * gridScale, y * gridScale);
        this->m_tetromino_sprite.setPosition(pos);
        window.draw(this->m_tetromino_sprite);

        x = (x + 1) % m_width;
        if (x == 0) y++;
    }

    // Draw the Tetromino
    this->m_tetromino_sprite.setTextureRect(GetTextureRect(this->m_tetromino.GetType()));
    for (auto &block_position : this->m_tetromino.GetAbsoluteCoordinates()) {
        this->m_tetromino_sprite.setPosition(position + (sf::Vector2f(block_position) * gridScale));
        window.draw(this->m_tetromino_sprite);
    }

}

bool TetrisGame::IsColliding(Tetromino& new_tetromino) const
{
    for (auto &position : new_tetromino.GetAbsoluteCoordinates()) {
        uint ind = position.x + position.y * this->m_width;
        if (this->m_board[ind] != TetrominoType::SHAPE_MAX 
        ||  position.x < 0
        ||  position.x >= m_width
        ||  position.y >= m_height) {
            return true;
        }
    }
    return false;
}

void TetrisGame::FixTetromino()
{
    for (auto &position : this->m_tetromino.GetAbsoluteCoordinates()) {
        assert(this->m_board[position.x + position.y * this->m_width] == TetrominoType::SHAPE_MAX);
        this->m_board[position.x + position.y * this->m_width] = m_tetromino.GetType();
    }
}

void TetrisGame::CheckLines()
{
    uint8_t lines_full = 0;
    for (int y = (this->m_height-1); y > 0; y--) { 
        bool is_line_full = true;
        for (int x = 0; x < this->m_width; x++) {
            if (this->m_board[x + y * this->m_width] == TetrominoType::SHAPE_MAX) {
                is_line_full = false;
                break;
            }
        }
        if (is_line_full) {
            if (this->m_board[y * m_width] != TetrominoType::SHAPE_MAX) lines_full++;
            for (int new_y = y; new_y > 0; new_y--) {
                for (int x = 0; x < m_width; x++) {
                    this->m_board[x + new_y * m_width] = this->m_board[x + (new_y-1) * m_width];
                }
            }
            y++; // Check the same line again as it has been moved down
        }
    }
    TetrisGame::AddScore(0, lines_full);
}

bool TetrisGame::CanTetrominoMove(Movement movement)
{
    Tetromino next_position = this->m_tetromino;
    next_position.Move(movement);
    return !TetrisGame::IsColliding(next_position);
}
