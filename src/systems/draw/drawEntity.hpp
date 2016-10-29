#include "game.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/light.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include "glm/common.hpp"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_config.hpp"
#include <iostream>

class EntityDrawSystem {
  public:
    EntityDrawSystem(Game *game) : game(game) {
        entityTexture = SDL_CreateTexture(
            game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);
    }

    ~EntityDrawSystem() {
        SDL_DestroyTexture(entityTexture);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {
        auto playerPos = game->getPlayer().component<Position>()->getPosition();
        auto offset = playerPos - glm::vec2(GAME_WIDTH / 4.0f, GAME_HEIGHT) / 2.0f;
        offset.y = glm::min(offset.y, 212.0f);

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(game->renderer(), entityTexture);
        SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 0);
        SDL_RenderClear(game->renderer());

        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Position> position;

        for (entityx::Entity entity : es.entities_with_components(drawable, position)) {

            (void)entity;
            auto pos = position->getPosition() - offset;
            SDL_Rect dest{pos.x, pos.y, drawable->getWidth(), drawable->getHeight()};
            SDL_Rect src{0, 0, drawable->getWidth(), drawable->getHeight()};

            SDL_Texture* texture = game->res_manager().texture(drawable->texture_key());
            SDL_Rect *clip = &src;
            if(drawable->hasAnimation()){
                auto& animation = drawable->getAnimation();
                texture = game->res_manager().texture(animation.getTextureKey());
                clip = animation.getAnimationFrame(clip);
            }
            SDL_RenderCopy(game->renderer(), texture, clip, &dest);
        }
    }

    SDL_Texture *getTexture() {
        return this->entityTexture;
    }

  private:
    Game *game;
    SDL_Texture *entityTexture;
};
