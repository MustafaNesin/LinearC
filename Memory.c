#include <stdlib.h>
#include "Command.h"
#include "Menu.h"

Memory* init_mem()
{
	Memory* memory = malloc(sizeof(Memory) + CMD_COUNT * sizeof(void*));
	if (!memory)
		return 0;
	Menu* home = calloc(1, sizeof(Menu));
	if (!home)
	{
		free(memory);
		return 0;
	}
	Command** commands = malloc(CMD_COUNT * sizeof(Command*));
	if (!commands)
	{
		free(home);
		free(memory);
		return 0;
	}
	for (int cmd = 0; cmd < CMD_COUNT; cmd++)
	{
		commands[cmd] = malloc(sizeof(Command));
		if (!commands[cmd])
		{
			for (int i = cmd - 1; i >= 0; i--)
				free(commands[i]);
			free(commands);
			free(home);
			free(memory);
			return 0;
		}
	}

	home->title = "LineerC | Mustafa Nesin & Cem Ufuk Yilmaz";

	home->options[0] = "Cikis";
	home->options[1] = "Yeni Matris";
	home->options[2] = "Matris Listesi";
	home->options[3] = "Islemler";
	home->options[4] = "Denklem Cozucu";

	home->functions[0] = NULL;
	home->functions[1] = menu_define;
	home->functions[2] = menu_list;
	home->functions[3] = menu_console;
	home->functions[4] = menu_equation;

	commands[0]->name = "return";
	commands[0]->help = "Ana menuye dondurur.";
	commands[0]->function = NULL;

	commands[1]->name = "help";
	commands[1]->help = "Komutlar hakkinda bilgi verir.\n\tKullanim: help\n\tKullanim: help [KOMUT]\n\tOrnek: help\n\tOrnek: help list";
	commands[1]->function = cmd_help;

	commands[CMD_CLEAR]->name = "clear";
	commands[CMD_CLEAR]->help = "Konsolu temizler.";
	commands[CMD_CLEAR]->function = cmd_clear;

	commands[3]->name = "list";
	commands[3]->help = "Tanimli matrisleri listeler.";
	commands[3]->function = cmd_list;

	commands[4]->name = "print";
	commands[4]->help = "Matrisi gosterir.\n\tKullanim: print([MATRIS])\n\tOrnek: print(A)";
	commands[4]->function = cmd_print;

	memory->tail = NULL;
	memory->home = home;
	memory->commands = commands;

	return memory;
}

void free_mem(Memory* memory)
{
	if (!memory)
		return;

	while (memory->tail)
		mem_remove(memory, memory->tail);

	for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		free(memory->commands[cmd]);

	free(memory->commands);
	free(memory->home);
	free(memory);
}

Node* mem_new(Memory* memory, char name, Matrix* matrix)
{
	if (!memory)
		return 0;

	Node* node = calloc(1, sizeof(Node));
	if (!node)
		return 0;

	node->name = name;
	node->matrix = matrix;

	if (memory->tail)
	{
		memory->tail->next = node;
		node->prev = memory->tail;
	}

	return memory->tail = node;
}

Node* mem_search(Memory* memory, char name)
{
	if (!memory)
		return 0;

	Node* node = memory->tail;
	while (node)
	{
		if (node->name == name)
			break;
		node = node->prev;
	}
	return node;
}

void mem_remove(Memory* memory, Node* node)
{
	if (!memory || !node)
		return;

	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		memory->tail = node->prev;

	free(node->matrix->data);
	free(node->matrix);
	free(node);
}