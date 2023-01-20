#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <unistd.h>

void pressKey(Display *display, KeySym);

int main()
{
	Display *display = XOpenDisplay(NULL);
	pressKey(display, XK_A);
}

void pressKey(Display *display, KeySym key)
{
	unsigned int keycode = XKeysymToKeycode(display, key);
	XTestFakeKeyEvent(display, keycode, True, CurrentTime);
	XTestFakeKeyEvent(display, keycode, False, CurrentTime);
	XFlush(display);
	XSync(display, False);
}
