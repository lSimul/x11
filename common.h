#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#ifndef _COMMON_H_
#define _COMMON_H_

typedef struct coords
{
	int x;
	int y;
} COORDS;

typedef struct x_instance
{
	Display *display;
	Window window;
	XWindowAttributes attrs;
} X_INSTANCE;

#endif
