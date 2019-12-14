#include <stdio.h>
#include <string.h>
#include "Command.h"
#include "Utils.h"
#include "Menu.h"

void cmd_help(Memory* memory, char* args)
{
	if (*args++)
	{
		for (int cmd = 0; cmd < CMD_COUNT; cmd++)
			if (!memcmp(args, memory->commands[cmd]->name, strlen(memory->commands[cmd]->name)))
			{
				printf("%s", memory->commands[cmd]->help);
				return;
			}
		printf("Komut bulunamadi: %s", args);
	}
	else
	{
		printf("Fonksiyonlar ve aciklamalari;");
		for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		{
			printf("\n\n");
			printf("%s", memory->commands[cmd]->name);
			printf(": %s", memory->commands[cmd]->help);
		}
	}
}

void cmd_clear(Memory* memory, char* args)
{
	clear();
}

void cmd_list(Memory* memory, char* args)
{
	if (!memory)
		return;

	int count = 0;
	Node* node = memory->tail;
	while (node)
	{
		printf("%2d. %c %dx%d\n", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("%d adet matris bulundu.", count);
}

void cmd_test(Memory* memory, char* args)
{
	printf("Hello world!");
}

void cmd_print(Memory* memory, char* args)
{
	// ...
}