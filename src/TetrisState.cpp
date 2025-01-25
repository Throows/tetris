#include "TetrisState.hpp"
#include <iostream>

#define ASSET_SIZE (384)

TetrisState::TetrisState(StatesContext& context, RessourceManager& ressource_manager)
    : State(StateID::GAME, context, ressource_manager)
    , game_over_text(State::GetFont(FontsID::CHEESE_MARKET), "Game Over", 50)
    , score_text(State::GetFont(FontsID::CHEESE_MARKET), "Score: 0", 20)
    , tetromino(TetrisState::CreateTetromino())
    , next_tetromino(TetrisState::CreateTetromino())
    , background(State::GetTexture(TexturesID::TETROMINO))
{
    this->background.setScale({this->SIZE / ASSET_SIZE, this->SIZE / ASSET_SIZE});
    this->background.setTextureRect(sf::IntRect(sf::Vector2i(static_cast<int>(TetrominoType::SHAPE_MAX) * ASSET_SIZE, 0),
                                                sf::Vector2i(ASSET_SIZE, ASSET_SIZE)));

    this->tetromino.SetActiveTetromino();

    this->game_over_text.setFillColor(sf::Color::Red);
    this->score_text.setFillColor(sf::Color::White);
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(seed);
}

void TetrisState::Init(sf::Vector2u window_size)
{
    this->score_text.setPosition({380, 300});
    sf::Vector2f position = {window_size.x / 2 - this->game_over_text.getGlobalBounds().size.x / 2,
                             window_size.y / 2 - this->game_over_text.getGlobalBounds().size.y / 2};
    this->game_over_text.setPosition(position);
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
            tetromino.Update();
        } else {
            this->fixed_tetrominos.push_back(tetromino);
            this->tetromino = this->next_tetromino;
            this->next_tetromino = TetrisState::CreateTetromino();
            this->tetromino.SetActiveTetromino();
            TetrisState::CheckLines();
            if (TetrisState::IsColliding(tetromino)) {
                this->fixed_tetrominos.pop_back();
                this->tetromino.ClearTetromino();
                this->next_tetromino.ClearTetromino();
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

// 375, 125
const sf::Vector2f next_tetromino_position[] = {
    {350, 100},
    {375, 100},
    {400, 100},
    {425, 100},
    {450, 100},
    {475, 100},
    {500, 100},
    {500, 125},
    {500, 150},
    {500, 175},
    {500, 200},
    {500, 225},
    {475, 225},
    {450, 225},
    {425, 225},
    {400, 225},
    {375, 225},
    {350, 225},
    {350, 200},
    {350, 175},
    {350, 150},
    {350, 125}
};

void TetrisState::Render(sf::RenderWindow &window)
{
    // Border of next Tetromino
    for (int i = 0; i < BOARD_WIDTH; i++) {
        for (int j = 0; j < BOARD_HEIGHT; j++) {
            this->background.setPosition({i * this->SIZE + this->m_board_position.x, 
                                          j * this->SIZE + m_board_position.y});
            window.draw(this->background);
        }
    }

    // Board background
    for (const auto &position : next_tetromino_position) {
        this->background.setPosition(position);
        window.draw(this->background);
    }

    // Fixed Tetromino
    for (auto &fixed_tetromino : this->fixed_tetrominos) {
        fixed_tetromino.Render(window, this->m_board_position); 
    }

    tetromino.Render(window, this->m_board_position);
    if (next_tetromino.GetType() == TetrominoType::BAR || next_tetromino.GetType() == TetrominoType::CUBE) {
        next_tetromino.Render(window, this->m_board_position + sf::Vector2f(SIZE/2, 0));
    } else {
        next_tetromino.Render(window, this->m_board_position);
    }
    window.draw(this->score_text);
}

bool TetrisState::IsColliding(Tetromino& new_tetromino)
{
    for (const Tetromino& fixed_tetromino : fixed_tetrominos) {   
        if (new_tetromino.IsColliding(fixed_tetromino)) {
            return true;
        }
    }
    return false;
}

void TetrisState::CheckLines()
{
    uint8_t block_per_lines[BOARD_HEIGHT] = {0};

    for (const Tetromino& fixed_tetromino : fixed_tetrominos) {
        for (const sf::Vector2i& position : fixed_tetromino.GetAbsoluteCoordinates()) {
            block_per_lines[position.y]++;
        }
    }

    uint8_t lines_full = 0;
    for (uint8_t y = 0; y < BOARD_HEIGHT; y++)
    {
        if (block_per_lines[y] == BOARD_WIDTH)
        {
            lines_full++;
            for (Tetromino& fixed_tetromino : fixed_tetrominos) {
                fixed_tetromino.RemoveLine(y);
                fixed_tetromino.MovePartsDown(y);
            } 
        }
    }
    this->fixed_tetrominos.erase(
                std::remove_if(this->fixed_tetrominos.begin(),
                            this->fixed_tetrominos.end(),
                            [](Tetromino fixed_tetromino) {
                                return fixed_tetromino.IsEmpty();
                            }),
                            this->fixed_tetrominos.end());
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

    return !TetrisState::IsColliding(next_position) && !next_position.IsOutOfBoard(BOARD_WIDTH, BOARD_HEIGHT);
}

Tetromino TetrisState::CreateTetromino()
{
    std::uniform_int_distribution<int> uniform_dist_type(TetrominoType::BAR, TetrominoType::S_SHAPE);
    return Tetromino(State::GetTexture(TexturesID::TETROMINO),
                        {15, 5},
                        static_cast<TetrominoType>(uniform_dist_type(r)),
                        SIZE);
}