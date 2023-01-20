#include "grammer.h"

#include <stdio.h>
#include <stdlib.h>

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

	while (c != NULL)
	{
		for (int i = 0; i < c->length; i++)
		{
			printToken(&c->tokens[i]);
		}
		printf("\n");
		c = c->next;
	}

	return 0;
}
