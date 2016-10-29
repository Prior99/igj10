#include "game.hpp"

#include "components/position.hpp"
#include "components/text.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_config.hpp"
#include <iostream>

class OverlayDrawSystem {
  public:
    OverlayDrawSystem(Game *game) : m_game(game) {
        int w, h;
        SDL_RenderGetLogicalSize(game->renderer(), &w, &h);
        m_camera = SDL_Rect{0, 0, w/GAME_SCALE, h/GAME_SCALE};
        overlayTexture =
            SDL_CreateTexture(game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET,
                                game->world_size().w, game->world_size().h);
    }

    ~OverlayDrawSystem() {
        SDL_DestroyTexture(overlayTexture);
    }

    void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {

        // Change to render into rendertexture for now
        SDL_SetRenderTarget(m_game->renderer(), overlayTexture);
        SDL_SetRenderDrawColor(m_game->renderer(), 255, 255, 255, 0);
        SDL_RenderClear(m_game->renderer());

		entityx::ComponentHandle<Position> position;
        entityx::ComponentHandle<Text> text;
        for (entityx::Entity entity : es.entities_with_components(position, text)) {
	        auto color = text->getColor();
	        auto surf = TTF_RenderText_Blended(m_game->res_manager().font("font20"), text->getText().c_str(), color);
	        auto texture = SDL_CreateTextureFromSurface(m_game->renderer(), surf);
	        int w, h;
	        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

	        SDL_Rect dest{position->getPosition().x, position->getPosition().y, w, h};

	        SDL_RenderCopy(m_game->renderer(), texture, NULL, &dest);
	        SDL_FreeSurface(surf);
	    }

		SDL_SetTextureBlendMode(overlayTexture, SDL_BLENDMODE_ADD);
        // Render to final window
        SDL_SetRenderTarget(m_game->renderer(), nullptr);
        SDL_RenderCopy(m_game->renderer(), overlayTexture, &m_camera, nullptr);
    }

  private:
    Game *m_game;
    SDL_Rect m_camera;
    SDL_Texture *overlayTexture;
};
