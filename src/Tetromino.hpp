#pragma once 

#include <SFML/Graphics.hpp>

enum TetrominoType 
{
    BAR = 0,
    T_SHAPE,
    CUBE,
    L_SHAPE,
    J_SHAPE,
    Z_SHAPE,
    S_SHAPE,
    SHAPE_MAX
};

enum Rotation {
    ROTATION_0 = 0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270
};

enum Movement
{
    LEFT = 0,
    RIGHT = 1,
    DOWN = 2,
    ROTATE = 3
};

struct Block
{
    sf::Vector2f position;
    sf::Color color;

    Block() : position(-1, -1), color(sf::Color::Black){};
    Block(sf::Vector2f position, sf::Color color) : position(position), color(color){};

    void Render(sf::RenderWindow &window, float size)
    {
        sf::RectangleShape block;
        block.setSize(sf::Vector2f(size, size));
        block.setPosition(this->position * size);
        block.setFillColor(this->color);
        window.draw(block);
    }
};

class Tetromino
{
public:
    Tetromino();
    Tetromino(TetrominoType type);
    Tetromino(const Tetromino &tetromino);
    ~Tetromino() = default;

    void Update();
    void Render(sf::RenderWindow &window, float size);

    bool Move(Movement movement);
    bool Revert(Movement movement);

    std::vector<Block> GetBlocks() const;

private:
    TetrominoType m_type;
    Rotation m_rotation;
    sf::Vector2i m_center_position = { 5, 1 };
    std::vector<sf::Vector2i> m_positions;                    // Set of relative positions to the center position

    void CreateTetromino();

    void MoveLeft();
    void MoveRight();
    void MoveDown();
    void MoveUp();
    void Rotate();

    sf::Color GetColor() const;
    sf::Vector2f GetAbsolutePosition(sf::Vector2i position) const;
};
