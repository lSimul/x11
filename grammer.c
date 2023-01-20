#include "file.h"

#include <string.h>

typedef enum type
{
	EMPTY,

	TEXT,

	// "PRESS"
	PRESS,

	// "SEQUENCE"
	SEQUENCE,

	// COMMAND is used twice here, as a struct.
	// "COMMAND",
	T_COMMAND,

	// "FIND"
	FIND,

	// "MOVE"
	MOVE,

	// "MOUSE"
	MOUSE,

	// "CLICK"
	CLICK,

	SEPARATOR,
} TYPE;

#define STARTING_SIZE 2

typedef struct string
{
	int length;
	int capacity;
	char *data;
} STRING;

typedef struct token
{
	TYPE type;

	STRING text;
} TOKEN;

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

/**
 * @brief
 *
 * @param token
 */
void initToken(TOKEN *token);

/**
 * @brief
 *
 * @param reader
 * @param token
 * @return int
 */
int toToken(READER *reader, TOKEN *token);

/**
 * @brief
 *
 * @param token
 */
void textToType(TOKEN *token);

/**
 * @brief
 *
 * @param token
 */
void printToken(TOKEN *token);

/**
 * @brief
 *
 * @param s
 * @param c
 */
void appendToString(STRING *s, char c);

/**
 * @brief
 *
 * @param string
 */
void enlargeString(STRING *string);

int main()
{
	READER r = {};
	openFile(&r, "commands.txt");

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
		for (int i = 0; i < c->length; i++)
		{
			printToken(&c->tokens[i]);
		}
		printf("\n");
		c = c->next;
	}
}

COMMAND *newCommand()
{
	COMMAND *c = malloc(sizeof(*c));
	c->next = NULL;
	c->length = 0;
	c->capacity = STARTING_SIZE;
	c->tokens = malloc(STARTING_SIZE * sizeof(*c->tokens));

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

void initToken(TOKEN *token)
{
	token->type = EMPTY;

	STRING s = {};
	s.length = 0;
	s.capacity = STARTING_SIZE;
	s.data = malloc(STARTING_SIZE * sizeof(*s.data));
	token->text = s;
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

void appendToString(STRING *s, char c)
{
	s->data[s->length++] = c;

	if (s->capacity == s->length)
	{
		enlargeString(s);
	}
}

void enlargeString(STRING *string)
{
	char *data = malloc(string->capacity * 2 * sizeof(*data));
	for (unsigned int i = 0; i < string->length; i++)
	{
		data[i] = string->data[i];
	}
	string->capacity *= 2;

	string->data = data;
}
