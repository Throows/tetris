#include "Tetromino.hpp"

#include <stdlib.h>
#include <iostream>
#include <random>

Tetromino::Tetromino()
{
    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, SHAPE_MAX - 1);
    m_type = static_cast<TetrominoType>(uniform_dist(e1));
    Tetromino::CreateTetromino();
}

Tetromino::Tetromino(TetrominoType type)
    : m_type(type)
{
    Tetromino::CreateTetromino();
}

Tetromino::Tetromino(const Tetromino &tetromino)
{
    this->m_type = tetromino.m_type;
    this->m_rotation = tetromino.m_rotation;
    this->m_center_position = tetromino.m_center_position;
    this->m_positions = tetromino.m_positions;
}

void Tetromino::Update()
{
    Tetromino::MoveDown();
}

void Tetromino::Render(sf::RenderWindow &window, float size)
{
    for (auto &position : this->m_positions)
    {
        sf::RectangleShape rectangle({size, size});
        rectangle.setFillColor(GetColor());
        rectangle.setPosition(GetAbsolutePosition(position) * size);
        window.draw(rectangle);
    }
}

void Tetromino::MoveLeft()
{
    this->m_center_position.x--;
}

void Tetromino::MoveRight()
{
    this->m_center_position.x++;
}

void Tetromino::MoveDown()
{
    this->m_center_position.y++;
}

void Tetromino::MoveUp()
{
    this->m_center_position.y--;
}

void Tetromino::Rotate()
{
    for (auto &position : this->m_positions)
    {
        int x = position.x;
        position.x = -position.y;
        position.y = x;
    }
    this->m_rotation = static_cast<Rotation>((static_cast<int>(this->m_rotation) + 1) % 4);
}

bool Tetromino::Move(Movement movement) {
    switch (movement)
    {
    case Movement::LEFT:
        this->MoveLeft();
        break;
    case Movement::RIGHT:
        this->MoveRight();
        break;
    case Movement::DOWN:
        this->MoveDown();
        break;
    case Movement::ROTATE:
        this->Rotate();
        break;
    default:
        break;
    }
    return true;
}

bool Tetromino::Revert(Movement movement)
{
    switch (movement)
    {
    case Movement::LEFT:
        this->MoveRight();
        break;
    case Movement::RIGHT:
        this->MoveLeft();
        break;
    case Movement::DOWN:
        this->MoveUp();
        break;
    case Movement::ROTATE:
        this->Rotate();
        this->Rotate();
        this->Rotate();
        break;
    default:
        break;
    }
    return true;
}

std::vector<Block> Tetromino::GetBlocks() const
{
    std::vector<Block> blocks;
    sf::Color color = Tetromino::GetColor();
    for (auto &position : this->m_positions)
    {
        blocks.push_back(Block{GetAbsolutePosition(position), color});
    }
    return blocks;
}

sf::Vector2f Tetromino::GetAbsolutePosition(sf::Vector2i position) const
{
    return sf::Vector2f{this->m_center_position.x + position.x + 0.0f, this->m_center_position.y + position.y + 0.0f};
}

void Tetromino::CreateTetromino()
{
    switch (this->m_type)
    {
    case BAR:
        this->m_positions = { {-2, -1}, {-1, -1}, {0, -1}, {1, -1} };
        break;
    case T_SHAPE:
        this->m_positions = { {-1, -1}, {0, -1}, {1, -1}, {0, 0} };
        break;
    case CUBE:
        this->m_positions = { {-1, -1}, {0, -1}, {-1, 0}, {0, 0} };
        break;
    case L_SHAPE:
        this->m_positions = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0} };
        break;
    case J_SHAPE:
        this->m_positions = { {-1, -1}, {0, -1}, {1, -1}, {1, 0} };
        break;
    case Z_SHAPE: 
        this->m_positions = { {-1, -1}, {0, -1}, {0, 0}, {1, 0} };
        break;
    case S_SHAPE:
        this->m_positions = { {1, -1}, {0, -1}, {0, 0}, {-1, 0} };
        break;
    default:
        break;
    }
}

sf::Color Tetromino::GetColor() const
{
    switch (this->m_type)
    {
    case BAR:
        return sf::Color::Cyan;
        break;
    case T_SHAPE:
        return sf::Color(128, 0, 128);   // Purple
        break;
    case CUBE:
        return sf::Color::Yellow;
        break;
    case L_SHAPE:
        return sf::Color(255, 125, 0);  // Orange
        break;
    case J_SHAPE:
        return sf::Color::Blue;
        break;
    case Z_SHAPE:
        return sf::Color::Red;
        break;
    case S_SHAPE:
        return sf::Color::Green;
        break;
    default:
        break;
    }
    return sf::Color::White;
}
