#ifndef GAME_CONFIG_HPP
#define GAME_CONFIG_HPP

#include "glm/vec2.hpp"
#include <string>

/*
 *  Global config values for easy tweaking and changing
 */

//screen width and height
static const int WIDTH = 1024;
static const int HEIGHT = 768;
static const int GAME_WIDTH = 512;
static const int GAME_HEIGHT = 384;

static const bool DEBUG = false;

static const bool HARDCORE = false;

static const float SPEED = 66;
static const float VELOCITY_LOSS = 0.8;
static const float GRAVITY = 10;
static const float JUMP_SPEED = 250;
static const glm::vec2 MAX_VELOCITY = glm::vec2(300.0f, 1000.0f);
static const glm::vec2 VELOCITY_THRESHOLD = glm::vec2(0.01f, 0.01f);
//time it takes to increase the insanity again (in seconds)
static const float SANITY_SPEED = 0.2;
static const float ORB_SANITY = 0.06;
static const int STOMPER_SPEED_MIN = 600;
static const int STOMPER_SPEED_MAX = 700;
static const float MAX_SANITY = 15.0f;
static const float MAX_JUMP = 0.25f;
static const float PARALLAXITY = 4.0;
static const float TEXT_DURATION = 2.0f;
static const int DEATH_MESSAGE_COUNT = 13;
static const int PREGENERATE = 1200;
static const int GAME_BOTTOM = 400;
static const std::string DEATH_MESSAGES[DEATH_MESSAGE_COUNT] = {
    "Noob",
    "Lol",
    "Wow.",
    "Fail",
    "Rofl",
    "NEW LOWSCORE!",
    "Cyka Blyat",
    "Bad.",
    "Just... Wow.",
    "Seriously?",
    "Dude...",
    "R.I.P.",
    "You tried..."
};
#endif
