#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

int main()
{
	Display *display = XOpenDisplay(NULL);
	unsigned int keycode = XKeysymToKeycode(display, XK_A);
	XTestFakeKeyEvent(display, keycode, True, 0);
	XTestFakeKeyEvent(display, keycode, False, 0);
	XFlush(display);
}
