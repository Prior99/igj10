#ifndef COMPONENT_MULTIPART_DRAWABLE_HPP
#define COMPONENT_MULTIPART_DRAWABLE_HPP

#include "entityx/entityx.h"

struct PartialDrawable {
    std::string texture;
    int height;
    AnimationCollection animation;

    PartialDrawable(std::string texture, int height): texture(texture), height(height), animation("") {

    }

    PartialDrawable(std::string texture, int height, AnimationCollection animation):
        texture(texture),
        height(height),
        animation(animation) {

    }
};

struct MultipartDrawable : entityx::Component<MultipartDrawable> {
    MultipartDrawable(int width, PartialDrawable top, PartialDrawable middle, PartialDrawable bottom):
        top(top),
        middle(middle),
        bottom(bottom),
        totalHeight(top.height + bottom.height),
        width(width) {
    }

    PartialDrawable getTop() {
        return top;
    }

    PartialDrawable getMiddle() {
        return middle;
    }

    PartialDrawable getBottom() {
        return bottom;
    }

    double getRepeat() {
        return (totalHeight - top.height - bottom.height) / middle.height;
    }

    void setHeight(int height) {
        totalHeight = height;
        int minHeight = top.height + bottom.height;
        if(totalHeight < minHeight) {
            totalHeight = minHeight;
        }
    }

    void addHeight(int height) {
        setHeight(totalHeight + height);
    }

    int getWidth() {
        return width;
    }

    int getHeight() {
        return totalHeight;
    }

  private:
    PartialDrawable top;
    PartialDrawable middle;
    PartialDrawable bottom;
    int totalHeight;
    int width;
};
#endif
