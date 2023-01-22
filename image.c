#include "image.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

int findImage(COORDS *result, BITMAP *needle, X_INSTANCE *instance)
{
	XImage *haystack = XGetImage(instance->display, instance->window, 0, 0, instance->attrs.width, instance->attrs.height, AllPlanes, ZPixmap);

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
