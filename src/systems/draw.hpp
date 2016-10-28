#include "game.hpp"
#include "systems/draw/drawEntity.hpp"
#include "systems/draw/drawLight.hpp"
#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : m_game(game),
        entityDrawSystem(game),
        lightDrawSystem(game){
    }

    ~DrawSystem() {
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entityDrawSystem.update(es, events, dt);
        lightDrawSystem.update(es, events, dt);

        SDL_RenderPresent(m_game->renderer());
    }

  private:
    Game *m_game;
    EntityDrawSystem entityDrawSystem;
    LightDrawSystem lightDrawSystem;
};
