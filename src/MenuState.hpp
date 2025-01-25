#pragma once
#include "State.hpp"
#include "Tetromino.hpp"
#include "Button.hpp"

class MenuState : public State
{
public:
    MenuState(StatesContext& context, RessourceManager& ressource_manager);
    ~MenuState() {};

    void Init(sf::Vector2u window_size);
    void ProcessEvents(sf::Event& event) override;
    void Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
    sf::Text m_title;
    std::vector<Tetromino> m_menu_tetrominos;
    std::vector<Button> m_buttons;
};