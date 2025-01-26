#include "Application.hpp"
#include "MenuState.hpp"
#include "TetrisState.hpp"
#include "PauseState.hpp"
#include "GameOverState.hpp"

Application::Application()
{
    m_window.create(sf::VideoMode({600, 600}), "Tetris - The Game", sf::Style::Titlebar, sf::State::Windowed);
    m_window.setFramerateLimit(60);
    Application::Init();
}

Application::~Application()
{
}

int Application::Run()
{
    while (this->m_window.isOpen())
    {
        sf::Time elapsed = this->m_clock.restart();
        ProcessEvents();
        Update(elapsed);
        Render();
    }

    Shutdown();
    return 0;
}

void Application::Init()
{
    Application::InitAssets();
    Application::InitStates();
    // Load First State
    auto stateFactory = this->m_states_context.states_map.at(StateID::MAIN_MENU);
    this->m_states_context.states_vec.push_back(stateFactory());
}

void Application::ProcessEvents() 
{
    while (const std::optional event = this->m_window.pollEvent())
    {
        // Close window: exit
        if (event->is<sf::Event::Closed>()) {
            this->m_window.close();
        } 
        if (event.has_value()) {
            sf::Event event_type = event.value();
            
            for (auto it = this->m_states_context.states_vec.rbegin(); it != this->m_states_context.states_vec.rend(); ++it) {
                if (!(*it)->ProcessEvents(event_type)) 
                    break;
            }
        }
    }
    Application::UpdateStates();
}

void Application::Update(sf::Time elapsed)
{
    for (auto it = this->m_states_context.states_vec.rbegin(); it != this->m_states_context.states_vec.rend(); ++it) {
        if (!(*it)->Update(elapsed))
            break;
    }
    Application::UpdateStates();
}

void Application::UpdateStates()
{
    for (auto& state_in_change : this->m_states_context.states_changes) {
        if (state_in_change.action == StatesContext::StateChange::Action::PUSH) {
            auto stateFactory = this->m_states_context.states_map.at(state_in_change.state_id);
            this->m_states_context.states_vec.push_back(stateFactory());
        } else if (state_in_change.action == StatesContext::StateChange::Action::POP) {
            this->m_states_context.states_vec.erase(
                std::remove_if(this->m_states_context.states_vec.begin(),
                            this->m_states_context.states_vec.end(),
                            [state_in_change](std::unique_ptr<State>& state) {
                                return state.get()->GetStateID() == state_in_change.state_id;
                            }),
                            this->m_states_context.states_vec.end());
        }
    }
    this->m_states_context.states_changes.clear();
    if (this->m_states_context.states_vec.empty()) {
        this->m_window.close();
    }
}

void Application::Render()
{
    this->m_window.clear(sf::Color::Black);
 
    for (auto& state : this->m_states_context.states_vec) {
        state->Render(this->m_window);
    }
 
    // Update the window
    this->m_window.display();
}

void Application::Shutdown()
{
}

void Application::InitAssets()
{
    this->m_ressource_manager.LoadTexture(TexturesID::TETROMINO, "assets/image/tetrominos.png");
    this->m_ressource_manager.LoadFont(FontsID::CHEESE_MARKET, "assets/font/CheeseMarket.ttf");
}

void Application::InitStates()
{
    this->m_states_context.states_map.emplace(StateID::MAIN_MENU,[this]() { 
            auto state = std::make_unique<MenuState>(this->m_states_context, this->m_ressource_manager);
            state->Init(this->m_window.getSize());
            return state;
        });
    this->m_states_context.states_map.emplace(StateID::GAME,[this]() { 
            return std::make_unique<TetrisState>(this->m_states_context, this->m_ressource_manager);
        });
    
    this->m_states_context.states_map.emplace(StateID::PAUSE,[this]() { 
            auto state = std::make_unique<PauseState>(this->m_states_context, this->m_ressource_manager);
            state->Init(this->m_window.getSize());
            return state;
        });

    this->m_states_context.states_map.emplace(StateID::GAME_OVER,[this]() { 
            auto state = std::make_unique<GameOverState>(this->m_states_context, this->m_ressource_manager);
            state->Init(this->m_window.getSize());
            return state;
        });
}
