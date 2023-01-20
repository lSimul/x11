#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include <stdlib.h>
#include <unistd.h>

void pressKey(Display *display, KeySym);

typedef struct event
{
	Display *display;
	KeyCode keycode;
	struct event *previous;
} EVENT;

EVENT *press(Display *display, EVENT *previous, KeySym key);

void release(EVENT *event);

int main()
{
	Display *display = XOpenDisplay(NULL);
	pressKey(display, XK_A);

	EVENT *q = press(display, NULL, XK_Control_L);
	q = press(display, q, XK_Alt_L);
	q = press(display, q, XK_T);

	release(q);
}

void pressKey(Display *display, KeySym key)
{
	unsigned int keycode = XKeysymToKeycode(display, key);
	XTestFakeKeyEvent(display, keycode, True, CurrentTime);
	XTestFakeKeyEvent(display, keycode, False, CurrentTime);
	XFlush(display);
	XSync(display, False);
}

EVENT *press(Display *display, EVENT *previous, KeySym key)
{
	unsigned int code = XKeysymToKeycode(display, key);
	XTestFakeKeyEvent(display, code, True, CurrentTime);

	EVENT *e = malloc(sizeof(*e));
	e->display = display;
	e->keycode = code;
	e->previous = previous;

	return e;
}

void release(EVENT *event)
{
	if (event == NULL)
	{
		return;
	}

	XTestFakeKeyEvent(event->display, event->keycode, False, CurrentTime);
	if (event->previous == NULL)
	{
		XFlush(event->display);
		XSync(event->display, False);
	}
	else
	{
		release(event->previous);
	}
	free(event);
}
