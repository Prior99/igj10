#ifndef COMPONENT_UNIMULTIPART_DRAWABLE_HPP
#define COMPONENT_UNIMULTIPART_DRAWABLE_HPP

#include "entityx/entityx.h"

#include <glm/vec2.hpp>

struct UniMultipartDrawable : entityx::Component<UniMultipartDrawable> {
    UniMultipartDrawable(std::string texture, glm::i32vec2 textureSize, int left, int right, int top, int bottom, glm::i32vec2 repetitions_):
        texture(texture),
        textureSize(textureSize),
        left(left),
        right(right),
        top(top),
        bottom(bottom),
        repititions(repetitions_) {
        }

    SDL_Rect *getLeftTopClip(SDL_Rect* rect) {
        rect->x = 0;
        rect->y = 0;
        rect->w = left;
        rect->h = top;
        return rect;
    }

    SDL_Rect *getLeftCenterClip(SDL_Rect* rect) {
        rect->x = 0;
        rect->y = top;
        rect->w = left;
        rect->h = textureSize.y - top - bottom;
        return rect;
    }

    SDL_Rect *getLeftBottomClip(SDL_Rect* rect) {
        rect->x = 0;
        rect->y = textureSize.y - bottom;
        rect->w = left;
        rect->h = bottom;
        return rect;
    }

    SDL_Rect *getRightTopClip(SDL_Rect* rect) {
        rect->x = textureSize.x - right;
        rect->y = 0;
        rect->w = right;
        rect->h = top;
        return rect;
    }

    SDL_Rect *getRightCenterClip(SDL_Rect* rect) {
        rect->x = textureSize.x - right;
        rect->y = top;
        rect->w = right;
        rect->h = textureSize.y - top - bottom;
        return rect;
    }

    SDL_Rect *getRightBottomClip(SDL_Rect* rect) {
        rect->x = textureSize.x - right;
        rect->y = textureSize.y - bottom;
        rect->w = right;
        rect->h = bottom;
        return rect;
    }

    SDL_Rect *getCenterTopClip(SDL_Rect* rect) {
        rect->x = left;
        rect->y = 0;
        rect->w = textureSize.x - left - right;
        rect->h = top;
        return rect;
    }

    SDL_Rect *getCenterClip(SDL_Rect* rect) {
        rect->x = left;
        rect->y = top;
        rect->w = textureSize.x - left - right;
        rect->h = textureSize.y - top - bottom;
        return rect;
    }

    SDL_Rect *getCenterBottomClip(SDL_Rect* rect) {
        rect->x = left;
        rect->y = textureSize.y - bottom;
        rect->w = textureSize.x - left - right;
        rect->h = bottom;
        return rect;
    }

    glm::i32vec2 getTextureSize() {
        return textureSize;
    }

    std::string getTexture() {
        return texture;
    }

    glm::i32vec2 getRepititions() {
        return repititions;
    }

    int getLeft() {
        return left;
    }

    int getRight() {
        return right;
    }

    int getTop() {
        return top;
    }

    int getBottom() {
        return bottom;
    }

    int getCenterX() {
        return textureSize.x - left - right;
    }

    int getCenterY() {
        return textureSize.y - top - bottom;
    }

    private:
    std::string texture;
    glm::i32vec2 textureSize;
    int left, right, top, bottom;
    glm::i32vec2 repititions;
};

#endif
