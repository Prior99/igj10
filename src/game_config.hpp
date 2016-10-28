#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include "glm/vec2.hpp"

/*
 *  Global config values for easy tweaking and changing
 */

//screen width and height
static const int WIDTH = 800;
static const int HEIGHT = 600;

static const bool DEBUG = false; 

static const float SPEED = 119;
static const float VELOCITY_LOSS = 0.7;
static const float GRAVITY = 10;
static const float JUMP_SPEED = 400;
static const glm::vec2 MAX_VELOCITY = glm::vec2(200.0f, 1000.0f);
static const float MAX_JUMP = 1.5f;

#endif
