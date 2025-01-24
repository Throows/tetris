#include "Application.hpp"
#include "MenuState.hpp"
#include "TetrisState.hpp"

Application::Application()
{
    m_window.create(sf::VideoMode({550, 550}), "Tetris - The Game");
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
    this->m_states_context.states_map.emplace(StateID::MAIN_MENU,[this]() { 
            return std::make_unique<MenuState>(this->m_states_context);
        });
    this->m_states_context.states_map.emplace(StateID::GAME,[this]() { 
            auto state = std::make_unique<TetrisState>(this->m_states_context);
            state->Init(this->m_window.getSize());
            return state;
        });

    //this->m_tetris.Init(this->m_window.getSize());
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
            for (auto& state : this->m_states_context.states_vec) {
                state->ProcessEvents(event_type);
            }
        }
    }
    Application::UpdateStates();
}

void Application::Update(sf::Time elapsed)
{
    for (auto& state : this->m_states_context.states_vec) {
        state->Update(elapsed);
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