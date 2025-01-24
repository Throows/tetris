#pragma once

#include <spdlog/spdlog.h>
#include <functional>
#include <map>
#include "State.hpp"

class Application
{
public:
    Application();
    ~Application();

    int Run();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    sf::Time m_elapsed;

    StatesContext m_states_context;

    void Init();
    void ProcessEvents();
    void Update(sf::Time elapsed);
    void UpdateStates();
    void Render();
    void Shutdown();
};


