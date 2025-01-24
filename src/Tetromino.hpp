#pragma once 

#include <SFML/Graphics.hpp>
#include <map>

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
    ROTATE = 3,
    BOTTOM = 4
};

class Tetromino
{
public:
    Tetromino(const sf::Texture& texture, float size = 25.0f);
    Tetromino(const sf::Texture& texture, TetrominoType type, float size = 25.0f);
    Tetromino(const Tetromino &tetromino);
    ~Tetromino() = default;

    void Update();
    void Render(sf::RenderWindow &window, sf::Vector2f position = {0, 0});

    bool Move(Movement movement);
    bool Revert(Movement movement);
    bool SetActiveTetromino();

    std::vector<sf::Vector2i> GetAbsoluteCoordinates() const;

    bool IsOutOfBoard(uint16_t width, uint16_t height) const;
    bool IsColliding(const Tetromino &tetromino) const;
    bool IsEmpty() const { return m_relative_coordinates.empty(); };

    void RemoveLine(uint8_t line);
    void MovePartsDown(uint8_t line);

    Tetromino &operator=(const Tetromino &tetromino);

private:
    TetrominoType m_type;
    float m_size;
    Rotation m_rotation;
    sf::Vector2i m_coordinates = {14, 5};               // Coordinate of next block
    std::vector<sf::Vector2i> m_relative_coordinates;   // Relative coordinates of the block parts

    sf::Sprite block_sprite;

    void CreateTetromino();
    void ApplyTexture();

    void MoveLeft();
    void MoveRight();
    void MoveDown();
    void MoveUp();
    void Rotate();
};
