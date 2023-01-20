// Contains pixel.h, too
#include "file.h"
#include "movement.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#define STEP_COUNT 2

/**
 * @brief
 *
 * @param img
 * @param reference
 * @param read
 * @return int
 */
static inline int comparePixels(XImage *img, PIXEL reference, unsigned long read);

/**
 * Main source of screen managing:
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
 * Read screen and based on the sequence of pixels
 * click on the position. (Just sequence, context is not checked.)
 *
 * @return int
 */
int main()
{
	BITMAP bitmap = {};

	Display *display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}
	Window root = XDefaultRootWindow(display);
	XWindowAttributes attributes;
	XGetWindowAttributes(display, root, &attributes);

	static const char *steps[STEP_COUNT] = {
		"line.bmp",
		"terminal.bmp",
	};
	for (int i = 0; i < STEP_COUNT; i++)
	{
		if (readFile(&bitmap, steps[i]))
		{
			printf("Fail while reading '%s'.\n", steps[i]);
			return 1;
		}

		XImage *img = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);
		int matched = 0;
		int success = 0;
		for (int y = 0; y < img->height; y++)
		{
			for (int x = 0; x < img->width; x++)
			{
				unsigned long pixel = XGetPixel(img, x, y);
				if (comparePixels(img, bitmap.data[matched], pixel))
				{
					if (++matched == bitmap.size)
					{
						if (moveAndClick(display, &root, x, y))
						{
							goto NEXT;
						}
						success = 1;
						goto NEXT;
					}
				}
				else
				{
					matched = 0;
				}
			}
		}
	NEXT:
		free(bitmap.data);

		if (!success)
		{
			printf("Failed finding stuff in '%s'\n", steps[i]);
			goto END;
		}

		while (XPending(display))
		{
			sleep(1);
		}
		// I need active waiting for some reason.
		sleep(1);
	}

END:
	XCloseDisplay(display);
	return 0;
}

static inline int comparePixels(XImage *img, PIXEL reference, unsigned long read)
{
	unsigned char blue = read & img->blue_mask;
	unsigned char green = (read & img->green_mask) >> 8;
	unsigned char red = (read & img->red_mask) >> 16;

	return blue == reference.blue && green == reference.green && red == reference.red;
}
