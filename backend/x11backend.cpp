#include "x11backend.h"

void X11Backend::init() {
	printf("initializing x11 window... \n");
	display = XOpenDisplay(nullptr);
	if (!display) {
		printf("Couldn't open display\n");
		return;
	}
	int screen = DefaultScreen(display);
	if (!XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo)) {
        printf("ARGB32 not supported\n");
        return;
    }
	XSetWindowAttributes attr;
	attr.background_pixmap = None;
    attr.colormap = XCreateColormap(display, RootWindow(display, screen), vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0;
    attr.event_mask = ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask | PointerMotionMask | StructureNotifyMask;
	window = XCreateWindow(
			display,
            RootWindow(display, screen),
            100,
            100,
            1,
            1,
            0,
            vinfo.depth,
            InputOutput,
            vinfo.visual,
            CWColormap |
            CWBorderPixel |
            CWBackPixmap |
            CWEventMask,
            &attr);
	gc = XCreateGC(display, window, 0, nullptr);
    XStoreName(display, window, "Utopia Engine");
    XMapWindow(display, window);
}

void X11Backend::drawCanvas() {
	if (!backend_canvas.image) {
		printf("No image to display\n");
		return;
	}
	for (int y = 0; y < backend_canvas.height; ++y) {
		memcpy(
			shm.shmaddr + static_cast<size_t>(y) * dst_stride,
			reinterpret_cast<const char*>(backend_canvas.image) + static_cast<size_t>(y) * src_stride,
			src_stride
		);
	}		
	XShmPutImage(
		display, window, gc, ximg,
		0, 0, 0, 0,
		backend_canvas.width, backend_canvas.height, False
	);
	XFlush(display);	
}

void X11Backend::getNextEvent() {
		XNextEvent(display, &event);
		if (event.type == Expose) {
			drawCanvas();
		}
		if (event.type == MotionNotify) {
			events_array[0] = event.xmotion.x;
            events_array[1] = event.xmotion.y;
            drawCanvas();
		}
		if (event.type == ButtonPress) {
			if (event.xbutton.button == Button1) events_array[2] = event.xbutton.button;
			if (event.xbutton.button == Button2) events_array[3] = event.xbutton.button;
			if (event.xbutton.button == Button3) events_array[4] = event.xbutton.button;
			if (event.xbutton.button == Button4) events_array[5] = event.xbutton.button;
			if (event.xbutton.button == Button5) events_array[6] = event.xbutton.button;
			drawCanvas();
		}
		if (event.type == ButtonRelease) {
			if (event.xbutton.button == Button1) events_array[2] = !event.xbutton.button;
			if (event.xbutton.button == Button2) events_array[3] = !event.xbutton.button;
			if (event.xbutton.button == Button3) events_array[4] = !event.xbutton.button;
			if (event.xbutton.button == Button4) events_array[5] = !event.xbutton.button;
			if (event.xbutton.button == Button5) events_array[6] = !event.xbutton.button;
			drawCanvas();
		}
}

void X11Backend::setCanvasImage(CanvasImage canvas)
{
		ximg = XShmCreateImage(
		display,
		vinfo.visual,
		vinfo.depth,
		ZPixmap,
		nullptr,
		&shm,
		canvas.width,
		canvas.height
	);
	if (!ximg) {
		printf("Couldn't create shared memory XImage\n");
		return;
	}
	const size_t shm_size = static_cast<size_t>(ximg->bytes_per_line) * ximg->height;
	shm.shmid = shmget(IPC_PRIVATE, shm_size, IPC_CREAT | 0600);
	void* addr = shmat(shm.shmid, nullptr, 0);
	if (addr == reinterpret_cast<void*>(-1)) {
		perror("shmat");
		return;
	}
	shm.shmaddr = static_cast<char*>(addr);
	shm.readOnly = False;
	ximg->data = shm.shmaddr;
	XShmAttach(display, &shm);
	XSync(display, False);
	shmctl(shm.shmid, IPC_RMID, nullptr);

	src_stride = static_cast<size_t>(canvas.width) * sizeof(uint32_t);
	dst_stride = static_cast<size_t>(ximg->bytes_per_line);
	
	XSizeHints hints = {0};
	hints.flags = PMinSize | PMaxSize;
	hints.min_width  = canvas.width;
	hints.max_width  = canvas.width;
	hints.min_height = canvas.height;
	hints.max_height = canvas.height;
	XSetWMNormalHints(display, window, &hints);
    XResizeWindow(display, window, canvas.width, canvas.height);
    XFlush(display);
    
    backend_canvas = canvas;
}
