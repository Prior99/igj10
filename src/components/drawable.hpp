#ifndef COMPONENT_DRAWABLE_HPP
#define COMPONENT_DRAWABLE_HPP

#include "entityx/entityx.h"

#include "animation.hpp"

#include <SDL2/SDL.h>

struct Drawable : entityx::Component<Drawable> {
    Drawable(std::string key, float width, float height, AnimationCollection animation = { "" }, glm::vec2 offset = glm::vec2(0, 0)):
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

    glm::vec2 getOffset() {
        return this->offset;
    }

    void setOffset(glm::vec2 offset) {
        this->offset = offset;
    }

    std::string texture_key() {
        return textureKey;
    }

    double getRotation() {
        return rotation;
    }

    void setRotation(double r) {
        this->rotation = r;
    }

    bool hasAnimation() {
        return !animation.getTextureKey().empty();
    }

    AnimationCollection& getAnimation() {
        return animation;
    }

  private:
    std::string textureKey;
    float height;
    float width;
    AnimationCollection animation;
    double rotation;
    glm::vec2 offset;
};
#endif
