#include "canvas.h"

Canvas::Canvas() {
	this->canvas_image.width = 800;
	this->canvas_image.height = 600;
	this->pixels.reserve(canvas_image.width * canvas_image.height);
	this->canvas_image.image = pixels.data();
}

Canvas& Canvas::getInstance() {
        static Canvas instance;
        return instance;
}

void Canvas::setCanvasSize(int width, int height) {
	this->canvas_image.width = width;
	this->canvas_image.height = height;
}

void Canvas::drawNonPersistentCanvas(VisualObject* object) {
		pixels.assign(static_cast<size_t>(canvas_image.width) * canvas_image.height, 0);
		pixman_image_t* dest = pixman_image_create_bits(
		PIXMAN_a8r8g8b8,
		this->canvas_image.width,
		this->canvas_image.height,
		pixels.data(),
		this->canvas_image.width * 4
	);

	pixman_image_t* dest2 = pixman_image_create_bits(
		PIXMAN_a8r8g8b8,
		object->image.width,
		object->image.height,
		object->image.image_data,
		object->image.width * 4
	);
	
	pixman_image_composite32(
		PIXMAN_OP_SRC,
		dest2,
		nullptr,
		dest,
		0, 0,
		0, 0,
		object->x, object->y,
		object->image.width,
		object->image.height
	);

	this->canvas_image.image = pixels.data();

	pixman_image_unref(dest);
	pixman_image_unref(dest2);
}
