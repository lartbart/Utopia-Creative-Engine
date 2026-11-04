#pragma once
#include "object.h"
#include "image.h"

class VisualObject : public Object {
		public:
			VisualObject() = default;
			~VisualObject() = default;
			int x, y, z;
			void setPosition(int x, int y, int z = 1);
			Image image;
};
