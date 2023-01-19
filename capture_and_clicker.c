// Contains pixel.h, too
#include "file.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

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
 * Combination of capture.c and clicker.c
 *
 * Read screen and based on the sequence of pixels
 * click on the position. (Just sequence, context is not checked.)
 *
 * Crucial combination of some parts, these two operations will
 * be a backbone for everything I'll be doing later on.
 *
 * Plus for a mouse click https://stackoverflow.com/questions/8767524/how-do-we-simulate-a-mouse-click-with-xlib-c
 * was an essential part.
 */
int main()
{
	BITMAP bitmap = {};
	readFile(&bitmap, "line.bmp");

	Display *display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}

	Window root = XDefaultRootWindow(display);

	XWindowAttributes attributes;
	XGetWindowAttributes(display, root, &attributes);

	XImage *img = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);
	int matched = 0;
	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			unsigned long pixel = XGetPixel(img, x, y);
			if (comparePixels(img, bitmap.data[matched], pixel))
			{
				if (++matched == bitmap.size)
				{
					mouseMove(display, &root, x, y);

					XTestFakeButtonEvent(display, Button1, True, 0);
					XTestFakeButtonEvent(display, Button1, False, 0);


					XFlush(display);
					XSync(display, False);
					goto END;
				}
			}
			else
			{
				matched = 0;
			}
		}
	}

END:

	while (XPending(display))
	{
		sleep(1);
	}
	// I need active waiting for some reason.
	sleep(1);
	if (readFile(&bitmap, "terminal.bmp"))
	{
		printf("fail\n");
	}

	img = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);

	matched = 0;
	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			unsigned long pixel = XGetPixel(img, x, y);
			if (comparePixels(img, bitmap.data[matched], pixel))
			{
				// printf("%d\n", matched + 1);
				if (++matched == bitmap.size)
				{
					// For some reason it was clicking outside.
					// bitmap.width is solving it to some degree,
					// but it looks still way too off.
					mouseMove(display, &root, x - bitmap.width, y);

					XTestFakeButtonEvent(display, Button1, True, 0);
					XTestFakeButtonEvent(display, Button1, False, 0);

					XFlush(display);
					XSync(display, False);

					return 0;
				}
			}
			else
			{
				matched = 0;
			}
		}
	}

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
	if (XWarpPointer(display, None, *root, 0, 0, 0, 0, x, y) == BadWindow)
	{
		return 1;
	}
	XFlush(display);
	// XSync(display, False);

	return 0;
}
