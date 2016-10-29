#include "game.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/light.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_config.hpp"
#include <iostream>

class LightDrawSystem {
  public:
    LightDrawSystem(Game *game) : game(game) {
        lightTexture = SDL_CreateTexture(
            game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);
    }

    ~LightDrawSystem() {
        SDL_DestroyTexture(lightTexture);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {
        auto playerPos = game->getPlayer().component<Position>()->getPosition();
        auto offset = playerPos - glm::vec2(GAME_WIDTH / 4.0f, GAME_HEIGHT) / 2.0f;

        // RENDER LIGHT

        // Change to render into light rendertexture for now
        SDL_SetRenderTarget(game->renderer(), lightTexture);
        SDL_SetRenderDrawColor(game->renderer(), 20, 20, 20, 255);
        SDL_RenderClear(game->renderer());

        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Light> light;

        for (entityx::Entity entity : es.entities_with_components(position, light)) {

            (void)entity;

            auto coord = position->getPosition();
            auto tex = game->res_manager().texture(light->texture_key());
            auto drawable = entity.component<Drawable>();

            int wPlayer, hPlayer;
            if(drawable) {
                wPlayer = drawable->getWidth();
                hPlayer = drawable->getHeight();
            }
            else {
                SDL_QueryTexture(tex, nullptr, nullptr, &wPlayer, &hPlayer);
            }

            auto wLight = wPlayer * light->scale() / game->getInsanity();
            auto hLight = hPlayer * light->scale() / game->getInsanity();

            // Converted position
            auto pos = glm::vec2(coord.x + wPlayer / 2 - wLight / 2, coord.y + hPlayer / 2 - hLight / 2) - offset;
            SDL_Rect dest{pos.x, pos.y, wLight, hLight};

            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(tex, light->color().r, light->color().g, light->color().b);
            SDL_RenderCopy(game->renderer(), tex, nullptr, &dest);
        }
    }

    SDL_Texture *getTexture() {
        return this->lightTexture;
    }

  private:
    Game *game;
    SDL_Texture *lightTexture;
};
