#ifndef SYSTEM_MAP_HPP
#define SYSTEM_MAP_HPP

#include "game.hpp"
#include "game_config.hpp"

#include "components/position.hpp"
#include "components/player.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include<iostream>

class MapSystem : public entityx::System<MapSystem> {
  public:
	MapSystem() {

	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
		if(!created) {
			srand (15);
		    for (int i = 0; i < 10; i++) {
		        entityx::Entity box1 = es.create();
		        box1.assign<Position>(glm::vec2(i * 250.f, 200.f + rand() % 100 - 50));
		        box1.assign<Box>(glm::vec2(128.f, 128.f));
		        box1.assign<Drawable>("house", 128, 128);
		    }
			created = true;
		}
	}

  private:
	bool created;
};
#endif
