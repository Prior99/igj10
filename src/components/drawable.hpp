#ifndef COMPONENT_DRAWABLE_HPP
#define COMPONENT_DRAWABLE_HPP

#include "entityx/entityx.h"

#include <SDL2/SDL.h>

struct Drawable : entityx::Component<Drawable> {
    Drawable(std::string key, float width, float height): textureKey(key), height(height), width(width) {
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

  private:
    std::string textureKey;
    float height;
    float width;
};
#endif
