#include "main_state.hpp"

#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/velocity.hpp"
#include "components/box.hpp"
#include "components/gravity.hpp"
#include "systems/collision.hpp"
#include "systems/controls.hpp"
#include "systems/movement.hpp"
#include "systems/gravity.hpp"
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
    m_systems.add<MovementSystem>();
    m_systems.add<GravitySystem>();
    m_systems.add<CollisionSystem>();
    m_systems.configure();

    entityx::Entity box1 = entities.create();
    box1.assign<Position>(glm::vec2(0.0f, 200.f));
    box1.assign<Box>(glm::vec2(200.0f, 400.0f));
	box1.assign<Drawable>("house", 200, 400);

    entityx::Entity box2 = entities.create();
    box2.assign<Position>(glm::vec2(400.0f, 400.f));
    box2.assign<Box>(glm::vec2(400.0f, 200.0f));
	box2.assign<Drawable>("house", 400, 200);

    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(000.f, 000.f));
	player.assign<Drawable>("gradient", 100, 100);
	player.assign<Velocity>();
	player.assign<Gravity>();
	player.assign<Collidable>(50.0f);
	player.assign<Player>();

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
    m_systems.update<MovementSystem>(dt);
    m_systems.update<ControlSystem>(dt);
    m_systems.update<GravitySystem>(dt);
    m_systems.update<CollisionSystem>(dt);
}
