#include "MenuState.hpp"
#include <iostream>

MenuState::MenuState(StatesContext& context, RessourceManager& ressource_manager) 
    : State(StateID::MAIN_MENU, context, ressource_manager)
    , m_title(State::GetFont(FontsID::CHEESE_MARKET), "Tetris - Le Jeu", sf::Text::Style::Bold)
    , tetromono_sprite(State::GetTexture(TexturesID::TETROMINO))
{
    this->tetromono_sprite.setScale({this->SIZE / ASSET_SIZE, this->SIZE / ASSET_SIZE});
    m_title.setCharacterSize(45);
    m_title.setFillColor(sf::Color::White);
    m_menu_tetrominos.resize(7);
}

void MenuState::Init(sf::Vector2u window_size)
{
    sf::Vector2f position = {(window_size.x * 2) / 3 - (this->m_title.getGlobalBounds().size.x * 2) / 3,
                             window_size.y / 5 - this->m_title.getGlobalBounds().size.y / 5};
    this->m_title.setPosition(position);

    position += {50, 100};

    auto start_btn = Button(State::GetFont(FontsID::CHEESE_MARKET));
    start_btn.SetSize({200, 50});
    start_btn.SetText("Start");
    start_btn.SetPosition(position);
    start_btn.SetIdleColor(sf::Color::Green);
    start_btn.SetHoverColor(sf::Color::Red);
    start_btn.SetActiveColor(sf::Color::Blue);
    start_btn.SetCallback([this](){
        PushState(StateID::GAME);
        PopState(State::GetStateID());
    });
    m_buttons.push_back(start_btn);

    // TODO: (Later on) Make a replay file and play the game in the menu
    m_menu_tetrominos[0].SetType(TetrominoType::BAR);
    m_menu_tetrominos[0].SetCoordinates({2, static_cast<int>((window_size.y-(0*SIZE))/SIZE)});
    m_menu_tetrominos[0].SetRotation(Rotation::ROTATION_0);

    m_menu_tetrominos[1].SetType(TetrominoType::T_SHAPE);
    m_menu_tetrominos[1].SetCoordinates({0, static_cast<int>((window_size.y-(2*SIZE))/SIZE)});
    m_menu_tetrominos[1].SetRotation(Rotation::ROTATION_90);

    m_menu_tetrominos[2].SetType(TetrominoType::J_SHAPE);
    m_menu_tetrominos[2].SetCoordinates({2, static_cast<int>((window_size.y-(2*SIZE))/SIZE)});
    m_menu_tetrominos[2].SetRotation(Rotation::ROTATION_270);

    m_menu_tetrominos[3].SetType(TetrominoType::CUBE);
    m_menu_tetrominos[3].SetCoordinates({4, static_cast<int>((window_size.y-(1*SIZE))/SIZE)});
    m_menu_tetrominos[3].SetRotation(Rotation::ROTATION_0);

    m_menu_tetrominos[4].SetType(TetrominoType::S_SHAPE);
    m_menu_tetrominos[4].SetCoordinates({0, static_cast<int>((window_size.y-(4*SIZE))/SIZE)});
    m_menu_tetrominos[4].SetRotation(Rotation::ROTATION_90);

    m_menu_tetrominos[5].SetType(TetrominoType::J_SHAPE);
    m_menu_tetrominos[5].SetCoordinates({5, static_cast<int>((window_size.y-(1*SIZE))/SIZE)});
    m_menu_tetrominos[5].SetRotation(Rotation::ROTATION_270);

    m_menu_tetrominos[6].SetType(TetrominoType::Z_SHAPE);
    m_menu_tetrominos[6].SetCoordinates({3, static_cast<int>((window_size.y-(3*SIZE))/SIZE)});
    m_menu_tetrominos[6].SetRotation(Rotation::ROTATION_0);
}

bool MenuState::ProcessEvents(sf::Event &event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            PopState(State::GetStateID());
        } else if (keyPressed->code == sf::Keyboard::Key::Space) {
            PushState(StateID::GAME);
            PopState(State::GetStateID());
        }
    }
    for (auto& button : m_buttons) {
        button.ProcessEvents(event);
    }
    return true;
}

bool MenuState::Update(sf::Time elapsed)
{
    for (auto& button : m_buttons) {
        button.Update(elapsed);
    }
    return true;
}

void MenuState::Render(sf::RenderWindow &window)
{
    window.clear(sf::Color(0xC7, 0x6E, 0x00));
    window.draw(this->m_title);

    for (auto& tetromino : m_menu_tetrominos) {
        this->tetromono_sprite.setTextureRect(GetTextureRect(tetromino.GetType()));
        for (auto &position : tetromino.GetAbsoluteCoordinates()) {
            this->tetromono_sprite.setPosition((sf::Vector2f(position) * this->SIZE));
            window.draw(this->tetromono_sprite);
        }
    }

    for (auto& button : m_buttons) {
        button.Render(window);
    }
}

sf::IntRect MenuState::GetTextureRect(TetrominoType type)
{
    return sf::IntRect(sf::Vector2i(static_cast<int>(type) * ASSET_SIZE, 0), sf::Vector2i(ASSET_SIZE, ASSET_SIZE));
}
