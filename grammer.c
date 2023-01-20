#include "file.h"

typedef enum type
{
	EMPTY,

	TEXT,

	PRESS,
	SEQUENCE,
	// COMMAND,
	FIND,
	MOVE,
	MOUSE,
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
	int length;
	int capacity;
	TOKEN *tokens;
} COMMAND;

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

	COMMAND c = {};
	c.length = 0;
	c.capacity = STARTING_SIZE;
	c.tokens = malloc(STARTING_SIZE * sizeof(*c.tokens));

	while (1)
	{
		initToken(&(c.tokens[c.length]));
		if (toToken(&r, &(c.tokens[c.length])))
		{
			break;
		}

		textToType(&(c.tokens[c.length]));

		if (c.tokens[c.length].type == SEPARATOR)
		{
			printf("Whole command parsed.\n");
			break;
		}

		if (++c.length == c.capacity)
		{
			enlargeCommands(&c);
		}
	}

	for (int i = 0; i < c.length; i++)
	{
		printf("%s\n", c.tokens[i].text.data);
	}
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
	token->type = TEXT;
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
