#ifndef COMPONENT_DRAWABLE_HPP
#define COMPONENT_DRAWABLE_HPP

#include "entityx/entityx.h"

#include "animation.hpp"

#include <SDL2/SDL.h>

struct Drawable : entityx::Component<Drawable> {
    Drawable(std::string key, float width, float height, AnimationCollection animation = { "" }):
        textureKey(key),
        height(height),
        width(width),
        animation(animation) {
    }

    float getHeight() {
        return height;
    }

    float getWidth() {
        return width;
    }

    void setHeight(float height) {
        this->height = height;
    }

    void setWidth(float width) {
        this->width = width;
    }

    std::string texture_key() {
        return textureKey;
    }

    bool hasAnimation() {
        return !animation.getTextureKey().empty();
    }

    auto getAnimation() {
        return animation;
    }

  private:
    std::string textureKey;
    float height;
    float width;
    AnimationCollection animation;
};
#endif
