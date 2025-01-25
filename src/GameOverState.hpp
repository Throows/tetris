#pragma once

#include "State.hpp"
#include "Button.hpp"

class GameOverState : public State
{
public:
    GameOverState(StatesContext& context, RessourceManager& ressources);

    void Init(sf::Vector2u window_size);

    bool ProcessEvents(sf::Event& event) override;
    bool Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
    sf::Text m_gameover_text;
    std::vector<Button> m_buttons;
    sf::RectangleShape m_background;
};