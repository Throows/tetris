#pragma once 

#include <SFML/Graphics.hpp>
#include <map>

enum TetrominoType : int
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

enum Rotation : int {
    ROTATION_0      = 0,
    ROTATION_90     = 1,
    ROTATION_180    = 2,
    ROTATION_270    = 3
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
    Tetromino(const sf::Texture& texture, sf::Vector2i coordinates, TetrominoType type, float size = 25.0f,  Rotation rotation = Rotation::ROTATION_0);
    Tetromino(const Tetromino &tetromino);
    ~Tetromino() = default;

    void Update();
    void Render(sf::RenderWindow &window, sf::Vector2f position = {0, 0});

    bool Move(Movement movement);
    bool Revert(Movement movement);

    bool SetActiveTetromino();
    void ClearTetromino() { m_relative_coordinates.clear(); };

    TetrominoType GetType() const { return m_type; };
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
    sf::Vector2i m_coordinates;                         // Coordinate of next block
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
