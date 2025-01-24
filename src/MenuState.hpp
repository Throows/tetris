#pragma once

#include "State.hpp"

class MenuState : public State
{
public:
    MenuState(StatesContext& context);
    ~MenuState() {};

    void ProcessEvents(sf::Event& event) override;
    void Update(sf::Time elapsed) override;
    void Render(sf::RenderWindow &window) override;

private:
};