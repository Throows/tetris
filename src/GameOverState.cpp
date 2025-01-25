#include "GameOverState.hpp"

GameOverState::GameOverState(StatesContext& context, RessourceManager& ressources)
    : State(StateID::GAME_OVER, context, ressources)
    , m_gameover_text(State::GetFont(FontsID::CHEESE_MARKET), "Game Over !", 50)
{
    m_gameover_text.setFillColor(sf::Color::Red);

    m_background.setFillColor(sf::Color(0, 0, 0, 150));
}

void GameOverState::Init(sf::Vector2u window_size)
{
    m_gameover_text.setPosition({window_size.x / 2 - m_gameover_text.getGlobalBounds().size.x / 2,
                                 window_size.y / 2 - m_gameover_text.getGlobalBounds().size.y / 2});

    m_background.setSize({static_cast<float>(window_size.x), static_cast<float>(window_size.y)});
}

bool GameOverState::ProcessEvents(sf::Event &event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape){
            State::PopState(State::GetStateID());
            State::PopState(StateID::GAME);
            State::PushState(StateID::MAIN_MENU);
        }
    }
    return false; // Block the state behind
}

bool GameOverState::Update(sf::Time elapsed)
{
    (void)elapsed;
    return false; // Block the state behind
}

void GameOverState::Render(sf::RenderWindow &window)
{
    window.draw(m_background);
    window.draw(m_gameover_text);
}
