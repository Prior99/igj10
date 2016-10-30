#ifndef SYSTEM_DRAW_HPP
#define SYSTEM_DRAW_HPP

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
    DrawSystem(Game *game) : game(game), entityDrawSystem(game), lightDrawSystem(game), overlayDrawSystem(game) {
        gameTexture = SDL_CreateTexture(
            game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);

    }

    ~DrawSystem() {
    }

    void drawBackground() {
        auto bgSize = glm::vec2(800, 600);
        auto playerPos = game->getPlayer().component<Position>()->getPosition();
        auto offset = (playerPos + bgSize / 2) / PARALLAXITY;
        SDL_Rect src{0, 0, (int)bgSize.x, (int)bgSize.y};
        for (int x = -10; x < 10; x++) {
                auto pos = glm::vec2(x * bgSize.x, -0) - offset;
                SDL_Rect dest{(int)pos.x, (int)pos.y, (int)bgSize.x, (int)bgSize.y};
                SDL_RenderCopy(game->renderer(), game->res_manager().texture("background"), &src, &dest);
        }
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entityDrawSystem.update(es, events, dt);
        lightDrawSystem.update(es, events, dt);
        overlayDrawSystem.update(es, events, dt);
        auto src = SDL_Rect{0, 0, GAME_WIDTH, GAME_HEIGHT};
        auto dest = SDL_Rect{0, 0, GAME_WIDTH, GAME_HEIGHT};
        auto destScreen = SDL_Rect{0, 0, WIDTH, HEIGHT};
        auto renderer = game->renderer();
        SDL_SetRenderTarget(renderer, gameTexture);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        this->drawBackground();
        auto entityTexture = entityDrawSystem.getTexture();
        auto lightTexture = lightDrawSystem.getTexture();
        auto overlayTexture = overlayDrawSystem.getTexture();
        SDL_SetTextureBlendMode(entityTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, entityTexture, &src, &dest);
        if (this->game->isFrozen()) {
            SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_MOD);
            SDL_RenderCopy(renderer, lightTexture, &src, &dest);
        }
        SDL_SetTextureBlendMode(overlayTexture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, overlayTexture, &src, &dest);
        SDL_SetRenderTarget(renderer, nullptr);
        SDL_RenderCopy(renderer, gameTexture, &src, &destScreen);
        SDL_RenderPresent(renderer);
    }

  private:
    Game *game;
    EntityDrawSystem entityDrawSystem;
    LightDrawSystem lightDrawSystem;
    OverlayDrawSystem overlayDrawSystem;
    SDL_Texture *gameTexture;
};

#endif
