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

typedef struct coords
{
	int x;
	int y;
} COORDS;

/**
 * @brief
 *
 * @param result
 * @param needle
 * @param display
 * @param window
 * @return int
 */
int findImage(COORDS *result, BITMAP *needle, Display *display, Window *window);

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
		"images/line.bmp",
		"images/terminal.bmp",
	};
	for (int i = 0; i < STEP_COUNT; i++)
	{
		if (readBitmap(&bitmap, steps[i]))
		{
			printf("Fail while reading '%s'.\n", steps[i]);
			return 1;
		}

		COORDS c = {};

		if (findImage(&c, &bitmap, display, &root))
		{
			free(bitmap.data);
			printf("Failed finding stuff in '%s'\n", steps[i]);
			break;
		}
		free(bitmap.data);

		if (moveAndClick(display, &root, c.x, c.y))
		{
			printf("Failed moving and clicking mouse.\n");
			break;
		}

		while (XPending(display))
		{
			sleep(1);
		}
		// I need active waiting for some reason.
		sleep(1);
	}

	XCloseDisplay(display);
	return 0;
}

int findImage(COORDS *result, BITMAP *needle, Display *display, Window *window)
{
	XWindowAttributes attributes;
	XGetWindowAttributes(display, *window, &attributes);

	XImage *haystack = XGetImage(display, *window, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);

	int matched = 0;
	for (int y = 0; y < haystack->height; y++)
	{
		for (int x = 0; x < haystack->width; x++)
		{
			unsigned long pixel = XGetPixel(haystack, x, y);
			if (comparePixels(haystack, needle->data[matched], pixel))
			{
				if (++matched == needle->size)
				{
					result->x = x;
					result->y = y;
					return 0;
				}
			}
			else
			{
				matched = 0;
			}
		}
	}
	return 1;
}

static inline int comparePixels(XImage *img, PIXEL reference, unsigned long read)
{
	unsigned char blue = read & img->blue_mask;
	unsigned char green = (read & img->green_mask) >> 8;
	unsigned char red = (read & img->red_mask) >> 16;

	return blue == reference.blue && green == reference.green && red == reference.red;
}
