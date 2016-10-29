#ifndef SYSTEM_DEATH_HPP
#define SYSTEM_DEATH_HPP

#include "game.hpp"
#include "game_config.hpp"

#include "components/position.hpp"
#include "components/player.hpp"

#include "entityx/entityx.h"
#include <glm/vec2.hpp>

#include<iostream>

class DeathSystem : public entityx::System<DeathSystem> {
  public:
	DeathSystem(Game *game): game(game) {

	}

	void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) {
		entityx::ComponentHandle<Position> position;
		entityx::ComponentHandle<Player> player;
		for(entityx::Entity entity: es.entities_with_components(position, player)) {
			(void)entity;
			if(position->getPosition().y >= HEIGHT) {
				game->shutdown();
			}
		}
	}

  private:
	Game *game;
};
#endif
