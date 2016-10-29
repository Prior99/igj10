#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <glm/vec2.hpp>

#include <SDL2/SDL.h>

struct Animation {
	int offset;
	int frames;
	double duration;
	glm::vec2 size;

	double runTime = 0;
};


struct AnimationCollection {
	AnimationCollection(std::string textureKey): textureKey(textureKey) {

	}

	addAnimation(std::string animationKey, int offset, int frames, double duration, glm::vec2 size) {
		Animation animation = {offset, frames, duration, size};
		animations[animationKey] = animation;
	}

	setAnimation(std::string animationKey, bool repeat) {
		if (currentAnimation.empty()) {
			animations[currentAnimation].runTime = 0;
		}
		loop = repeat;
		currentAnimation = animationKey;
	}

	std::string getTextureKey() {
		return textureKey;
	}

	SDL_Rect* getAnimationFrame() {
		if (animations.count(currentAnimation)) {
			auto animation = animations[currentAnimation];
			SDL_Rect rect;
			int currentFrame = animation.runTime * animation.frames / animation.duration;
			rect.x = currentFrame * animation.size.x;
			rect.y = animation.offset;
			rect.w = animation.size.x;
			rect.h = animation.size.y;
			return &rect;
		} else {
			return NULL;
		}
	}

	update(double dt) {
		auto animation = animations[currentAnimation];
		animation.runTime += dt;
		if (!loop && animation.runTime >= animation.duration) {
			animation.runTime = 0;
			currentAnimation.erase();
		}
	}

  private:
	std::string textureKey;
	std::map<std::string, Animation> animations;
	std::string currentAnimation;
	bool loop;
};

#endif
