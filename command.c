#include "command.h"

#include <stdlib.h>

COMMAND *newCommand()
{
	COMMAND *c = malloc(sizeof(*c));
	c->next = NULL;
	c->length = 0;
	c->capacity = COMMAND_STARTING_SIZE;
	c->tokens = malloc(c->capacity * sizeof(*c->tokens));

	return c;
}

void enlargeCommands(COMMAND *command)
{
	TOKEN *tokens = malloc(command->capacity * 2 * sizeof(*tokens));
	for (unsigned int i = 0; i < command->length; i++)
	{
		tokens[i] = command->tokens[i];
	}
	command->capacity *= 2;

	command->tokens = tokens;
}
