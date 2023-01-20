#include "grammer.h"

int fileToCommands(const char *file, COMMAND **command)
{
	READER r = {};
	if (openFile(&r, file))
	{
		return 1;
	}

	COMMAND *c = newCommand();
	COMMAND *root = c;

	while (1)
	{
		initToken(&(c->tokens[c->length]));
		if (toToken(&r, &(c->tokens[c->length])))
		{
			break;
		}

		textToType(&(c->tokens[c->length]));

		if (c->tokens[c->length].type == SEPARATOR)
		{
			if (c->length > 0)
			{
				COMMAND *newC = newCommand();
				c->next = newC;
				c = newC;
			}
			continue;
		}

		if (++c->length == c->capacity)
		{
			enlargeCommands(c);
		}
	}

	c = root;
	while (c != NULL)
	{
		if (grammerCheck(c))
		{
			printf("There is something wrong with this command.\n");
			return 1;
		}

		if (c->next != NULL && c->next->length == 0)
		{
			free(c->next->tokens);
			free(c->next);
			c->next = NULL;
		}
		c = c->next;
	}

	*command = root;

	return 0;
}

int toToken(READER *reader, TOKEN *token)
{
	char c;
	while (1)
	{
		c = getChar(reader);
		if (c == EOF)
		{
			return token->type == EMPTY;
		}

		if (c == '\n')
		{
			if (token->type == EMPTY)
			{
				token->type = SEPARATOR;
				return 0;
			}
			else
			{
				ungetChar(reader, c);
				return 0;
			}
		}

		if (c == ' ')
		{
			if (token->type == EMPTY)
			{
				continue;
			}
			return 0;
		}

		appendToString(&token->text, c);
		token->type = TEXT;
	}
	return 1;
}

int grammerCheck(COMMAND *c)
{
	// Edge case caused by hungry addition of the
	// next command.
	// NOOP should be fine, atlast for now.
	if (c->length <= 0)
	{
		return 0;
	}
	TOKEN t = c->tokens[0];
	if (t.type == PRESS)
	{
		if (c->length != 3)
		{
			return 1;
		}
		t = c->tokens[1];
		if (t.type != SEQUENCE && t.type != T_COMMAND)
		{
			return 1;
		}
		if (c->tokens[2].type != TEXT)
		{
			return 1;
		}
	}
	else if (t.type == FIND)
	{
		if (c->length != 2)
		{
			return 1;
		}
		else if (c->tokens[1].type != TEXT)
		{
			return 1;
		}
	}
	else if (t.type == MOVE)
	{
		if (c->length != 2)
		{
			return 1;
		}
		else if (c->tokens[1].type != MOUSE)
		{
			return 1;
		}
	}
	else if (t.type == CLICK)
	{
		if (c->length > 1)
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}

	return 0;
}
