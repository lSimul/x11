#include <stdio.h>
#include <stdlib.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/**
 * @brief Everything copied from https://stackoverflow.com/questions/20581343/how-to-move-the-mouse-cursor-from-user-code,
 * tweaked pretty heavily.
 *
 * The position is relative, now it moves cursor 200 px to the right-down coridor.
 *
 * https://tronche.com/gui/x/xlib/input/XWarpPointer.html
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

	XWarpPointer(displayMain, None, None, 0, 0, 0, 0, x, y);

	XCloseDisplay(displayMain);

	return 0;
}

int main()
{
	return mouseMove(200, 200);
}
