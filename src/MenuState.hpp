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
    bool ProcessEvents(sf::Event& event) override;
    bool Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
    static constexpr float SIZE = 25.0f;

    sf::Text m_title;
    sf::Sprite tetromono_sprite;
    std::vector<Tetromino> m_menu_tetrominos;
    std::vector<Button> m_buttons;

    sf::IntRect GetTextureRect(TetrominoType type);
};