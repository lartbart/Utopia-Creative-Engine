#include "image.h"

void Image::loadPNG(const char* file) {
	FILE* fp = fopen(file, "rb");

    if (!fp) {
        printf("The png doesn't exist");
        return;
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (setjmp(png_jmpbuf(png))) { 
		fclose(fp); 
		return;
	}

    png_init_io(png, fp);
    png_read_info(png, info);

    this->width = png_get_image_width(png, info);
    this->height = png_get_image_height(png, info);

    png_set_expand(png);
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    png_read_update_info(png, info);

    unsigned char* data = new unsigned char[this->width * this->height * 4];
    png_bytep* rows = new unsigned char*[this->height];
    for (int i = 0; i < this->height; i++) rows[i] = data + i * this->width * 4;
    png_read_image(png, rows);

    for (int i = 0; i < this->width * this->height * 4; i += 4) {
        unsigned char r = data[i + 0];
        unsigned char g = data[i + 1];
        unsigned char b = data[i + 2];
        unsigned char a = data[i + 3];

        float alpha_factor = a / 255.0f;
        r = (unsigned char)(r * alpha_factor);
        g = (unsigned char)(g * alpha_factor);
        b = (unsigned char)(b * alpha_factor);

        data[i + 0] = b;
        data[i + 1] = g;
        data[i + 2] = r;
        data[i + 3] = a;
    }

	this->image_data =  reinterpret_cast<uint32_t*>(data);
}
