#include "main_state.hpp"

#include "animation.hpp"
#include "components/drawable.hpp"
#include "components/text.hpp"
#include "components/position.hpp"
#include "components/light.hpp"
#include "components/velocity.hpp"
#include "components/stepping.hpp"
#include "components/box.hpp"
#include "components/stomper.hpp"
#include "components/gravity.hpp"
#include "components/foreground.hpp"
#include "systems/collision.hpp"
#include "systems/controls.hpp"
#include "systems/movement.hpp"
#include "systems/gravity.hpp"
#include "systems/draw.hpp"
#include "systems/death.hpp"
#include "systems/map.hpp"
#include "systems/insanity.hpp"
#include "systems/animation.hpp"
#include "systems/stepping.hpp"
#include "systems/stomper.hpp"

#include "entityx/entityx.h"

#include <stdlib.h>
#include <SDL2/SDL.h>

MainState::MainState(Game *game) : m_game(game) {
}

MainState::~MainState() {
}

int MainState::init() {
    m_systems.add<DrawSystem>(m_game);
    m_systems.add<ControlSystem>(m_game);
    m_systems.add<MovementSystem>();
    m_systems.add<GravitySystem>();
    m_systems.add<CollisionSystem>(m_game);
    m_systems.add<DeathSystem>(m_game);
    m_systems.add<MapSystem>(m_game);
    m_systems.add<InsanitySystem>(m_game);
    m_systems.add<AnimationSystem>();
    m_systems.add<StomperSystem>(m_game);
    m_systems.add<SteppingSystem>(m_game);
    m_systems.configure();

    auto playerAnimations = AnimationCollection("player");
    playerAnimations.addAnimation("run", 0, 4, 0.3, glm::vec2(16, 24));
    playerAnimations.addAnimation("jump", 24, 3, 0.5, glm::vec2(16, 24));
    playerAnimations.setAnimation("run", AnimationPlaybackType::LOOP);
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(000.f, 000.f));
    player.assign<Drawable>("player", 16, 24, playerAnimations);
    glm::i8vec3 testcolor = {255, 64, 16};
    player.assign<Light>("gradient", 500, testcolor);
    player.assign<Velocity>();
    player.assign<Stepping>();
    player.assign<Gravity>();
    player.assign<Collidable>(24.0f);
    player.assign<Player>();
    player.assign<Foreground>();
    m_game->setPlayer(player);

    entityx::Entity lol = entities.create();
    lol.assign<Position>(glm::vec2(0.f, 0.f));
    lol.assign<Text>("LOL", SDL_Color {200, 100, 100, 150});

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

    m_systems.update<MapSystem>(dt);
    m_systems.update<DeathSystem>(dt);
    m_systems.update<AnimationSystem>(dt);
    m_systems.update<DrawSystem>(dt);
    m_systems.update<MovementSystem>(dt);
    m_systems.update<ControlSystem>(dt);
    m_systems.update<GravitySystem>(dt);
    m_systems.update<CollisionSystem>(dt);
    m_systems.update<InsanitySystem>(dt);
    m_systems.update<SteppingSystem>(dt);
    m_systems.update<StomperSystem>(dt);
}
