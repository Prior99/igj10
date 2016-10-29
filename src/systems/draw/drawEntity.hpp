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

class EntityDrawSystem {
  public:
    EntityDrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w, h};
        entityTexture =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                                game->world_size().w, game->world_size().h);
    }

    ~EntityDrawSystem() {
        SDL_DestroyTexture(entityTexture);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), entityTexture);
        SDL_SetRenderDrawColor(m_game->renderer(), 0, 100, 200, 255);
        SDL_RenderClear(m_game->renderer());

        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;

        for (entityx::Entity entity : es.entities_with_components(drawable, position)) {

            (void)entity;

            SDL_Rect dest;
            dest.x = position->getPosition()[0];
            dest.y = position->getPosition()[1];
            dest.w = drawable->getWidth();
            dest.h = drawable->getHeight();

            SDL_RenderCopyEx(m_game->renderer(),
                             m_game->res_manager().texture(drawable->texture_key()), NULL, &dest, 0,
                             NULL, SDL_FLIP_NONE);
        }

        // Render to final window
        SDL_SetRenderTarget(m_game->renderer(), nullptr);
        SDL_RenderCopy(m_game->renderer(), entityTexture, &m_camera, nullptr);
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *entityTexture;
};
