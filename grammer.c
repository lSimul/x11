#include "file.h"

int main()
{
	READER r = {};
	openFile(&r, "commands.txt");

	char c;
	while ((c = getChar(&r)) != EOF)
	{
		printf("%c\n", c);
	}
}
