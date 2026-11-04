#pragma once

#include "stdint.h"
#include "stdlib.h"
#include "png.h"
#include "object.h"

class Image : public Object {
	public:
		Image() = default;
		~Image() = default;
		uint32_t* image_data = nullptr;
		int width = 0;
		int height = 0;
		void loadPNG(const char* file);
};
