#ifndef COMPONENT_STACKED_DRAWABLE_HPP
#define COMPONENT_STACKED_DRAWABLE_HPP

#include "entityx/entityx.h"

#include "components/drawable.hpp"

#include <SDL2/SDL.h>

struct StackedDrawable : entityx::Component<StackedDrawable> {
	StackedDrawable(std::vector<Drawable> layers): layers(layers) {

	}

	std::vector<Drawable> &getLayers() {
		return layers;
	}

  private:
	std::vector<Drawable> layers;
};
#endif
