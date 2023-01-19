#include "pixel.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

#define NEEDLE_LENGTH 16

static inline int comparePixels(XImage *img, PIXEL reference, unsigned long read);

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
	// Content of the red rectangle in the capture_and_clicker.bmp
	static PIXEL line[NEEDLE_LENGTH] = {
		{0x00, 0x42, 0xa5},
		{0x00, 0x44, 0xaa},
		{0x00, 0x44, 0xaa},
		{0xcf, 0xdc, 0xef},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0xff, 0xff, 0xff},
		{0x77, 0x9b, 0xd1},
		{0x00, 0x44, 0xaa},
		{0x00, 0x44, 0xaa},
		{0x00, 0x42, 0xa5},
	};

	Display *display = XOpenDisplay(NULL);
	if (display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}

	Window root = XRootWindow(display, 0);

	XWindowAttributes attributes;
	XGetWindowAttributes(display, root, &attributes);

	XImage *img = XGetImage(display, root, 0, 0, attributes.width, attributes.height, AllPlanes, ZPixmap);

	int matched = 0;
	for (int y = 0; y < img->height; y++)
	{
		for (int x = 0; x < img->width; x++)
		{
			unsigned long pixel = XGetPixel(img, x, y);
			if (comparePixels(img, line[matched], pixel))
			{
				if (++matched == NEEDLE_LENGTH)
				{
					printf("Everything found!\n");
					mouseMove(display, &root, x, y);

					XTestFakeButtonEvent(display, Button1, True, 0);
					XTestFakeButtonEvent(display, Button1, False, 0);

					XCloseDisplay(display);
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
