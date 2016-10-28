#include "main_state.hpp"

#include "components/drawable.hpp"
#include "components/position.hpp"
#include "systems/collision.hpp"
#include "systems/controls.hpp"
#include "systems/draw.hpp"

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

MainState::MainState(Game *game) : m_game(game) {
}

MainState::~MainState() {
}

int MainState::init() {
    m_systems.add<DrawSystem>(m_game);
    m_systems.add<ControlSystem>();
    m_systems.add<CollisionSystem>();
    m_systems.configure();

    entityx::Entity player = m_entities.create();
    player.assign<Position>(glm::vec2(300.f, 400.f));
	player.assign<Drawable>("gradient", 100, 100);
	player.assign<Player>(10);

    return 0;
}

void MainState::update(double dt) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            m_game->shutdown();
        }
        if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_ESCAPE) {
                m_game->shutdown();
            }
        }
    }

    m_systems.update<DrawSystem>(dt);
    m_systems.update<ControlSystem>(dt);
    m_systems.update<CollisionSystem>(dt);
}
