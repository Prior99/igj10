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
#include "systems/saw.hpp"
#include "systems/orb.hpp"

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
    m_systems.add<SanitySystem>(m_game);
    m_systems.add<AnimationSystem>();
    m_systems.add<StomperSystem>(m_game);
    m_systems.add<SteppingSystem>(m_game);
    m_systems.add<SawSystem>(m_game);
    m_systems.add<OrbSystem>(m_game);
    m_systems.configure();

    auto playerAnimations = AnimationCollection("player");
    playerAnimations.addAnimation("run", 0, 4, 0.3, glm::vec2(16, 24));
    playerAnimations.addAnimation("jump", 24, 3, 0.5, glm::vec2(16, 24));
    playerAnimations.addAnimation("dissolve", 48, 11, 1.2, glm::vec2(16, 24));
    playerAnimations.setAnimation("run", AnimationPlaybackType::LOOP);
    entityx::Entity player = entities.create();
    player.assign<Position>(glm::vec2(20.f, 0.f));
    player.assign<Drawable>("player", 16, 24, playerAnimations);
    player.component<Drawable>()->setOffset(glm::vec2(0, 24));
    glm::i8vec3 testcolor = {255, 64, 16};
    player.assign<Light>("gradient", 500, testcolor);
    player.assign<Velocity>();
    player.assign<Stepping>();
    player.assign<Gravity>();
    player.assign<Collidable>(8.0f);
    player.assign<Player>();
    player.assign<Foreground>();
    m_game->setPlayer(player);

    entityx::Entity highscore = entities.create();
    highscore.assign<Position>(glm::vec2(10.f, 10.f));
    highscore.assign<Highscore>();
    highscore.assign<Text>("Score: 0", SDL_Color {255, 200, 0, 0});

    glm::vec2 clockSize(64, 64);

    AnimationCollection clockSwitch("clock-face");
    clockSwitch.addAnimation("day", 0, 1, 0, clockSize);
    clockSwitch.addAnimation("night", clockSize.y, 1, 0, clockSize);
    clockSwitch.setAnimation("day", AnimationPlaybackType::FREEZE);
    Drawable clockFace("clock-face", clockSize.x, clockSize.y, clockSwitch);

    AnimationCollection clockHandMSSwitch("clock-hand-ms");
    clockHandMSSwitch.addAnimation("day", 0, 1, 0, clockSize);
    clockHandMSSwitch.addAnimation("night", clockSize.y, 1, 0, clockSize);
    clockHandMSSwitch.setAnimation("day", AnimationPlaybackType::FREEZE);
    Drawable clockHandMS("clock-hand-ms", clockSize.x, clockSize.y, clockHandMSSwitch);

    AnimationCollection clockHandSSwitch("clock-hand-s");
    clockHandSSwitch.addAnimation("day", 0, 1, 0, clockSize);
    clockHandSSwitch.addAnimation("night", clockSize.y, 1, 0, clockSize);
    clockHandSSwitch.setAnimation("day", AnimationPlaybackType::FREEZE);
    Drawable clockHandS("clock-hand-s", clockSize.x, clockSize.y, clockHandSSwitch);

    std::vector<Drawable> clockLayers({clockFace, clockHandMS, clockHandS});
    entityx::Entity clock = entities.create();
    clock.assign<Position>(glm::vec2(GAME_WIDTH - 8 - clockSize.x, 8));
    clock.assign<StackedDrawable>(clockLayers);
    clock.assign<Overlay>();
    m_game->setInsanityCounter(clock);

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

    m_systems.update_all(dt);
}
