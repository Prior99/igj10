#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

#include <iostream>

enum struct AnimationPlaybackType {
	LOOP,
	RESET,
	FREEZE
};

struct Animation {
	int offset;
	int frames;
	double duration;
	glm::vec2 size;

	double runTime;

	Animation():
		offset(0),
		frames(0),
		duration(0),
		size({0, 0}),
		runTime(0) {
		std::cout << "construct\n";
	}

	Animation(int offset, int frames, double duration, glm::vec2 size):
		offset(offset),
		frames(frames),
		duration(duration),
		size(size),
		runTime(0) {

	}
};

struct AnimationCollection {
	AnimationCollection(std::string textureKey): textureKey(textureKey) {

	}

	void addAnimation(std::string animationKey, int offset, int frames, double duration, glm::vec2 size) {
		Animation animation = {offset, frames, duration, size};
		animations[animationKey] = animation;
	}

	void setAnimation(std::string animationKey, AnimationPlaybackType playbackType) {
		if (!currentAnimation.empty()) {
			std::cout << "reset animation\n";
			animations[currentAnimation].runTime = 0;
		}
		this->playbackType = playbackType;
		currentAnimation = animationKey;
	}

	void pause(bool pause) {
		paused = pause;
	}

	std::string getTextureKey() {
		return textureKey;
	}

	SDL_Rect* getAnimationFrame(SDL_Rect* rect) {
		if (animations.count(currentAnimation)) {
			auto animation = animations[currentAnimation];

			int currentFrame = animation.runTime * animation.frames / animation.duration;
			std::cout << animation.runTime << " * " << animation.frames << " / " << animation.duration << " = " << currentFrame << "\n";
			rect->x = currentFrame * animation.size.x;
			rect->y = animation.offset;
			rect->w = animation.size.x;
			rect->h = animation.size.y;
			return rect;
		} else {
			return NULL;
		}
	}

	void update(double dt) {
		auto animation = animations[currentAnimation];
		if(!paused) {
			std::cout << "runtime: " << animation.runTime << " + " << dt << " = ";
			animation.runTime += dt;
			std::cout << animation.runTime << "\n";
		}
		if (animation.runTime > animation.duration) {
			std::cout << "end\n";
			switch(playbackType) {
				case AnimationPlaybackType::RESET:
					animation.runTime = 0;
					currentAnimation.erase();
					break;
				case AnimationPlaybackType::FREEZE:
					animation.runTime = animation.duration;
					paused = true;
					break;
				case AnimationPlaybackType::LOOP:
					// Nothing to do here
					break;
			}
		}
		animations[currentAnimation] = animation;
		std::cout << paused << " " << animation.runTime << "\n";
	}

  private:
	std::string textureKey;
	std::map<std::string, Animation> animations;
	std::string currentAnimation;
	AnimationPlaybackType playbackType;
	bool paused = false;
};

#endif
