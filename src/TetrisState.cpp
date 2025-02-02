#include "TetrisState.hpp"
#include <iostream>
#include <fstream>

TetrisState::TetrisState(StatesContext& context, RessourceManager& ressource_manager, sf::RenderWindow& window)
    : State(StateID::GAME, context, ressource_manager)
    , SIZE(window.getSize().y / 20.0f) // Why ?
    , m_game(ressource_manager)
    , m_score_text(State::GetFont(FontsID::CHEESE_MARKET))
    , m_tetromino_sprite(State::GetTexture(TexturesID::TETROMINO))
{
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> uniform_dist_type(TetrominoType::BAR, TetrominoType::SHAPE_MAX-1);
    for (int i = 0; i < NEXT_TETROMINO_COUNT; i++) {
        this->falling_tetrominos[i] = static_cast<TetrominoType>(uniform_dist_type(gen));
    }

    this->m_game.InitGame(this->BOARD_WIDTH, this->BOARD_HEIGHT, this->SIZE);

    this->m_tetromino_sprite.setScale(sf::Vector2f{this->SIZE / ASSET_SIZE, this->SIZE / ASSET_SIZE});

    this->m_game.NewTetromino(TetrisState::GetRandomTetromino()); // Set Active Tetromino
    this->m_next_tetromino.SetType(TetrisState::GetRandomTetromino());
    this->m_next_tetromino.SetRotation(Rotation::ROTATION_0);
    this->m_next_tetromino.SetCoordinates({BOARD_WIDTH + 7 , 8 });

    this->m_score_text.setString("Score: " + std::to_string(this->m_game.GetScore()));
    this->m_score_text.setFillColor(sf::Color::White);
    this->m_score_text.setPosition({ this->SIZE * 16 , this->SIZE * 13 });

    ActionHistory action {
        .type = ActionHistory::ActionType::NEW_PIECE,
        .block_type = this->m_game.GetTetromino().GetType()
    };
    this->m_action_history.push_back(action);

}

void TetrisState::MoveTetromino(Movement direction)
{
    this->m_game.MoveTetromino(direction);
    ActionHistory action{
        .type = ActionHistory::ActionType::MOVEMENT,
        .move = direction,
    };
    this->m_action_history.push_back(action);
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
    if (!this->m_game.IsGameOver() && this->elapsed_time >= this->speed_time) {
        bool new_tetromino = this->m_game.UpdateBoard();
        if (!this->m_game.IsGameOver() && !new_tetromino) {
            ActionHistory action {
                .type = ActionHistory::ActionType::MOVEMENT,
                .move = Movement::DOWN,
            };
            this->m_action_history.push_back(action);
        } else if (this->m_game.IsGameOver()) {
            State::PushState(StateID::GAME_OVER);
            TetrisState::SaveActions();
        } else if (new_tetromino) {
            this->m_game.NewTetromino(this->m_next_tetromino.GetType());
            this->m_next_tetromino.SetType(TetrisState::GetRandomTetromino());
            this->m_next_tetromino.SetRotation(Rotation::ROTATION_0);
            this->m_next_tetromino.SetCoordinates({BOARD_WIDTH + 7 , 8 });
            ActionHistory action {
                .type = ActionHistory::ActionType::NEW_PIECE,
                .block_type = this->m_game.GetTetromino().GetType()
            };
            this->m_action_history.push_back(action);
        }
        this->m_score_text.setString("Score: " + std::to_string(this->m_game.GetScore()));
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
    this->m_game.RenderBoard(this->m_board_position, this->SIZE, window);

    // Border of next Tetromino
    this->m_tetromino_sprite.setTextureRect(m_game.GetTextureRect(TetrominoType::SHAPE_MAX));
    for (auto &position : tetromino_border_grid) {
        this->m_tetromino_sprite.setPosition((sf::Vector2f{BOARD_WIDTH + 7 , 8} + sf::Vector2f(position)) * this->SIZE);
        window.draw(this->m_tetromino_sprite);
    }

    // Draw the next Tetromino
    TetrominoType type = this->m_next_tetromino.GetType();
    this->m_tetromino_sprite.setTextureRect(this->m_game.GetTextureRect(type));
    for (auto &position : this->m_next_tetromino.GetAbsoluteCoordinates()) { 
        sf::Vector2f tetrominoPos = (sf::Vector2f(position) * this->SIZE);
        if (type == TetrominoType::CUBE) {
            tetrominoPos += sf::Vector2f(SIZE/2, -1);
        } else if (type == TetrominoType::BAR) {
            tetrominoPos += sf::Vector2f(SIZE/2, -1-SIZE/2);
        }
        this->m_tetromino_sprite.setPosition(tetrominoPos);
        window.draw(this->m_tetromino_sprite);
    }
    window.draw(this->m_score_text);
}

void TetrisState::SaveActions()
{
    std::ofstream file("game.replay");
    for (const auto& action : this->m_action_history)
        file << action << std::endl;

    file.close();
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

std::ostream &operator<<(std::ostream &os, const ActionHistory &action)
{
    if (action.type == ActionHistory::ActionType::MOVEMENT) {
        os << "M:" << action.move;
    } else if (action.type == ActionHistory::ActionType::NEW_PIECE) {
        os << "P:" << action.block_type;
    }
    return os;
}
