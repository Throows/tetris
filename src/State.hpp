#pragma once

#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <spdlog/spdlog.h>
#include "RessourceManager.hpp"

enum StateID {
    SPLASH,
    MAIN_MENU,
    GAME,
    PAUSE,
    GAME_OVER,
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
    State(StateID id, StatesContext& context, RessourceManager& ressources) 
        : m_state_id(id), m_states_context(context), m_ressources(ressources) {};
    virtual ~State() = default;

    virtual bool ProcessEvents(sf::Event& event) = 0;
    virtual bool Update(sf::Time elapsed) = 0;
    virtual void Render(sf::RenderWindow &window) = 0;

    StateID GetStateID() { return this->m_state_id; }
    sf::Font& GetFont(FontsID id) { return this->m_ressources.GetFont(id); }
    sf::Texture& GetTexture(TexturesID id) { return this->m_ressources.GetTexture(id); }

    void PushState(StateID state_id) {
        this->m_states_context.states_changes.push_back({StatesContext::StateChange::Action::PUSH, state_id});
    }

    void PopState(StateID state_id) {
        this->m_states_context.states_changes.push_back({StatesContext::StateChange::Action::POP, state_id});
    }

private:
    StateID m_state_id;
    StatesContext& m_states_context;
    RessourceManager& m_ressources;
};

