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
        int sidewalkwidth = 64;

        // create houses
		    for (int i = 0; i < 10; i++) {
            float height = 200.f + rand() % 100 - 50;
		        entityx::Entity box1 = es.create();
		        box1.assign<Position>(glm::vec2(i * 250.f, height));
		        box1.assign<Box>(glm::vec2(128.f, 128.f));
		        box1.assign<Drawable>("house", housewidth, 128);
		    }

        // create sidewalk
        for (int i = 0; i < 10; i++) {
	        entityx::Entity sidewalk = es.create();
	        sidewalk.assign<Position>(glm::vec2(0.f + i * sidewalkwidth, 400));
	        sidewalk.assign<Box>(glm::vec2(64.f, 48.f));
	        sidewalk.assign<Drawable>("sidewalk", sidewalkwidth, 48);
        }
			created = true;
		}
	}

  private:
	bool created;
};
#endif
