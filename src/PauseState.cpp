#include "PauseState.hpp"

PauseState::PauseState(StatesContext& context, RessourceManager& ressources)
    : State(StateID::PAUSE, context, ressources)
    , m_paused_text(State::GetFont(FontsID::CHEESE_MARKET), "Paused", 50)
{
    m_paused_text.setFillColor(sf::Color::White);

    m_background.setFillColor(sf::Color(0, 0, 0, 150));
}

void PauseState::Init(sf::Vector2u window_size)
{
    m_paused_text.setPosition({window_size.x / 2 - m_paused_text.getGlobalBounds().size.x / 2,
                               window_size.y / 2 - m_paused_text.getGlobalBounds().size.y / 2});

    m_background.setSize({static_cast<float>(window_size.x), static_cast<float>(window_size.y)});
}

bool PauseState::ProcessEvents(sf::Event &event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Escape){
            State::PopState(State::GetStateID());
            State::PopState(StateID::GAME);
            State::PushState(StateID::MAIN_MENU);
        } else if (keyPressed->code == sf::Keyboard::Key::Space) {
            State::PopState(State::GetStateID());
        }
    }
    return false; // Block the state behind
}

bool PauseState::Update(sf::Time elapsed)
{
    (void)elapsed;
    return false; // Block the state behind
}

void PauseState::Render(sf::RenderWindow &window)
{
    window.draw(m_background);
    window.draw(m_paused_text);
}
