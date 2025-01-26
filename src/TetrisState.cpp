#include "TetrisState.hpp"
#include <iostream>

TetrisState::TetrisState(StatesContext& context, RessourceManager& ressource_manager, sf::RenderWindow& window)
    : State(StateID::GAME, context, ressource_manager)
    , SIZE(window.getSize().y / 20)
    , tetromono_sprite(State::GetTexture(TexturesID::TETROMINO))
    , score_text(State::GetFont(FontsID::CHEESE_MARKET), "Score: 0", 20)
{

    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            this->m_board[i + j * BOARD_WIDTH] = TetrominoType::SHAPE_MAX;
        }
    }

    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> uniform_dist_type(TetrominoType::BAR, TetrominoType::SHAPE_MAX-1);
    for (int i = 0; i < NEXT_TETROMINO_COUNT; i++) {
        this->falling_tetrominos[i] = static_cast<TetrominoType>(uniform_dist_type(gen));
    }
    this->tetromino.SetType(TetrisState::GetRandomTetromino());
    this->tetromino.SetRotation(Rotation::ROTATION_0);
    this->tetromino.SetActiveTetromino();
    this->next_tetromino.SetType(TetrisState::GetRandomTetromino());
    this->next_tetromino.SetRotation(Rotation::ROTATION_0);

    this->tetromono_sprite.setScale({this->SIZE / ASSET_SIZE, this->SIZE / ASSET_SIZE});
    this->tetromino.SetActiveTetromino();
    this->score_text.setFillColor(sf::Color::White);
    this->score_text.setPosition({ this->SIZE * 16 , this->SIZE * 13 });
}

void TetrisState::MoveTetromino(Movement direction)
{
    if (direction == Movement::BOTTOM) {
        uint8_t movement = 0;
        while (TetrisState::CanTetrominoMove(Movement::DOWN)) {
            tetromino.Move(direction);
            movement++;
        }
        score += movement + 1;
    } else {
        if (TetrisState::CanTetrominoMove(direction)) {
            tetromino.Move(direction);
        }
    } 
}

bool TetrisState::ProcessEvents(sf::Event &event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Left) 
                TetrisState::MoveTetromino(Movement::LEFT);
        else if (keyPressed->code == sf::Keyboard::Key::Right)
                TetrisState::MoveTetromino(Movement::RIGHT);
        else if (keyPressed->code == sf::Keyboard::Key::Down)
                TetrisState::MoveTetromino(Movement::DOWN);
        else if (keyPressed->code == sf::Keyboard::Key::Up)
                TetrisState::MoveTetromino(Movement::ROTATE);
        else if (keyPressed->code == sf::Keyboard::Key::Space)
                TetrisState::MoveTetromino(Movement::BOTTOM);
        else if (keyPressed->code == sf::Keyboard::Key::Escape){
            State::PushState(StateID::PAUSE);
        }
    }
    return true;
}

bool TetrisState::Update(sf::Time elapsed)
{
    this->elapsed_time += elapsed;
    if (this->elapsed_time > this->speed_time) {
        if (TetrisState::CanTetrominoMove(Movement::DOWN)) {
            tetromino.Move(Movement::DOWN);
        } else {
            TetrisState::FixTetromino();
            this->tetromino.SetType(this->next_tetromino.GetType());
            this->tetromino.SetRotation(Rotation::ROTATION_0);
            this->tetromino.SetActiveTetromino();
            this->next_tetromino.SetType(TetrisState::GetRandomTetromino());
            this->next_tetromino.SetRotation(Rotation::ROTATION_0);
            TetrisState::CheckLines();
            if (!TetrisState::CanTetrominoMove(Movement::DOWN)) {
                this->game_over = true;
                State::PushState(StateID::GAME_OVER);
            }
        }
        this->score_text.setString("Score: " + std::to_string(score));
        this->elapsed_time = sf::Time::Zero;
        this->update_number++;
    }

    if ((this->update_number * this->speed_time.asSeconds()) >= 60.0 && this->speed_time > sf::milliseconds(200)) {
        this->speed_time -= sf::milliseconds(50);
        this->update_number = 0;
    }
    return true;
}

// 7*6 Rectangle around the Tetromino
const sf::Vector2f tetromino_border_grid[] = {
    {-3, -3},{-2, -3},{-1, -3},{0, -3},{1, -3},{2, -3},{3, -3},
    {3, -2},{3, -1},{3, 0},{3, 1},{3, 2},
    {2, 2},{1, 2},{0, 2},{-1, 2},{-2, 2},{-3, 2},
    {-3, 1},{-3, 0},{-3, -1},{-3, -2}
};

