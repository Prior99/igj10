#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include "game.hpp"
#include "game_config.hpp"

#include "components/position.hpp"
#include "components/player.hpp"
#include "components/stomper.hpp"
#include "components/box.hpp"
#include "components/multipartDrawable.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include <iostream>
#include <math.h>

static const int houseWidth = 128;

int tobergteDifficultyFunction(int x, int max) {
    return glm::min(glm::max(log(x*x) * 2.0, 1.0), (double)max);
}

class MapSystem : public entityx::System<MapSystem> {
    public:
        MapSystem(Game *game): game(game), mapGeneratedX(0), sidewalkGeneratedX(0), height(0) {
            srand (time(0));
        }

        void generateStreet(entityx::EntityManager &es, glm::vec2 pos) {
            const int streetChunkWidth = 64;
            while(sidewalkGeneratedX < pos.x + PREGENERATE) {
                // Generate sidewalk texture
                entityx::Entity sidewalk = es.create();
                sidewalk.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM));
                sidewalk.assign<Drawable>("sidewalk", streetChunkWidth, 48);
                // Generate street texture
                entityx::Entity street = es.create();
                street.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM + 4));
                street.assign<Drawable>("street", streetChunkWidth, 48);
                // Generate invisble floor of death
                entityx::Entity invisibleFloor = es.create();
                invisibleFloor.assign<Box>(glm::vec2((float)streetChunkWidth, 48.f), true);
                invisibleFloor.assign<Position>(glm::vec2((float)sidewalkGeneratedX, GAME_BOTTOM + 10));
                // All of them were streetChunkWid pixel wide, so increase accordingly
                sidewalkGeneratedX += streetChunkWidth;
            }
        }

        void createStomper(entityx::EntityManager &es, int x, int y) {
            PartialDrawable top = {"stomper-top", 39};
            PartialDrawable middle = {"stomper-middle", 8};
            PartialDrawable bottom = {"stomper-bottom", 8};
            entityx::Entity stomper = es.create();
            stomper.assign<Position>(glm::vec2(x, y));
            stomper.assign<MultipartDrawable>(36, top, middle, bottom);
            stomper.assign<Box>(glm::vec2(36.f, 47.f), false, false, true, false);
            stomper.assign<Stomper>(0, 100, 1000, true, (rand() % 40) / 10.0);
            stomper.component<MultipartDrawable>()->setHeight(80);
        }

        void createHouse(entityx::EntityManager &es, int height, int x) {
            static const int roofHeight = 16;
            static const int bottomHeight = 32;
            static const int middleHeight = 63;
            // Generate bottom part of house
            entityx::Entity bottom = es.create();
            bottom.assign<Position>(glm::vec2(x, GAME_BOTTOM - bottomHeight));
            bottom.assign<Drawable>("house-01-bottom", houseWidth, bottomHeight);
            // Generate middle parts of house, dependend on height
            for (int j = 0; j < height; j++) {
                entityx::Entity middle = es.create();
                middle.assign<Position>(glm::vec2(x, GAME_BOTTOM - bottomHeight - (j + 1) * middleHeight));
                middle.assign<Drawable>("house-01-middle", houseWidth, middleHeight);
            }
            const int totalMiddleHeight = middleHeight * height;
            // Generate roof part of house
            entityx::Entity roof = es.create();
            roof.assign<Position>(glm::vec2(x, GAME_BOTTOM - bottomHeight - totalMiddleHeight - roofHeight));
            roof.assign<Drawable>("house-01-roof", houseWidth, roofHeight);
            roof.assign<Box>(glm::vec2((float)houseWidth, (float)roofHeight));
        }

        void generateBuildings(entityx::EntityManager &es, glm::vec2 pos) {
            static const int BUILDING_HIGHER_1 = 0;
            static const int BUILDING_HIGHER_2 = 1;
            static const int BUILDING_SAME = 2;
            static const int BUILDING_LOWER = 3;
            static const int BUILDING_LOWER_TWICE = 4;
            static const int minGap = 10;
            static const int variableGapSize = 80;
            while (mapGeneratedX < pos.x + PREGENERATE) {
                int heightChange = rand() % 5; // 0 = higher, 1 = same height, 2 = lower, 3 = twice lower
                int heightDifferenceModifier;
                if ((heightChange == BUILDING_HIGHER_1 || heightChange == BUILDING_HIGHER_2) && height < 10) {
                    height++;
                    heightDifferenceModifier = 0;
                }
                if (heightChange == BUILDING_SAME && height > 1) {
                    heightDifferenceModifier = rand() % tobergteDifficultyFunction(mapGeneratedX, 20);
                }
                if (heightChange == BUILDING_LOWER && height > 1) {
                    height --;
                    heightDifferenceModifier = rand() % tobergteDifficultyFunction(mapGeneratedX, 50);
                }
                if (heightChange == BUILDING_LOWER_TWICE && height > 2) {
                    height -= 2;
                    heightDifferenceModifier = rand() % tobergteDifficultyFunction(mapGeneratedX, 100);
                }
                createHouse(es, height, mapGeneratedX);
                if (rand() % 5 == 0) {
                    createStomper(es, mapGeneratedX + 16, GAME_BOTTOM - height * 63 - 16 - 36 - 220);
                }
                mapGeneratedX += houseWidth + minGap + rand() % variableGapSize + heightDifferenceModifier;
            }
        }

        void cleanup(entityx::EntityManager &es, glm::vec2 pos) {
            entityx::ComponentHandle<Position> position;
            for (entityx::Entity entity : es.entities_with_components(position)) {
                if (position->getPosition().x < pos.x - PREGENERATE) {
                    entity.destroy();
                }
            }
        }

        void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
            glm::vec2 pos;
            auto player = game->getPlayer();
            pos = player.component<Position>()->getPosition();
            generateStreet(es, pos);
            generateBuildings(es, pos);
            cleanup(es, pos);
        }

    private:
        Game *game;
        int x;
        int mapGeneratedX;
        int sidewalkGeneratedX;
        int height;
};
#endif
