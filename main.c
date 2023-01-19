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
int mouseMove(int x, int y)
{
	Display *displayMain = XOpenDisplay(NULL);

	if (displayMain == NULL)
	{
		printf("No display found.\n");
		return 1;
	}

	Window root = XRootWindow(displayMain, 0);

	XWarpPointer(displayMain, None, root, 0, 0, 0, 0, x, y);

	XCloseDisplay(displayMain);

	return 0;
}

int main()
{
	for (unsigned int i = 200; i < 1000; i += 15)
	{
		if (mouseMove(i, i) == 1)
		{
			return 1;
		}
		sleep(1);
	}
	return 0;
}
