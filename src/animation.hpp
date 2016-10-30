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
    }

    Animation(int offset, int frames, double duration, glm::vec2 size):
        offset(offset),
        frames(frames),
        duration(duration),
        size(size),
        runTime(0.0) {
        }
};

struct AnimationCollection {
    AnimationCollection(std::string textureKey): textureKey(textureKey) {

    }

    void addAnimation(std::string animationKey, int offset, int frames, double duration, glm::vec2 size) {
        animations[animationKey] = new Animation(offset, frames, duration, size);
    }

    void setAnimation(std::string animationKey, AnimationPlaybackType playbackType) {
        if (!currentAnimation.empty()) {
            animations[currentAnimation]->runTime = 0;
        }
        this->playbackType = playbackType;
        currentAnimation = animationKey;
    }

    void pause(bool pause) {
        paused = pause;
    }

    bool isRunning() {
        return paused;
    }

    std::string getTextureKey() {
        return textureKey;
    }

    SDL_Rect* getAnimationFrame(SDL_Rect* rect) {
        if (animations.count(currentAnimation)) {
            auto& animation = animations[currentAnimation];

            int currentFrame = (int)(animation->runTime * animation->frames / animation->duration) % animation->frames;
            rect->x = currentFrame * animation->size.x;
            rect->y = animation->offset;
            rect->w = animation->size.x;
            rect->h = animation->size.y;
            return rect;
        } else {
            return NULL;
        }
    }

    void update(double dt) {
        if(initialized() && !currentAnimation.empty()){
            auto& animation = animations[currentAnimation];
            std::cout << "initialized " << textureKey << "\n";
            if(!paused) {
                std::cout << "paused\n";
                animation->runTime += dt;
            }
            std::cout << "paused ende\n";
            if (animation->runTime >= animation->duration) {
                std::cout << "vor switch\n";
                switch(playbackType) {
                    case AnimationPlaybackType::RESET:
                        animation->runTime = 0;
                        currentAnimation.erase();
                        break;
                    case AnimationPlaybackType::FREEZE:
                        animation->runTime = animation->duration - dt;
                        paused = true;
                        break;
                    case AnimationPlaybackType::LOOP:
                        animation->runTime = 0;
                        break;
                }
                std::cout << "nach switch\n";
            }
        }
    }

    std::string getCurrentAnimation() {
        return this->currentAnimation;
    }

    bool initialized() {
        return !textureKey.empty();
    }

  private:
    std::string textureKey;
    std::map<std::string, Animation*> animations;
    std::string currentAnimation;
    AnimationPlaybackType playbackType;
    bool paused = false;
};

#endif
