#pragma once

#include "object.h"
#include "eventserver.h"
#include <vector>
#include <functional>

class Scene {
		public:
			Scene() = default;
			~Scene() = default;
			std::vector<Object*> scene_objects = {};
			std::function<void(const Event&)> run;
};
