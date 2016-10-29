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

class EntityDrawSystem {
  public:
    EntityDrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w, h};
        entityTexture =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                                game->world_size().w/GAME_SCALE, game->world_size().h/GAME_SCALE);
    }

    ~EntityDrawSystem() {
        SDL_DestroyTexture(entityTexture);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {
        auto playerPos = m_game->getPlayer().component<Position>()->getPosition();
        auto offset = playerPos - glm::vec2(WIDTH, HEIGHT) / (2.0f * GAME_SCALE);
        // Change to render into rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), entityTexture);
        SDL_SetRenderDrawColor(m_game->renderer(), 0, 0, 0, 0);
        SDL_RenderClear(m_game->renderer());

        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;

        for (entityx::Entity entity : es.entities_with_components(drawable, position)) {

            (void)entity;
            auto pos = position->getPosition() - offset;
            SDL_Rect dest{pos.x, pos.y, drawable->getWidth(), drawable->getHeight()};
            SDL_Rect src{0, 0, drawable->getWidth(), drawable->getHeight()};

            SDL_RenderCopy(m_game->renderer(), m_game->res_manager().texture(drawable->texture_key()), &src, &dest);
        }

        // Render to final window
        SDL_SetTextureBlendMode(entityTexture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(m_game->renderer(), nullptr);
        SDL_RenderCopy(m_game->renderer(), entityTexture, &m_camera, nullptr);
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *entityTexture;
};
