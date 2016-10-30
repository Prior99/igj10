#include "game.hpp"

#include "components/drawable.hpp"
#include "components/multipartDrawable.hpp"
#include "components/position.hpp"
#include "components/light.hpp"
#include "components/foreground.hpp"
#include "components/game-text.hpp"
#include "components/uniMultiDrawable.hpp"

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

    void renderMultiPart(entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<MultipartDrawable> multipartDrawable = entity.component<MultipartDrawable>();

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

    void renderRow(glm::vec2 startPos, SDL_Texture* texture, int reps, SDL_Rect* leftClip, SDL_Rect* centerClip, SDL_Rect* rightClip) {
        SDL_Rect leftDest{startPos.x, startPos.y, leftClip->w, leftClip->h};
        SDL_RenderCopy(game->renderer(), texture, leftClip, &leftDest);

        SDL_Rect centerDest{leftDest.x + leftDest.w, startPos.y, centerClip->w, centerClip->h};
        for(int i = 0; i < reps; i++) {
            SDL_RenderCopy(game->renderer(), texture, centerClip, &centerDest);
            centerDest.x += centerClip->w;
        }

        SDL_Rect rightDest{centerDest.x, startPos.y, rightClip->w, rightClip->h};
        SDL_RenderCopy(game->renderer(), texture, rightClip, &rightDest);
    }

    void renderUniMultiPart(entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<UniMultipartDrawable> drawable = entity.component<UniMultipartDrawable>();

        auto texture = game->res_manager().texture(drawable->getTexture());
        glm::vec2 reps = drawable->getRepititions();

        auto pos = position->getPosition() - offset;

        SDL_Rect leftClip, rightClip, centerClip;

        renderRow(pos, texture, reps.x, drawable->getLeftTopClip(&leftClip), drawable->getCenterTopClip(&centerClip), drawable->getRightTopClip(&rightClip));

        for(int row = 0; row < reps.y; row++) {
            auto newPos = pos + glm::vec2(0, drawable->getTop() + row * drawable->getCenterY());
            renderRow(newPos, texture, reps.x, drawable->getLeftCenterClip(&leftClip), drawable->getCenterClip(&centerClip), drawable->getRightCenterClip(&rightClip));
        }

        auto bottomPos = pos + glm::vec2(0, drawable->getTop() + reps.y * drawable->getCenterY());
        renderRow(bottomPos, texture, reps.x, drawable->getLeftBottomClip(&leftClip), drawable->getCenterBottomClip(&centerClip), drawable->getRightBottomClip(&rightClip));
    }

    void renderSinglePart(entityx::Entity entity, glm::vec2 offset) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();

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

    void renderText(entityx::Entity entity, glm::vec2 offset, double dt) {
        entityx::ComponentHandle<Position> position = entity.component<Position>();
        entityx::ComponentHandle<GameText> text = entity.component<GameText>();
        bool fast = text->isFast();
        auto player = game->getPlayer();
        auto pp = player.component<Position>()->getPosition();
        auto pos =  (fast ? pp - glm::vec2(24, -10) : position->getPosition()) - offset;
        float progress = text->getTime() / TEXT_DURATION;
        auto surf = TTF_RenderText_Blended(game->res_manager().font("font-small"), text->getText().c_str(), text->getColor());
        if (!fast) {
            SDL_LockSurface(surf);
            SDL_PixelFormat* fmt = surf->format;
            unsigned bpp = fmt->BytesPerPixel;
            for (int y = 0; y < surf->h; y++)
                for (int x = 0; x < surf->w; x++) {
                    Uint32* pixel_ptr = (Uint32 *)((Uint8 *)surf->pixels + y * surf->pitch + x * bpp);
                    Uint8 r, g, b, a;
                    SDL_GetRGBA( *pixel_ptr, fmt, &r, &g, &b, &a );
                    *pixel_ptr = SDL_MapRGBA( fmt, r, g, b, (int)(a * (0.5f + 0.5f * (1.0 - progress))));
                }

            SDL_UnlockSurface(surf);
        }
        auto texture = SDL_CreateTextureFromSurface(game->renderer(), surf);
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        auto progressSize = glm::min(progress * 2, 1.0f);
        int nW = w * progressSize;
        int offsetX = pos.x + (w - nW) / 2;
        SDL_Rect dest{pos.x + offsetX, pos.y - progress * 100, nW, h * progressSize};

        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(game->renderer(), texture, NULL, &dest);
        SDL_FreeSurface(surf);
        text->updateTime(dt);
        if (text->getTime() > TEXT_DURATION) {
            entity.destroy();
        }
    }

    void renderEntity(entityx::Entity entity, glm::vec2 offset, double dt) {
        if(entity.component<MultipartDrawable>()) {
            renderMultiPart(entity, offset);
        } else if (entity.component<Drawable>()) {
            renderSinglePart(entity, offset);
        } else if (entity.component<UniMultipartDrawable>()) {
            renderUniMultiPart(entity, offset);
        } else if(entity.component<GameText>()) {
            renderText(entity, offset, dt);
        }
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {
        auto playerPos = game->getPlayer().component<Position>()->getPosition();
        entityx::ComponentHandle<Drawable> drawable = game->getPlayer().component<Drawable>();
        if(drawable) {
            auto playerWidth = drawable->getWidth();
            auto playerHeight = drawable->getHeight();
            playerPos = glm::vec2(playerPos.x + playerWidth / 2, playerPos.y + playerHeight / 2);
        }
        auto offset = playerPos - glm::vec2(GAME_WIDTH / 4.0f, GAME_HEIGHT) / 2.0f;
        offset.y = glm::min(offset.y, 50.0f);

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(game->renderer(), entityTexture);
        SDL_SetRenderDrawColor(game->renderer(), 0, 0, 0, 0);
        SDL_RenderClear(game->renderer());

        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Foreground> foreground;

        for (entityx::Entity entity : es.entities_with_components(position)) {
            if(!entity.component<Foreground>()) {
                auto privOffset = glm::vec2(0, 0);
                auto draw = entity.component<Drawable>();
                if (draw) {
                    privOffset = draw->getOffset();
                }
                renderEntity(entity, offset + privOffset, dt);
            }
        }

        for(entityx::Entity entity: es.entities_with_components(position, foreground)) {
            auto privOffset = glm::vec2(0, 0);
            auto draw = entity.component<Drawable>();
            if (draw) {
                privOffset = draw->getOffset();
            }
            renderEntity(entity, offset + privOffset, dt);
        }
    }

    SDL_Texture *getTexture() {
        return this->entityTexture;
    }

  private:
    Game *game;
    SDL_Texture *entityTexture;
};
