#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include "game.hpp"
#include "game_config.hpp"

#include "components/position.hpp"
#include "components/player.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include <iostream>

class MapSystem : public entityx::System<MapSystem> {
    public:
        MapSystem(Game *game): game(game), mapGeneratedX(0), sidewalkGeneratedX(0), height(0) {
            srand (time(0));
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            glm::vec2 pos;
            if(!created) {
                pos = glm::vec2(0, 0);
            } else {
                auto player = game->getPlayer();
                pos = player.component<Position>()->getPosition();
            }
            //calculate values
            int housewidth = 128;

            // create houses
            while (mapGeneratedX < pos.x + 1200) {
                int heightChange = rand() % 3;
                if (mapGeneratedX != 0) {
                    auto margin = housewidth + 60 + rand() % 90;
                    if (heightChange == 0 && height != 6) {
                        margin -= 40 + rand() % 40;
                    } else if (heightChange == 2 && height != 1) {
                        margin += rand() % 40;
                    }
                    mapGeneratedX += glm::max(margin, housewidth);
                }
                if (heightChange == 0 && height < 10) {
                    height++;
                }
                if (heightChange == 2 && height > 1) {
                    height --;
                }
                entityx::Entity bottom = es.create();
                bottom.assign<Position>(glm::vec2(mapGeneratedX, 400 - 32));
                bottom.assign<Drawable>("house-01-bottom", 128, 32);
                for (int j = 0; j < height; j++) {
                    entityx::Entity middle = es.create();
                    middle.assign<Position>(glm::vec2(mapGeneratedX, 400 - 32 - (j + 1) * 63));
                    middle.assign<Drawable>("house-01-middle", 128, 63);
                }
                entityx::Entity roof = es.create();
                roof.assign<Position>(glm::vec2(mapGeneratedX, 400 - 32 - height * 63 - 16));
                roof.assign<Drawable>("house-01-roof", 128, 32);
                roof.assign<Box>(glm::vec2(128.0f, 32.0f));
                if (mapGeneratedX == 0) {
                    mapGeneratedX = 1;
                }
            }
            while(sidewalkGeneratedX < pos.x + 1200) {
                entityx::Entity sidewalk = es.create();
                sidewalk.assign<Position>(glm::vec2(0.f + sidewalkGeneratedX, 400));
                sidewalk.assign<Drawable>("sidewalk", 64, 48);
                entityx::Entity street = es.create();
                street.assign<Position>(glm::vec2(0.f + sidewalkGeneratedX, 404));
                street.assign<Drawable>("street", 64, 48);
                entityx::Entity invisibleFloor = es.create();
                invisibleFloor.assign<Box>(glm::vec2(64.f, 48.f), true);
                invisibleFloor.assign<Position>(glm::vec2(0.f + sidewalkGeneratedX, 410));
                sidewalkGeneratedX += 64;
            }
            created = true;
            entityx::ComponentHandle<Position> position;
            for (entityx::Entity entity : es.entities_with_components(position)) {
                if (position->getPosition().x < pos.x - 1200) {
                    entity.destroy();
                }
            }
        }

    private:
        bool created;
        Game *game;
        int x;
        int mapGeneratedX;
        int sidewalkGeneratedX;
        int height;
};
#endif
