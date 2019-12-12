#include <stdio.h>
#include <Windows.h>
#include "Main.h"

int main()
{
	Memory memory = { 0 };

	while (parse_line(memory));

	free_memory(&memory);

	return 0;
}

int parse_line(Memory memory)
{
	int input, cont = 1;

	input = getchar();
	if (input == EOF)
		return 0;
	if (input == '\n')
		return cont;

	// To be continued...
}