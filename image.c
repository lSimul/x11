#include "image.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>

int findImage(COORDS *result, BITMAP *needle, X_INSTANCE *instance)
{
	XImage *haystack = XGetImage(instance->display, instance->window, 0, 0, instance->attrs.width, instance->attrs.height, AllPlanes, ZPixmap);
	if (needle->height == 1)
	{
		return findImageRow(result, needle, haystack);
	}
	return findComplexImage(result, needle, haystack, instance);
}

int findImageRow(COORDS *result, BITMAP *needle, XImage *haystack)
{
	int matched = 0;
	for (int y = 0; y < haystack->height; y++)
	{
		for (int x = 0; x < haystack->width; x++)
		{
			unsigned long pixel = XGetPixel(haystack, x, y);
			if (comparePixel(haystack, needle->data[matched], pixel))
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

int findComplexImage(COORDS *result, BITMAP *needle, XImage *haystack, X_INSTANCE *instance)
{
	for (int y = 0; y < haystack->height; y++)
	{
		for (int x = 0; x < haystack->width; x++)
		{
			unsigned long pixel = XGetPixel(haystack, x, y);
			if (comparePixel(haystack, needle->data[0], pixel))
			{
				if (x + needle->width > instance->attrs.width || y + needle->height > instance->attrs.height)
				{
					continue;
				}

				if (!findComplexImageInner(result, needle, haystack, x, y))
				{
					return 0;
				}
			}
		}
	}
	return 1;
}

int findComplexImageInner(COORDS *result, BITMAP *needle, XImage *haystack, int offsetX, int offsetY)
{
	for (int y = 0; y < needle->height; y++)
	{
		for (int x = 0; x < needle->width; x++)
		{
			unsigned long pixel = XGetPixel(haystack, x + offsetX, y + offsetY);
			if (!comparePixel(haystack, needle->data[x + y], pixel))
			{
				return 1;
			}
		}
	}
	return 0;
}

static inline int comparePixel(XImage *img, PIXEL reference, unsigned long read)
{
	unsigned char blue = read & img->blue_mask;
	unsigned char green = (read & img->green_mask) >> 8;
	unsigned char red = (read & img->red_mask) >> 16;

	return blue == reference.blue && green == reference.green && red == reference.red;
}
