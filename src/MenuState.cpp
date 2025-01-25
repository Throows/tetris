#include "MenuState.hpp"
#include <iostream>

MenuState::MenuState(StatesContext& context, RessourceManager& ressource_manager) 
    : State(StateID::MAIN_MENU, context, ressource_manager)
    , m_title(State::GetFont(FontsID::CHEESE_MARKET), "Tetris (POO)")
{
    m_title.setCharacterSize(45);
    m_title.setFillColor(sf::Color::White);

    auto start_btn = Button(ressource_manager);
    start_btn.SetSize({200, 50});
    start_btn.SetText("Start");
    start_btn.SetPosition({250, 250});
    start_btn.SetIdleColor(sf::Color::Green);
    start_btn.SetHoverColor(sf::Color::Red);
    start_btn.SetActiveColor(sf::Color::Blue);
    start_btn.SetCallback([this](){
        PushState(StateID::GAME);
        PopState();
    });
    m_buttons.push_back(start_btn);
}

void MenuState::Init(sf::Vector2u window_size)
{
    sf::Vector2f position = {(window_size.x * 2) / 3 - (this->m_title.getGlobalBounds().size.x * 2) / 3,
                             window_size.y / 5 - this->m_title.getGlobalBounds().size.y / 5};
    this->m_title.setPosition(position);

    // TODO: (Later on) Make a replay file and play the game in the menu
    sf::Texture& texture = State::GetTexture(TexturesID::TETROMINO);
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_0, {1, 550/25}, TetrominoType::BAR)); // TODO: Investigate why 0 coordinate is outside
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_270, {1, 475/25}, TetrominoType::T_SHAPE));
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_90, {1, 475/25}, TetrominoType::J_SHAPE));
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_0, {3, 500/25}, TetrominoType::CUBE));
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_90, {0, 425/25}, TetrominoType::S_SHAPE));
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_90, {4, 500/25}, TetrominoType::J_SHAPE));
    m_menu_tetrominos.push_back(Tetromino(texture, Rotation::ROTATION_0, {3, 450/25}, TetrominoType::Z_SHAPE));
}

void MenuState::ProcessEvents(sf::Event &event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape) {
            PopState();
        } else if (keyPressed->code == sf::Keyboard::Key::Space) {
            PushState(StateID::GAME);
            PopState();
        }
    }
    for (auto& button : m_buttons) {
        button.ProcessEvents(event);
    }
}

void MenuState::Update(sf::Time elapsed)
{
    for (auto& button : m_buttons) {
        button.Update(elapsed);
    }
}

void MenuState::Render(sf::RenderWindow &window)
{
    window.clear(sf::Color(0xC7, 0x6E, 0x00));
    window.draw(this->m_title);

    for (auto& tetromino : m_menu_tetrominos) {
        tetromino.Render(window);
    }

    for (auto& button : m_buttons) {
        button.Render(window);
    }
}
