#include "game.hpp"
#include "components/drawable.hpp"
#include "components/multipartDrawable.hpp"
#include "components/position.hpp"
#include "components/light.hpp"

#include "utils.hpp"

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
        entityx::ComponentHandle<MultipartDrawable> multipartDrawable;
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

        for(entityx::Entity entity: es.entities_with_components(position, multipartDrawable)) {
            (void)entity;
            auto pos = position->getPosition() - offset;
            SDL_Rect dest{pos.x, pos.y, multipartDrawable->getWidth(), 0};
            SDL_Rect src{0, 0, multipartDrawable->getWidth(), 0};

            PartialDrawable top = multipartDrawable->getTop();
            src.h = top.height;
            dest.h = top.height;
            SDL_RenderCopy(game->renderer(), game->res_manager().texture(top.texture), &src, &dest);

            dest.y += top.height;
            PartialDrawable middle = multipartDrawable->getMiddle();
            src.h = middle.height;
            dest.h = middle.height;
            for(int i = 0; i < (int) multipartDrawable->getRepeat(); i++) {
                SDL_RenderCopy(game->renderer(), game->res_manager().texture(middle.texture), &src, &dest);
                dest.y += middle.height;
            }
            if(compareDoubles(multipartDrawable->getRepeat(), (int) multipartDrawable->getRepeat()) != 0) {
                auto fractRepeat = multipartDrawable->getRepeat() - (int) multipartDrawable->getRepeat();
                src.h = fractRepeat * middle.height;
                dest.h = src.h;
                SDL_RenderCopy(game->renderer(), game->res_manager().texture(middle.texture), &src, &dest);
                dest.y += src.h;
            }
            PartialDrawable bottom = multipartDrawable->getBottom();
            src.h = bottom.height;
            dest.h = bottom.height;
            SDL_RenderCopy(game->renderer(), game->res_manager().texture(bottom.texture), &src, &dest);
        }
    }

    SDL_Texture *getTexture() {
        return this->entityTexture;
    }

  private:
    Game *game;
    SDL_Texture *entityTexture;
};
