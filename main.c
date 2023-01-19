#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/**
 * @brief Everything copied from https://stackoverflow.com/questions/20581343/how-to-move-the-mouse-cursor-from-user-code,
 * tweaked pretty heavily.
 *
 * https://tronche.com/gui/x/xlib/input/XWarpPointer.html
 *
 * Another source is:
 *
 * https://github.com/misusi/x11-keyboard-mouse-control/blob/master/xkbmouse.c
 *
 * Code is terrible, but it shows the way, how to use absolute positioning.
 *
 * @param x
 * @param y
 * @return
 */
int mouseMove(Display *display, Window *root, int x, int y)
{
	if (XWarpPointer(display, None, *root, 0, 0, 0, 0, x, y) == BadWindow)
	{
		return 1;
	}
	XFlush(display);
	// XSync(display, False);

	return 0;
}

int main()
{
	Display *display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}

	Window root = XRootWindow(display, 0);

	for (unsigned int i = 200; i < 1000; i += 15)
	{
		if (mouseMove(display, &root, i, i) == 1)
		{
			XCloseDisplay(display);
			return 1;
		}
		sleep(1);
	}
	XCloseDisplay(display);
	return 0;
}
