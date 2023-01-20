#include "grammer.h"
#include "keys.h"

#include <stdio.h>
#include <stdlib.h>

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

		command = command->next;
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

void C_pressCommand(Display *display, COMMAND *command)
{
	// TODO: Parse CTR to XK_Control_{L|R} etc.
}
