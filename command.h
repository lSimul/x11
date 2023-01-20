#include "token.h"

#ifndef _COMMAND_H_
#define _COMMAND_H_

#define COMMAND_STARTING_SIZE 2

typedef struct command
{
	struct command *next;

	int length;
	int capacity;
	TOKEN *tokens;
} COMMAND;

/**
 * @brief
 *
 * Command is allocated on the heap, because
 * they can be chained into the queue.
 * Queue is the most natural structure for the
 * execution of the commands.
 *
 * @return COMMAND*
 */
COMMAND *newCommand();

/**
 * @brief
 *
 * @param command
 */
void enlargeCommands(COMMAND *command);

#endif
