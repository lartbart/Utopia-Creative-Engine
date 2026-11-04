#pragma once

#include "stdint.h"
#include <vector>
#include "../core/canvas.h"

class DisplayBackend {
	public:
		DisplayBackend() = default;
		virtual ~DisplayBackend() = default;
		virtual void init() = 0;
		virtual void drawCanvas() = 0;
		virtual void getNextEvent() = 0;
		virtual void setCanvasImage(CanvasImage canvas) = 0;
		std::vector<int> events_array = {0};
};
