#include "game.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/light.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>

class LightDrawSystem {
  public:
    LightDrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w, h};

        lightTexture =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                                game->world_size().w, game->world_size().h);
    }

    ~LightDrawSystem() {
        SDL_DestroyTexture(lightTexture);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {

        // RENDER LIGHT

        // Change to render into light rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), lightTexture);
        SDL_SetRenderDrawColor(m_game->renderer(), 20, 20, 20, 255);
        SDL_RenderClear(m_game->renderer());

        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Light> light;

        for (entityx::Entity entity : es.entities_with_components(position, light)) {

            (void)entity;

            auto coord = position->getPosition();
            auto tex = m_game->res_manager().texture(light->texture_key());
            auto drawable = entity.component<Drawable>();

            int w, h;
            if(drawable) {
                w = drawable->getWidth();
                h = drawable->getHeight();
            }
            else {
                SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
            }

            auto width = w * light->scale();
            auto height = h * light->scale();

            // Converted position
            SDL_Rect dest;
            dest.x = coord[0] - width / 2;
            dest.y = coord[1] - height / 2;
            dest.w = width;
            dest.h = height;

            SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_ADD);
            SDL_SetTextureColorMod(tex, light->color().r, light->color().g, light->color().b);
            SDL_RenderCopy(m_game->renderer(), tex, nullptr, &dest);
        }

        SDL_SetTextureBlendMode(lightTexture, SDL_BLENDMODE_MOD);

        // Render to final window
        SDL_SetRenderTarget(m_game->renderer(), nullptr);
        SDL_RenderCopy(m_game->renderer(), lightTexture, &m_camera, nullptr);
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *lightTexture;
};
