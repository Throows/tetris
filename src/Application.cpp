#include "Application.hpp"

Application::Application()
{
    m_window.create(sf::VideoMode({800, 600}), "Tetris - POO");
}

Application::~Application()
{
}

int Application::Run()
{
    Init();

    while (this->m_window.isOpen())
    {
        ProcessEvents();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}

void Application::Init()
{

}

void Application::ProcessEvents() 
{
    while (const std::optional event = this->m_window.pollEvent())
    {
        // Close window: exit
        if (event->is<sf::Event::Closed>()) {
            this->m_window.close();
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::Left) 
                m_tetris.MoveTetromino(Movement::LEFT);
            else if (keyPressed->code == sf::Keyboard::Key::Right)
                m_tetris.MoveTetromino(Movement::RIGHT);
            else if (keyPressed->code == sf::Keyboard::Key::Down)
                m_tetris.MoveTetromino(Movement::DOWN);
            else if (keyPressed->code == sf::Keyboard::Key::Up)
                m_tetris.MoveTetromino(Movement::ROTATE);
            else if (keyPressed->code == sf::Keyboard::Key::Escape)
                this->m_window.close();
        }
    }
}

void Application::Update()
{
    // Update the tetris
    sf::Time elapsed = this->m_clock.getElapsedTime();
    this->m_tetris.Update(elapsed);

    if (elapsed.asMilliseconds() > 800)
    {
        this->m_clock.restart();
    }
}

void Application::Render()
{
    this->m_window.clear(sf::Color::Black);
 
    this->m_tetris.Render(this->m_window);
 
    // Update the window
    this->m_window.display();
}

void Application::Shutdown()
{
}