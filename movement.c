#include "movement.h"

#include <X11/extensions/XTest.h>

int click(Display *display)
{
	XTestFakeButtonEvent(display, Button1, True, 0);
	XTestFakeButtonEvent(display, Button1, False, 0);

	XFlush(display);
	XSync(display, False);
	return 0;
}

int mouseMove(Display *display, Window *root, int x, int y)
{
	return XWarpPointer(display, None, *root, 0, 0, 0, 0, x, y) == BadWindow;
}

int moveAndClick(Display *display, Window *root, int x, int y)
{
	if (mouseMove(display, root, x, y))
	{
		return 1;
	}
	XTestFakeButtonEvent(display, Button1, True, 0);
	XTestFakeButtonEvent(display, Button1, False, 0);

	XFlush(display);
	XSync(display, False);

	return 0;
}