void TetrisState::Render(sf::RenderWindow &window)
{
    // Draw the board
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            this->tetromono_sprite.setTextureRect(GetTextureRect(m_board[i + j * BOARD_WIDTH]));
            sf::Vector2f pos = this->m_board_position + sf::Vector2f(i * this->SIZE, j * this->SIZE);
            this->tetromono_sprite.setPosition(pos);
            window.draw(this->tetromono_sprite);
        }
    }

    // Border of next Tetromino
    this->tetromono_sprite.setTextureRect(GetTextureRect(TetrominoType::SHAPE_MAX));
    for (auto &position : tetromino_border_grid) {
        this->tetromono_sprite.setPosition(this->m_next_tetromino_position + (sf::Vector2f(position) * this->SIZE));
        window.draw(this->tetromono_sprite);
    }

    // Draw the Tetromino
    this->tetromono_sprite.setTextureRect(GetTextureRect(this->tetromino.GetType()));
    for (auto &position : tetromino.GetAbsoluteCoordinates()) {
        this->tetromono_sprite.setPosition(this->m_board_position + (sf::Vector2f(position) * this->SIZE));
        window.draw(this->tetromono_sprite);
    }

    // Draw the next Tetromino
    this->tetromono_sprite.setTextureRect(GetTextureRect(this->next_tetromino.GetType()));
    for (auto &position : next_tetromino.GetAbsoluteCoordinates()) { 
        if (next_tetromino.GetType() == TetrominoType::BAR || next_tetromino.GetType() == TetrominoType::CUBE) { 
            this->tetromono_sprite.setPosition(this->m_next_tetromino_position + (sf::Vector2f(position) * this->SIZE) + sf::Vector2f(SIZE/2, 0));
        } else {
            this->tetromono_sprite.setPosition(this->m_next_tetromino_position + (sf::Vector2f(position) * this->SIZE));
        }
        window.draw(this->tetromono_sprite);
    }
    window.draw(this->score_text);
}

bool TetrisState::IsColliding(Tetromino& new_tetromino)
{
    for (auto &position : new_tetromino.GetAbsoluteCoordinates()) {
        if (position.x < 0 
        || position.x >= BOARD_WIDTH 
        || position.y >= BOARD_HEIGHT
        || this->m_board[position.x + position.y * BOARD_WIDTH] != TetrominoType::SHAPE_MAX) {
            return true;
        }
    }
    return false;
}

void TetrisState::CheckLines()
{
    uint8_t lines_full = 0;
    for (int y = (BOARD_HEIGHT-1); y > 0; y--) { 
        bool is_line_full = true;
        for (int x = 0; x < BOARD_WIDTH; x++) {
            if (this->m_board[x + y * BOARD_WIDTH] == TetrominoType::SHAPE_MAX) {
                is_line_full = false;
                break;
            }
        }
        if (is_line_full) {
            if (this->m_board[y * BOARD_WIDTH] != TetrominoType::SHAPE_MAX) lines_full++;
            for (int new_y = y; new_y > 0; new_y--) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    this->m_board[x + new_y * BOARD_WIDTH] = this->m_board[x + (new_y-1) * BOARD_WIDTH];
                }
            }
            y++; // Check the same line again as it has been moved down
        }
    }
    TetrisState::UpdateScore(lines_full);
}

void TetrisState::UpdateScore(uint8_t lines)
{
    if (lines == 1) {
        score += 40;
    } else if (lines == 2) {
        score += 100;
    } else if (lines == 3) {
        score += 300;
    } else if (lines == 4) {
        score += 1200;
    }
}

bool TetrisState::CanTetrominoMove(Movement movement)
{
    Tetromino next_position = tetromino;
    next_position.Move(movement);
    return !TetrisState::IsColliding(next_position);
}

void TetrisState::FixTetromino()
{
    for (auto &position : tetromino.GetAbsoluteCoordinates()) {
        assert(this->m_board[position.x + position.y * BOARD_WIDTH] == TetrominoType::SHAPE_MAX);
        this->m_board[position.x + position.y * BOARD_WIDTH] = tetromino.GetType();
    }
}

sf::IntRect TetrisState::GetTextureRect(TetrominoType type)
{
    return sf::IntRect(sf::Vector2i(static_cast<int>(type) * ASSET_SIZE, 0), sf::Vector2i(ASSET_SIZE, ASSET_SIZE));
}

TetrominoType TetrisState::GetRandomTetromino()
{
    TetrominoType type = this->falling_tetrominos[0];
    std::uniform_int_distribution<int> uniform_dist_type(TetrominoType::BAR, TetrominoType::SHAPE_MAX-1); 
    //TODO: Implement weighted random distribution

    for (int i = 0; i < NEXT_TETROMINO_COUNT - 1; i++) {
        this->falling_tetrominos[i] = this->falling_tetrominos[i+1];
    }
    this->falling_tetrominos[NEXT_TETROMINO_COUNT - 1] = static_cast<TetrominoType>(uniform_dist_type(r));

    return type;
}
