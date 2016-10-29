#include "game.hpp"

#include "systems/draw/drawEntity.hpp"
#include "systems/draw/drawLight.hpp"
#include "systems/draw/drawOverlay.hpp"

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
        lightDrawSystem(game),
        overlayDrawSystem(game) {
    }

    ~DrawSystem() {
    }

    void drawBackground() {
        auto bgscale = glm::vec2(800, 600);
        auto playerPos = m_game->getPlayer().component<Position>()->getPosition();
        auto offset = playerPos - glm::vec2(WIDTH, HEIGHT) / (2 * GAME_SCALE * 16) + glm::vec2(0, bgscale.y / 2);
        SDL_SetRenderTarget(m_game->renderer(), nullptr);
        SDL_Rect src{0, 0, (int)bgscale.x, (int)bgscale.y};
        for (int x = -10; x < 10; x++) {
                auto pos = -offset + glm::vec2(x, 0) * bgscale * GAME_SCALE;
                SDL_Rect dest{(int)pos.x, (int)pos.y, (int)bgscale.x * GAME_SCALE, (int)bgscale.y * GAME_SCALE};
                SDL_RenderCopy(m_game->renderer(), m_game->res_manager().texture("background"), &src, &dest);
        }
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
        this->drawBackground();
        entityDrawSystem.update(es, events, dt);
        lightDrawSystem.update(es, events, dt);
        overlayDrawSystem.update(es, events, dt);

        SDL_RenderPresent(m_game->renderer());
    }

  private:
    Game *m_game;
    EntityDrawSystem entityDrawSystem;
    LightDrawSystem lightDrawSystem;
    OverlayDrawSystem overlayDrawSystem;
};
