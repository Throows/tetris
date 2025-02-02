#include "Tetromino.hpp"

#include <stdlib.h>
#include <iostream>
#include <spdlog/spdlog.h>

bool Tetromino::Move(Movement movement) {
    switch (movement)
    {
    case Movement::LEFT:
        Tetromino::MoveLeft();
        break;
    case Movement::RIGHT:
        Tetromino::MoveRight();
        break;
    case Movement::DOWN:
    case Movement::BOTTOM:
        Tetromino::MoveDown();
        break;
    case Movement::ROTATE:
        Tetromino::Rotate();
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
        Tetromino::MoveRight();
        break;
    case Movement::RIGHT:
        Tetromino::MoveLeft();
        break;
    case Movement::DOWN:
        Tetromino::MoveUp();
        break;
    case Movement::ROTATE:
        Tetromino::UnRotate();
        break;
    default:
        break;
    }
    return true;
}

void Tetromino::ResetPosition(sf::Vector2i position)
{
    this->m_coordinates = position;

    if (this->m_type == TetrominoType::CUBE || this->m_type == TetrominoType::BAR)
        this->m_coordinates.x++;
    if (this->m_type == TetrominoType::BAR)
        this->m_coordinates.y--;
}

void Tetromino::SetType(TetrominoType type)
{
    this->m_type = type;
    Tetromino::CreateTetromino();
}

void Tetromino::SetRotation(Rotation rotation)
{
    this->m_rotation = rotation;
    Tetromino::CreateTetromino();
    Tetromino::RotateBlocks();
}

std::array<sf::Vector2i, 4> Tetromino::GetAbsoluteCoordinates() const
{
    std::array<sf::Vector2i, 4> absolute_coordinates = this->m_relative_coordinates;
    for (auto &position : absolute_coordinates){
        position += this->m_coordinates;
    }
    return absolute_coordinates;
}

void Tetromino::CreateTetromino()
{
    switch (this->m_type)
    {
    case BAR:
        this->m_relative_coordinates = { sf::Vector2i(-2, 0), sf::Vector2i(-1, 0), sf::Vector2i(0, 0), sf::Vector2i(1, 0) };
        break;
    case T_SHAPE:
        this->m_relative_coordinates = { sf::Vector2i(0, -1), sf::Vector2i(-1, 0), sf::Vector2i(1, 0), sf::Vector2i(0, 0) };
        break;
    case CUBE:
        this->m_relative_coordinates = { sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(0, 0), sf::Vector2i(-1, 0) };
        break;
    case L_SHAPE:
        this->m_relative_coordinates = { sf::Vector2i(-1, 0), sf::Vector2i(0, 0), sf::Vector2i(1, 0), sf::Vector2i(1, -1) };
        break;
    case J_SHAPE:
        this->m_relative_coordinates = { sf::Vector2i(-1, -1), sf::Vector2i(-1, 0), sf::Vector2i(0, 0), sf::Vector2i(1, 0) };
        break;
    case Z_SHAPE: 
        this->m_relative_coordinates = { sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(0, 0), sf::Vector2i(1, 0) };
        break;
    case S_SHAPE:
        this->m_relative_coordinates = { sf::Vector2i(-1, 0), sf::Vector2i(0, 0), sf::Vector2i(0, -1), sf::Vector2i(1, -1) };
        break;
    default:
        break;
    }
}

void Tetromino::Rotate()
{
    this->m_rotation = static_cast<Rotation>((static_cast<int>(this->m_rotation) + 1) % 4);
    // Rotate the blocks Clockwise
    for (auto &position : this->m_relative_coordinates) {
        int x = position.x;
        position.x = -position.y;
        position.y = x;
    }
}

void Tetromino::RotateBlocks()
{
    if (this->m_type == TetrominoType::CUBE) return;
    for (int i = 0; i < static_cast<int>(this->m_rotation); i++) {
        for (auto &position : this->m_relative_coordinates) {
            int x = position.x;
            position.x = -position.y;
            position.y = x;
        }
    }
}

void Tetromino::UnRotate()
{
    this->m_rotation = static_cast<Rotation>((static_cast<int>(this->m_rotation) + 3) % 4);
    // Rotate the blocks Counter-Clockwise
    for (auto &position : this->m_relative_coordinates) {
        int x = position.x;
        position.x = position.y;
        position.y = -x;
    }
}

std::ostream &operator<<(std::ostream &os, const Movement &move)
{
    switch (move)
    {
    case LEFT:
        os << "LEFT";
        break;
    case RIGHT:
        os << "RIGHT";
        break;
    case DOWN:
        os << "DOWN";
        break;
    case ROTATE:
        os << "ROTATE";
        break;
    case BOTTOM:
        os << "BOTTOM";
        break;
    default:
        break;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const TetrominoType &type)
{
    switch (type)
    {
    case BAR:
        os << "BAR";
        break;
    case T_SHAPE:
        os << "T_SHAPE";
        break;
    case CUBE:
        os << "CUBE";
        break;
    case L_SHAPE:
        os << "L_SHAPE";
        break;
    case J_SHAPE:
        os << "J_SHAPE";
        break;
    case Z_SHAPE:
        os << "Z_SHAPE";
        break;
    case S_SHAPE:
        os << "S_SHAPE";
        break;
    default:
        break;
    }
    return os;
}
