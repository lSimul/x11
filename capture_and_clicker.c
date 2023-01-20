// Contains pixel.h, too
#include "file.h"

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
 * @brief
 *
 * @param display
 * @param root
 * @param x
 * @param y
 * @return int
 */
int mouseMove(Display *display, Window *root, int x, int y);

/**
 * @brief
 *
 * Inspiration for a mouse click comes from
 * https://stackoverflow.com/questions/8767524/how-do-we-simulate-a-mouse-click-with-xlib-c
 *
 * @param display
 * @param root
 * @param x
 * @param y
 * @return int
 */
int moveAndClick(Display *display, Window *root, int x, int y);

/**
 * Combination of capture.c and clicker.c
 *
 * Read screen and based on the sequence of pixels
 * click on the position. (Just sequence, context is not checked.)
 *
 * Crucial combination of some parts, these two operations will
 * be a backbone for everything I'll be doing later on.
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
