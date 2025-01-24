#include "MenuState.hpp"
#include <iostream>

MenuState::MenuState(StatesContext& context) 
    : State(StateID::MAIN_MENU, context)
{
}

void MenuState::ProcessEvents(sf::Event &event)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Space) {
            PushState(StateID::GAME);
            PopState();
        }
    }
}

void MenuState::Update(sf::Time elapsed)
{
    (void)elapsed;
}

void MenuState::Render(sf::RenderWindow &window)
{
    (void)window;
}
