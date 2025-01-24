#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <spdlog/spdlog.h>

enum StateID {
    SPLASH,
    MAIN_MENU,
    GAME,
    GAME_OVER
};

class State;

typedef std::map<StateID, std::function<std::unique_ptr<State>()>> StatesMap;
typedef std::vector<std::unique_ptr<State>> StatesVec;

struct StatesContext {
    StatesMap states_map;
    StatesVec states_vec;
    struct StateChange {
        enum class Action {
            PUSH,
            POP,
        } action;
        StateID state_id;
    };
    std::vector<StateChange> states_changes;
};

class State
{
public:
    State(StateID id, StatesContext& context) : m_state_id(id), m_states_context(context) {};
    virtual ~State() = default;

    virtual void ProcessEvents(sf::Event& event) = 0;
    virtual void Update(sf::Time elapsed) = 0;
    virtual void Render(sf::RenderWindow &window) = 0;

    StateID GetStateID() { return this->m_state_id; }

    void PushState(StateID state_id) {
        this->m_states_context.states_changes.push_back({StatesContext::StateChange::Action::PUSH, state_id});
    }

    void PopState() {
        this->m_states_context.states_changes.push_back({StatesContext::StateChange::Action::POP, m_state_id});
    }

private:
    StateID m_state_id;
    StatesContext& m_states_context;
    
};

