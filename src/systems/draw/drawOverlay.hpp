#include "game.hpp"

#include "components/position.hpp"
#include "components/text.hpp"
#include "components/overlay.hpp"

#include "systems/draw/drawEntity.hpp"

#include "strapon/resource_manager/resource_manager.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "game_config.hpp"
#include <iostream>

class OverlayDrawSystem {
    public:
        OverlayDrawSystem(Game *game) : game(game) {
            overlayTexture = SDL_CreateTexture(
                game->renderer(), SDL_PIXELTYPE_UNKNOWN, SDL_TEXTUREACCESS_TARGET, GAME_WIDTH, GAME_HEIGHT);

        }

        ~OverlayDrawSystem() {
            SDL_DestroyTexture(overlayTexture);
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events,
                entityx::TimeDelta dt) {

            // Change to render into rendertexture for now
            SDL_SetRenderTarget(game->renderer(), overlayTexture);
            SDL_SetRenderDrawColor(game->renderer(), 255, 255, 255, 0);
            SDL_RenderClear(game->renderer());

            entityx::ComponentHandle<Position> position;
            entityx::ComponentHandle<Text> text;
            entityx::ComponentHandle<Overlay> overlay;
            for(entityx::Entity entity: es.entities_with_components(position, overlay)) {
                EntityDrawSystem::renderEntity(game, entity, glm::vec2(0, 0), dt);
            }

            for (entityx::Entity entity : es.entities_with_components(position, text)) {
                auto color = text->getColor();
                auto surf = TTF_RenderText_Blended(game->res_manager().font("font-big"), text->getText().c_str(), color);
                auto texture = SDL_CreateTextureFromSurface(game->renderer(), surf);
                int w, h;
                SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

                SDL_Rect dest{position->getPosition().x, position->getPosition().y, w, h};

                SDL_RenderCopy(game->renderer(), texture, NULL, &dest);
                SDL_FreeSurface(surf);
            }
        }

        SDL_Texture *getTexture() {
            return this->overlayTexture;
        }

    private:
        Game *game;
        SDL_Texture *overlayTexture;
};
