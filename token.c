#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void initToken(TOKEN *token)
{
	token->type = EMPTY;

	STRING s = {};
	s.length = 0;
	s.capacity = TOKEN_STARTING_SIZE;
	s.data = malloc(s.capacity * sizeof(*s.data));
	token->text = s;
}

void textToType(TOKEN *token)
{
	if (token->type != TEXT)
	{
		return;
	}

	const char *str = token->text.data;
	if (strcmp(str, "PRESS") == 0)
	{
		token->type = PRESS;
	}
	else if (strcmp(str, "SEQUENCE") == 0)
	{
		token->type = SEQUENCE;
	}
	else if (strcmp(str, "COMMAND") == 0)
	{
		token->type = T_COMMAND;
	}
	else if (strcmp(str, "FIND") == 0)
	{
		token->type = FIND;
	}
	else if (strcmp(str, "MOVE") == 0)
	{
		token->type = MOVE;
	}
	else if (strcmp(str, "MOUSE") == 0)
	{
		token->type = MOUSE;
	}
	else if (strcmp(str, "CLICK") == 0)
	{
		token->type = CLICK;
	}
}

void printToken(TOKEN *t)
{
	if (t->type == TEXT)
	{
		printf("Unparsed text: %s\n", t->text.data);
	}
	else
	{
		printf("Command: %d\n", t->type);
	}
}
