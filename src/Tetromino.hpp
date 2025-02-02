#pragma once 

#include <SFML/Graphics.hpp>
#include <map>

enum TetrominoType : uint8_t
{
    BAR         = 0,
    T_SHAPE     = 1,
    CUBE        = 2,
    L_SHAPE     = 3,
    J_SHAPE     = 4,
    Z_SHAPE     = 5,
    S_SHAPE     = 6,
    SHAPE_MAX   = 7,
};

enum Rotation : uint8_t {
    ROTATION_0      = 0,
    ROTATION_90     = 1,
    ROTATION_180    = 2,
    ROTATION_270    = 3
};

enum Movement : uint8_t
{
    LEFT = 0,
    RIGHT = 1,
    DOWN = 2,
    ROTATE = 3,
    BOTTOM = 4
};

std::ostream& operator<<(std::ostream& os, const Movement& move);
std::ostream& operator<<(std::ostream& os, const TetrominoType& type);

class Tetromino
{
public:
    Tetromino() = default;
    ~Tetromino() = default;

    TetrominoType GetType() const { return m_type; };
    std::array<sf::Vector2i, 4> GetAbsoluteCoordinates() const;
    const sf::Vector2i& GetCenterCoordinates() const { return this->m_coordinates; }

    void ResetPosition(sf::Vector2i position = {4, 1});
    void SetType(TetrominoType type);
    void SetRotation(Rotation rotation);
    void SetCoordinates(sf::Vector2i coordinates) { this->m_coordinates = coordinates; }

    bool Move(Movement movement);
    bool Revert(Movement movement);

private:
    TetrominoType m_type;
    Rotation m_rotation = Rotation::ROTATION_0;
    sf::Vector2i m_coordinates = {0, 0};                           // Coordinate of next block
    std::array<sf::Vector2i, 4> m_relative_coordinates;   // Relative coordinates of the block parts

    void CreateTetromino();

    void MoveLeft() { this->m_coordinates.x--; }
    void MoveRight() { this->m_coordinates.x++; }
    void MoveDown() { this->m_coordinates.y++; }
    void MoveUp() { this->m_coordinates.y--;}
    void Rotate();
    void UnRotate();
    void RotateBlocks();
};
