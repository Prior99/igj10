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
        MapSystem() {

        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            if(!created) {
                srand (time(0));
                //calculate values
                int housewidth = 128;
                int sidewalkwidth = 64;
                int streetwidth = 64;
                int maplength = 20;

                // create houses
                int x = 0;
                int height = rand() % 5;
                for (int i = 0; i < maplength * floor(housewidth/housewidth); i++) {
                    bool up = rand() % 2 == 1;
                    height = up ? height + 1 : height - 1;
                    height = glm::min(glm::max(height, 1), 6);
                    auto margin = housewidth + 60 + rand() % 120;
                    entityx::Entity bottom = es.create();
                    bottom.assign<Position>(glm::vec2(x, 400 - 32));
                    bottom.assign<Drawable>("house-01-bottom", 128, 32);
                    for (int j = 0; j < height; j++) {
                        entityx::Entity middle = es.create();
                        middle.assign<Position>(glm::vec2(x, 400 - 32 - (j + 1) * 63));
                        middle.assign<Drawable>("house-01-middle", 128, 63);
                    }
                    entityx::Entity roof = es.create();
                    roof.assign<Position>(glm::vec2(x, 400 - 32 - height * 63 - 16));
                    roof.assign<Drawable>("house-01-roof", 128, 32);
                    roof.assign<Box>(glm::vec2(128.0f, 32.0f));
                    std::cout << i << std::endl;
                    x += margin;
                }
                // create sidewalk
                for (int i = 0; i < maplength * (floor(housewidth/sidewalkwidth)+floor(250.f/sidewalkwidth)); i++) {
                    entityx::Entity sidewalk = es.create();
                    sidewalk.assign<Position>(glm::vec2(0.f + i * sidewalkwidth, 400));
                    sidewalk.assign<Drawable>("sidewalk", sidewalkwidth, 48);
                    entityx::Entity street = es.create();
                    street.assign<Position>(glm::vec2(0.f + i * streetwidth, 404));
                    street.assign<Drawable>("street", streetwidth, 48);
                    entityx::Entity invisibleFloor = es.create();
                    invisibleFloor.assign<Box>(glm::vec2(64.f, 48.f), true);
                    invisibleFloor.assign<Position>(glm::vec2(0.f + i * streetwidth, 410));

                    std::cout << i << std::endl;
                }
                created = true;
            }
        }

    private:
        bool created;
};
#endif
