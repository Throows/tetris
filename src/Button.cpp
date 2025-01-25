#include "Button.hpp"
#include <spdlog/spdlog.h>

const char* to_string(ButtonState state)
{
    switch (state)
    {
    case ButtonState::IDLE:
        return "IDLE";
    case ButtonState::HOVER:
        return "HOVER";
    case ButtonState::ACTIVE:
        return "ACTIVE";
    default:
        return "UNKNOWN";
    }
}

void Button::Render(sf::RenderWindow & window)
{
    window.draw(m_shape);
    window.draw(m_text);
}

void Button::SetPosition(sf::Vector2f position)
{
    m_shape.setPosition(position);
    m_text.setPosition(position);
    m_text.move({m_shape.getSize().x / 2 - m_text.getGlobalBounds().size.x / 2, m_shape.getSize().y / 2 - m_text.getGlobalBounds().size.y});
}

void Button::ProcessEvents(sf::Event &event)
{
    if (const sf::Event::MouseMoved* mouse_move_event = event.getIf<sf::Event::MouseMoved>()) {
        if (m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_move_event->position))) {
            if (m_state == ButtonState::IDLE) {
               Hover(); 
            }
        }
        else if (m_state == ButtonState::HOVER) {
            Unhover();
        }
    }
    else if (const sf::Event::MouseButtonPressed* mouse_press_event = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mouse_press_event->button == sf::Mouse::Button::Left 
        && m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_press_event->position))
        && m_state == ButtonState::HOVER) {
            Click();
        }
    }
    else if (const sf::Event::MouseButtonReleased* mouse_release_event = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouse_release_event->button == sf::Mouse::Button::Left 
        && m_shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_release_event->position))
        && m_state == ButtonState::ACTIVE) {
            UnClick();
        }
    }
}

void Button::Update(sf::Time elapsed)
{
    (void)elapsed;
    if (!this->state_changed) return;

    switch (this->m_state)
    {
    case ButtonState::IDLE:
        this->m_shape.setFillColor(this->m_idle_color);
        break;
    case ButtonState::HOVER:
        this->m_shape.setFillColor(this->m_hover_color);
        break;
    case ButtonState::ACTIVE:
        this->m_shape.setFillColor(this->m_active_color);
        break;
    default:
        break;
    }
    this->state_changed = false;
}

void Button::Hover()
{
    this->m_state = ButtonState::HOVER;
    this->state_changed = true;
}

void Button::Unhover()
{
    this->m_state = ButtonState::IDLE;
    this->state_changed = true;
}

void Button::Click()
{
    this->m_state = ButtonState::ACTIVE;
    this->state_changed = true;
}

void Button::UnClick()
{
    this->m_state = ButtonState::IDLE;
    this->state_changed = true;
    this->m_callback();
}
