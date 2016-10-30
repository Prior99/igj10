#ifndef GAME_HPP
#define GAME_HPP

#include "components/position.hpp"

#include "strapon/state/state.hpp"
#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

#include <stack>
#include <string>

class Game {
  public:
    ~Game();

    int init();
    void mainloop();
    bool is_running();
    float getSanity();
    void setSanity(float f);
    void addSanity(float f);
    void shutdown();
    void reset();
    void setUnFreeze();
    void popstate();
    void toggleMute();
    bool isMuted();
    void toggleFreeze();
    bool isFrozen();

    std::stack<std::pair<std::string, std::unique_ptr<State>>> &states();
    const std::string &statename() const;

    SDL_Renderer *renderer();
    ResourceManager &res_manager();
    SDL_Window *window();
    entityx::Entity getPlayer();
    void setPlayer(entityx::Entity &player) {
        this->player = player;
    }

  private:
    entityx::Entity player;
    float sanity = 5.0f;
    bool m_running = true;
    int m_last_frame_time = 0;

    SDL_Renderer *m_render;
    SDL_Window *m_window;
    std::stack<std::pair<std::string, std::unique_ptr<State>>> m_states;
    entityx::EntityX m_ex;
    ResourceManager m_res_manager;
    bool freeze;
    bool muted;
};

#endif /* end of include guard: GAME_HPP */
