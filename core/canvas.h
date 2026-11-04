#pragma once

#include "pixman.h"
#include "stdlib.h"
#include "stdio.h"
#include <vector>
#include "visualobject.h"


typedef struct  {
	uint32_t* image;
	int width;
	int height;
} CanvasImage;

class Canvas {
	public:	
		~Canvas() = default;
		static Canvas& getInstance();
		void drawNonPersistentCanvas(VisualObject* object);
		CanvasImage canvas_image;
		std::vector<uint32_t> pixels;
		void setCanvasSize(int width, int height);
	private:
		Canvas();
};
