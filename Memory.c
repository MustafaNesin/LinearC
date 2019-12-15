#include <stdlib.h>
#include "Console.h"
#include "Matrix.h"
#include "Menu.h"

Memory* init_mem()
{
	Memory* memory = malloc(sizeof(Memory) + CMD_COUNT * sizeof(void*));
	if (!memory)
		return NULL;

	Menu* home = calloc(1, sizeof(Menu));
	if (!home)
	{
		free(memory);
		return NULL;
	}
	Command** commands = malloc(CMD_COUNT * sizeof(Command*));
	if (!commands)
	{
		free(home);
		free(memory);
		return NULL;
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
			return NULL;
		}
	}

	home->title = HOME_TITLE;

	home->options[0] = HOME_OPT0;
	home->options[1] = HOME_OPT1;
	home->options[2] = HOME_OPT2;
	home->options[3] = HOME_OPT3;
	home->options[4] = HOME_OPT4;
	home->options[5] = HOME_OPT5;
	home->options[6] = HOME_OPT6;

	home->functions[0] = NULL;
	home->functions[1] = menu_define;
	home->functions[2] = menu_list;
	home->functions[3] = menu_console;
	home->functions[4] = menu_equation;
	home->functions[5] = menu_save;
	home->functions[6] = menu_load;

	commands[0]->name = "return";
	commands[0]->help = CMD_HELP_RETURN;
	commands[0]->function = NULL;

	commands[1]->name = "help";
	commands[1]->help = CMD_HELP_HELP;
	commands[1]->function = cmd_help;

	commands[2]->name = "clear";
	commands[2]->help = CMD_HELP_CLEAR;
	commands[2]->function = cmd_clear;

	commands[3]->name = "list";
	commands[3]->help = CMD_HELP_LIST;
	commands[3]->function = cmd_list;

	commands[4]->name = "print";
	commands[4]->help = CMD_HELP_PRINT;
	commands[4]->function = cmd_print;

	commands[5]->name = "define";
	commands[5]->help = CMD_HELP_DEFINE;
	commands[5]->function = cmd_define;

	commands[6]->name = "delete";
	commands[6]->help = CMD_HELP_DELETE;
	commands[6]->function = cmd_delete;

	commands[7]->name = "transpose";
	commands[7]->help = CMD_HELP_TRANSPOSE;
	commands[7]->function = cmd_transpose;

	memory->tail = NULL;
	memory->matrix = NULL;
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

Node* mem_new(Memory* memory, char name, int rows, int cols, float* data)
{
	Matrix* matrix = mx_new(rows, cols, data);
	if (!matrix)
		return NULL;

	Node* node = mem_add(memory, name, matrix);
	if (!node)
	{
		free(matrix);
		return NULL;
	}

	return node;
}

Node* mem_add(Memory* memory, char name, Matrix* matrix)
{
	if (!memory)
		return NULL;

	Node* node = calloc(1, sizeof(Node));
	if (!node)
		return NULL;

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
		return NULL;

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

	mx_free(node->matrix);
	free(node);
}