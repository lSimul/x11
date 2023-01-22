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
 * @param command
 */
void C_execute(Display *display, COMMAND *command);

/**
 * @brief
 *
 * @param command
 */
void C_press(Display *display, COMMAND *command);

/**
 * @brief
 *
 * @param command
 */
void C_pressSequence(Display *display, COMMAND *command);

/**
 * @brief
 *
 * @param command
 */
void C_pressCommand(Display *display, COMMAND *command);

/**
 * @brief
 *
 * @param command
 */
void C_click(Display *display, COMMAND *command);

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

	Display *display = XOpenDisplay(NULL);
	C_execute(display, c);

	return 0;
}

void C_execute(Display *display, COMMAND *command)
{
	while (command != NULL)
	{
		TOKEN t = command->tokens[0];
		if (t.type == PRESS)
		{
			C_press(display, command);
		}
		else if (t.type == CLICK)
		{
			C_click(display, command);
		}

		command = command->next;
		// TODO: Sleeps are really annoying, but they
		// fix the problem.
		sleep(1);
	}
}

void C_press(Display *display, COMMAND *command)
{
	TOKEN t = command->tokens[1];
	if (t.type == SEQUENCE)
	{
		C_pressSequence(display, command);
	}
	else if (t.type = T_COMMAND)
	{
		C_pressCommand(display, command);
	}
	else
	{
		printf("Unknown action %d\n", t.type);
	}
}

void C_pressSequence(Display *display, COMMAND *command)
{
	for (int i = 0; i < command->tokens[2].text.length; i++)
	{
		char c = command->tokens[2].text.data[i];
		if (c >= 'a' && c <= 'z' || c == '-')
		{
			pressKey(display, c);
		}
		else if (c >= 'A' && c <= 'Z')
		{
			EVENT *q = press(display, NULL, XK_Shift_L);
			q = press(display, q, c);
			release(q);
		}
	}
}

KeySym readSequenceToKeySym(const char read[5], int readCount);

void C_pressCommand(Display *display, COMMAND *command)
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
					q = press(display, q, ks);
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
					q = press(display, q, ks);
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

void C_click(Display *display, COMMAND *command)
{
	click(display);
}
