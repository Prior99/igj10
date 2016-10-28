#include "game.hpp"
#include "components/drawable.hpp"
#include "components/position.hpp"
#include "components/text.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class DrawSystem : public entityx::System<DrawSystem> {
  public:
    DrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w, h};
        m_drawtex =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                              game->world_size().w, game->world_size().h);
    }

    ~DrawSystem() {
        SDL_DestroyTexture(m_drawtex);
    }

    void renderDrawable(entityx::Entity entity) {
        entityx::ComponentHandle<Drawable> drawable = entity.component<Drawable>();
        entityx::ComponentHandle<Position> position = entity.component<Position>();

        SDL_Rect dest;
        dest.x = position->position()[0];
        dest.y = position->position()[1];
        dest.w = drawable->width();
        dest.h = drawable->height();

        SDL_RenderCopyEx(m_game->renderer(),
                         m_game->res_manager().texture(drawable->texture_key()), NULL, &dest, 0,
                         NULL, SDL_FLIP_NONE);
    }

    void renderText(entityx::Entity entity) {
        entityx::ComponentHandle<Text> textComponent = entity.component<Text>();
        auto position = entity.component<Position>()->position();
        auto color = textComponent->getColor();
        auto surf = TTF_RenderText_Blended(m_game->res_manager().font("font20"), textComponent->getText().c_str(), color);
        auto texture = SDL_CreateTextureFromSurface(m_game->renderer(), surf);
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

        SDL_Rect dest;
        dest.x = position.x;
        dest.y = position.y;
        dest.w = w;
        dest.h = h;

        SDL_RenderCopy(m_game->renderer(), texture, NULL, &dest);
        SDL_FreeSurface(surf);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) override {

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), m_drawtex);
        SDL_SetRenderDrawColor(m_game->renderer(), 0, 100, 200, 255);
        SDL_RenderClear(m_game->renderer());

        entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Drawable> drawable;
        entityx::ComponentHandle<Text> text;
        for (entityx::Entity entity : es.entities_with_components(position)) {

            if (entity.component<Drawable>()) {
                renderDrawable(entity);
            }
            if (entity.component<Text>()) {
                renderText(entity);
            }
        }

        // Render to final window
        SDL_SetRenderTarget(m_game->renderer(), nullptr);
        SDL_RenderCopy(m_game->renderer(), m_drawtex, &m_camera, nullptr);
        SDL_RenderPresent(m_game->renderer());
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *m_drawtex;
};
