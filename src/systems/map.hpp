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
                srand (15);
                //calculate values
                int housewidth = 128;
                float housemargin = 250.f;
                int sidewalkwidth = 64;
                int streetwidth = 64;
                int maplength = 10;

                // create houses
                for (int i = 0; i < maplength * floor(housewidth/housewidth); i++) {
                    float height = 200.f + rand() % 100 - 50;
                    entityx::Entity box1 = es.create();
                    box1.assign<Position>(glm::vec2(i * housemargin, height));
                    box1.assign<Box>(glm::vec2(128.f, 128.f));
                    box1.assign<Drawable>("house", housewidth, 128);
                    std::cout << i << std::endl;
                }
                // create sidewalk
                for (int i = 0; i < maplength * (floor(housewidth/sidewalkwidth)+floor(housemargin/sidewalkwidth)); i++) {
                    entityx::Entity sidewalk = es.create();
                    sidewalk.assign<Position>(glm::vec2(0.f + i * sidewalkwidth, 400));
                    sidewalk.assign<Box>(glm::vec2(64.f, 48.f));
                    sidewalk.assign<Drawable>("sidewalk", sidewalkwidth, 48);
                    entityx::Entity street = es.create();
                    street.assign<Position>(glm::vec2(0.f + i * streetwidth, 404));
                    street.assign<Drawable>("street", streetwidth, 48);
                    std::cout << i << std::endl;
                }
                created = true;
            }
        }

    private:
        bool created;
};
#endif
