#include "common.h"
#include "image.h"
#include "grammer.h"
#include "keys.h"
#include "movement.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// C_ stands for COMMAND.
// `press` is also in the keys.h

/**
 * @brief
 *
 * @param instance
 * @param command
 */
void C_execute(X_INSTANCE *instance, COMMAND *command);

/**
 * @brief
 *
 * @param instance
 * @param command
 */
void C_press(X_INSTANCE *instance, COMMAND *command);

/**
 * @brief
 *
 * @param instance
 * @param command
 */
void C_pressSequence(X_INSTANCE *instance, COMMAND *command);

/**
 * @brief
 *
 * @param instance
 * @param command
 */
void C_pressCommand(X_INSTANCE *instance, COMMAND *command);

/**
 * @brief
 *
 * @param instance
 * @param command
 * @param coords
 * @return int
 */
int C_findImage(X_INSTANCE *instance, COMMAND *command, COORDS *coords);

/**
 * @brief
 *
 * @param instance
 * @param command
 */
void C_click(X_INSTANCE *instance, COMMAND *command);

int main(int argc, const char **argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <list of commands>\n", argv[0]);
		return 1;
	}

	COMMAND *c = NULL;
	if (fileToCommands(argv[1], &c))
	{
		printf("Error while reading the file or one of the commands.\n");
		return 1;
	}

	/*
	while (c != NULL)
	{
		for (int i = 0; i < c->length; i++)
		{
			printToken(&c->tokens[i]);
		}
		printf("\n");
		c = c->next;
	}
	*/

	X_INSTANCE instance = {};
	instance.display = XOpenDisplay(NULL);
	if (instance.display == NULL)
	{
		printf("No display found.\n");
		return 1;
	}
	// Used only in C_findImage, but there is no
	// fear of initialiazing it anytime.
	instance.window = XDefaultRootWindow(instance.display);
	XGetWindowAttributes(instance.display, instance.window, &instance.attrs);

	C_execute(&instance, c);

	return 0;
}

void C_execute(X_INSTANCE *instance, COMMAND *command)
{
	COORDS coords = {};

	while (command != NULL)
	{
		TOKEN t = command->tokens[0];
		if (t.type == PRESS)
		{
			C_press(instance, command);
		}
		else if (t.type == CLICK)
		{
			C_click(instance, command);
		}
		else if (t.type == FIND)
		{
			if (C_findImage(instance, command, &coords))
			{
				printf("Image not found.\n");
				coords.x = -1;
				coords.y = -1;
			}
		}
		else if (t.type == MOVE)
		{
			// Now it does not even need an extra function.
			// No checks for the next argument MOUSE.
			if (coords.x == -1 && coords.y == -1)
			{
				printf("No idea where to move.\n");
			}
			else
			{
				mouseMove(instance->display, &instance->window, coords.x, coords.y);
			}
		}

		command = command->next;
		// TODO: Sleeps are really annoying, but they
		// fix the problem.
		sleep(1);
	}
}

void C_press(X_INSTANCE *instance, COMMAND *command)
{
	TOKEN t = command->tokens[1];
	if (t.type == SEQUENCE)
	{
		C_pressSequence(instance, command);
	}
	else if (t.type = T_COMMAND)
	{
		C_pressCommand(instance, command);
	}
	else
	{
		printf("Unknown action %d\n", t.type);
	}
}

void C_pressSequence(X_INSTANCE *instance, COMMAND *command)
{
	for (int i = 0; i < command->tokens[2].text.length; i++)
	{
		char c = command->tokens[2].text.data[i];
		if (c >= 'a' && c <= 'z' || c == '-')
		{
			pressKey(instance->display, c);
		}
		else if (c >= 'A' && c <= 'Z')
		{
			EVENT *q = press(instance->display, NULL, XK_Shift_L);
			q = press(instance->display, q, c);
			release(q);
		}
	}
}

KeySym readSequenceToKeySym(const char read[5], int readCount);

void C_pressCommand(X_INSTANCE *instance, COMMAND *command)
{
	EVENT *q = NULL;

	// TODO: Ignoring upper bound, segmentation fault
	// is possible.
	char read[5];
	int readCount = 0;

	int i = 0;
	STRING s = command->tokens[2].text;

	// Very hungry parser, ignoring everything, what it does not like.
	while (1)
	{
		if (s.length == i)
		{
			if (readCount > 0)
			{
				int ks = readSequenceToKeySym(read, readCount);
				if (ks > 0)
				{
					q = press(instance->display, q, ks);
				}
			}
			break;
		}

		char c = s.data[i];
		if (c == '-')
		{
			if (readCount > 0)
			{
				int ks = readSequenceToKeySym(read, readCount);
				if (ks > 0)
				{
					q = press(instance->display, q, ks);
				}
				for (int j = 0; j < 5; j++)
				{
					read[j] = 0;
				}
				readCount = 0;
			}
		}
		else if (c >= 'A' && c <= 'Z')
		{
			read[readCount++] = c;
		}
		i++;
	}

	if (q != NULL)
	{
		release(q);
	}
}

KeySym readSequenceToKeySym(const char read[5], int readCount)
{
	if (readCount == 0)
	{
		return 0;
	}
	else if (readCount == 1)
	{
		return read[0];
	}

	if (strcmp(read, "CTR") == 0)
	{
		return XK_Control_L;
	}
	else if (strcmp(read, "ALT") == 0)
	{
		return XK_Alt_L;
	}
	return 0;
}

int C_findImage(X_INSTANCE *instance, COMMAND *command, COORDS *c)
{
	const char *src = command->tokens[1].text.data;

	BITMAP bitmap = {};
	if (readBitmap(&bitmap, src))
	{
		printf("Fail while reading '%s'.\n", src);
		return 1;
	}

	if (findImage(c, &bitmap, instance))
	{
		free(bitmap.data);
		printf("Failed finding stuff in '%s'\n", src);
		return 1;
	}
	free(bitmap.data);

	return 0;
}

void C_click(X_INSTANCE *instance, COMMAND *command)
{
	click(instance->display);
}
