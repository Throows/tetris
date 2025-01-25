#include "Tetromino.hpp"

#include <stdlib.h>
#include <iostream>
#include <spdlog/spdlog.h>

Tetromino::Tetromino(const sf::Texture& texture, sf::Vector2i coordinates, TetrominoType type, float size, Rotation rotation)
    : m_type(type)
    , m_size(size)
    , m_rotation(rotation)
    , m_coordinates(coordinates)
    , block_sprite(texture)
{
    Tetromino::CreateTetromino();
    Tetromino::ApplyTexture();
}

Tetromino::Tetromino(const Tetromino &tetromino)
    : m_type(tetromino.m_type)
    , m_size(tetromino.m_size)
    , m_rotation(tetromino.m_rotation)
    , m_coordinates(tetromino.m_coordinates)
    , m_relative_coordinates(tetromino.m_relative_coordinates)
    , block_sprite(tetromino.block_sprite.getTexture())
{
    Tetromino::ApplyTexture();
}

void Tetromino::Update()
{
    Tetromino::MoveDown();
}

void Tetromino::Render(sf::RenderWindow &window, sf::Vector2f position)
{
    for (auto &relative_position : this->m_relative_coordinates)
    {
        sf::Vector2f board_position = (sf::Vector2f(this->m_coordinates + relative_position) * this->m_size) + position;
        block_sprite.setPosition(board_position);
        window.draw(block_sprite);
    }
}

void Tetromino::MoveLeft()
{
    this->m_coordinates.x--;
}

void Tetromino::MoveRight()
{
    this->m_coordinates.x++;
}

void Tetromino::MoveDown()
{
    this->m_coordinates.y++;
}

void Tetromino::MoveUp()
{
    this->m_coordinates.y--;
}

void Tetromino::Rotate()
{
    if (this->m_type == TetrominoType::CUBE)
        return;

    // Rotate anti-clockwise
    for (auto &position : this->m_relative_coordinates) {
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
    case Movement::BOTTOM:
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

bool Tetromino::SetActiveTetromino()
{
    if (this->m_type == TetrominoType::CUBE || this->m_type == TetrominoType::BAR) {
        this->m_coordinates = {5, -1};
    } else {
        this->m_coordinates = {4, -1};
    }
    return true;
}

std::vector<sf::Vector2i> Tetromino::GetAbsoluteCoordinates() const
{
    std::vector<sf::Vector2i> absolute_coordinates;
    for (auto &position : this->m_relative_coordinates)
    {
        absolute_coordinates.push_back(position + this->m_coordinates);
    }
    return absolute_coordinates;
}

bool Tetromino::IsOutOfBoard(uint16_t width, uint16_t height) const
{
    for (auto &position : this->m_relative_coordinates)
    {
        auto position_x = position.x + this->m_coordinates.x;
        auto position_y = position.y + this->m_coordinates.y;

        if (position_x < 0 ||  position_x >= width ||  position_y >= height)
            return true;
    }
    return false;
}

bool Tetromino::IsColliding(const Tetromino& tetromino) const 
{
    for (auto &position : Tetromino::GetAbsoluteCoordinates())
    {
        for (auto &other_potitions : tetromino.GetAbsoluteCoordinates())
        {
            if (position == other_potitions)
                return true;
        }
    }
    return false;
}

void Tetromino::RemoveLine(uint8_t line)
{
    this->m_relative_coordinates.erase(
        std::remove_if(this->m_relative_coordinates.begin(), 
                       this->m_relative_coordinates.end(), 
                       [line, this](sf::Vector2i position) { 
                            return (position.y + this->m_coordinates.y) == line;
                        }),
                       this->m_relative_coordinates.end());
}

void Tetromino::MovePartsDown(uint8_t line)
{
    for (auto &position : this->m_relative_coordinates) {
        if ((position.y + this->m_coordinates.y) < line){
            position.y++;
        }
    }
}

Tetromino& Tetromino::operator=(const Tetromino &tetromino)
{
    if (this != &tetromino) {
        m_type = tetromino.m_type;
        m_size = tetromino.m_size;
        m_rotation = tetromino.m_rotation;
        m_coordinates = tetromino.m_coordinates;
        m_relative_coordinates = tetromino.m_relative_coordinates;
        block_sprite = tetromino.block_sprite;
        ApplyTexture();
    }
    return *this;
}

void Tetromino::CreateTetromino()
{
    switch (this->m_type)
    {
    case BAR:
        this->m_relative_coordinates = { {-2, 0}, {-1, 0}, {0, 0}, {1, 0} };
        break;
    case T_SHAPE:
        this->m_relative_coordinates = {{0 , -1}, {-1, 0}, {1, 0}, {0, 0} };
        break;
    case CUBE:
        this->m_relative_coordinates = { {-1, -1}, {0, -1}, {0, 0}, {-1, 0} };
        break;
    case L_SHAPE:
        this->m_relative_coordinates = { {-1, 0}, {0, 0}, {1, 0}, {1, -1} };
        break;
    case J_SHAPE:
        this->m_relative_coordinates = { {-1, -1}, {-1, 0}, {0, 0}, {1, 0} };
        break;
    case Z_SHAPE: 
        this->m_relative_coordinates = { {-1, -1}, {0, -1}, {0, 0}, {1, 0} };
        break;
    case S_SHAPE:
        this->m_relative_coordinates = { {-1, 0}, {0, 0}, {0, -1}, {1, -1} };
        break;
    default:
        break;
    }
    
    if (this->m_type == TetrominoType::CUBE) return;
    for (int i = 0; i < static_cast<int>(this->m_rotation); i++) {
        for (auto &position : this->m_relative_coordinates) {
            int x = position.x;
            position.x = -position.y;
            position.y = x;
        }
    }
}

void Tetromino::ApplyTexture()
{
    this->block_sprite.setScale({this->m_size / 384.0f, this->m_size / 384.0f});
    this->block_sprite.setTextureRect(sf::IntRect({this->m_type * 384, 0}, {384, 384}));
}
