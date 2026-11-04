#pragma once

#include "displaybackend.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
#include <pixman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

class X11Backend : public DisplayBackend{
	public:
		X11Backend() = default;
		~X11Backend() override = default;
		void init() override;
		void drawCanvas() override;
		void getNextEvent() override;
		void setCanvasImage(CanvasImage canvas) override;
	private:
		Display* display = nullptr;
		Window window = 0;
		GC gc = 0;
		XVisualInfo vinfo;
		XImage* ximg = nullptr;
		size_t src_stride = 0;
		size_t dst_stride = 0;
		XShmSegmentInfo shm = {};
		XEvent event;
		CanvasImage backend_canvas = {};
};
