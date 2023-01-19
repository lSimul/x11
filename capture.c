#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

/**
 * Main source:
 *
 * https://stackoverflow.com/questions/8249669/how-do-take-a-screenshot-correctly-with-xlib
 *
 * It goes nicely from top-left to the bottom-right, column-wise.
 * Bytes how they are right now fit the top of the
 *
 * !!! Important note !!!
 * It does not start from position 0, 0 for me, but from 1, 1 for some reason.
 * No need to be sad or something like that, one pixel is really nothing, but for some
 * hardcore evaluation over multiple lines or fixed position it can cause some troubles.
 *
 * @return int
 */
int main()
{
	Display *display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}

	Window root = XRootWindow(display, 0);

	XImage *img = XGetImage(display, root, 0, 0, 10, 10, AllPlanes, ZPixmap);

	printf("%dx%d\n", img->width, img->height);
	unsigned int sum = 0;
	for (int x = 0; x < img->width; x++)
	{
		for (int y = 0; y < img->height; y++)
		{
			unsigned long pixel = XGetPixel(img, x, y);

			unsigned char blue = pixel & img->blue_mask;
			unsigned char green = (pixel & img->green_mask) >> 8;
			unsigned char red = (pixel & img->red_mask) >> 16;

			printf("%d:\nR:\t%d\nG:\t%d\nB:\t%d\n", ++sum, red, green, blue);
		}
	}

	XCloseDisplay(display);
	return 0;
}
