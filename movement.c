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

int mouseMove(Display *display, Window *window, int x, int y)
{
	return XWarpPointer(display, None, *window, 0, 0, 0, 0, x, y) == BadWindow;
}

int moveAndClick(Display *display, Window *window, int x, int y)
{
	return moveAndClickInner(display, window, x, y, Button1);
}

int moveAndRightClick(Display *display, Window *window, int x, int y)
{
	return moveAndClickInner(display, window, x, y, Button3);
}

int moveAndClickInner(Display *display, Window *window, int x, int y, int button)
{
	if (mouseMove(display, window, x, y))
	{
		return 1;
	}
	XTestFakeButtonEvent(display, button, True, 0);
	XTestFakeButtonEvent(display, button, False, 0);

	XFlush(display);
	XSync(display, False);

	return 0;
}
