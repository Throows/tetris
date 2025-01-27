#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

#include "RessourceManager.hpp"

enum ButtonState
{
    IDLE = 0,
    HOVER,
    ACTIVE
};

class Button
{
public:
    Button(sf::Font& font) : m_text(font) {}
    ~Button() = default;

    void SetPosition(sf::Vector2f position);
    void SetSize(sf::Vector2f size) { m_shape.setSize(size); }
    void SetText(const std::string& text) { m_text.setString(text); }
    void SetCallback(std::function<void()> callback) { m_callback = std::move(callback); }
    void SetIdleColor(sf::Color color) { m_idle_color = color; }
    void SetHoverColor(sf::Color color) { m_hover_color = color; }
    void SetActiveColor(sf::Color color) { m_active_color = color; }

    void ProcessEvents(sf::Event& event);
    void Update(sf::Time elapsed);
    void Render(sf::RenderWindow& window);

    void Hover();
    void Unhover();
    void Click();
    void UnClick();

private:
    ButtonState m_state = ButtonState::IDLE;
    std::function<void()> m_callback;
    sf::Text m_text;
    sf::RectangleShape m_shape;
    sf::Color m_idle_color;
    sf::Color m_hover_color;
    sf::Color m_active_color;

    bool state_changed = true;
};