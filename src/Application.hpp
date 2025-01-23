#pragma once

#include <spdlog/spdlog.h>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Tetris.hpp"

class Application
{
public:
    Application();
    ~Application();

    int Run();

private:
    sf::RenderWindow m_window;
    Tetris m_tetris;
    sf::Clock m_clock;
    sf::Time m_elapsed;

    void Init();
    void ProcessEvents();
    void Update(sf::Time elapsed);
    void Render();
    void Shutdown();
};


