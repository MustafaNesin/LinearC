#include <stdio.h>
#include <stdint.h>
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
	home->functions[6] = menu_read;

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

	commands[7]->name = "get";
	commands[7]->help = CMD_HELP_GET;
	commands[7]->function = cmd_get;

	commands[8]->name = "set";
	commands[8]->help = CMD_HELP_SET;
	commands[8]->function = cmd_set;

	commands[9]->name = "isequal";
	commands[9]->help = CMD_HELP_ISEQUAL;
	commands[9]->function = cmd_isequal;

	commands[10]->name = "transpose";
	commands[10]->help = CMD_HELP_TRANSPOSE;
	commands[10]->function = cmd_transpose;

	commands[11]->name = "add";
	commands[11]->help = CMD_HELP_ADD;
	commands[11]->function = cmd_add;

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

	mem_remove_all(memory);

	for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		free(memory->commands[cmd]);

	free(memory->commands);
	free(memory->home);
	free(memory);
}

Node* mem_new(Memory* memory, char name, uint8_t rows, uint8_t cols, float* data)
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

Node* mem_query(Memory* memory, char name)
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

void mem_remove_all(Memory* memory)
{
	while (memory->tail)
		mem_remove(memory, memory->tail);
}

int mem_save(Memory* memory)
{
	FILE* file = fopen(FILE_NAME, "wb");

	if (!file)
		return -1;

	int p;
	uint8_t count = 0, row, col;
	fseek(file, sizeof(count), SEEK_SET);

	Node* node = memory->tail;

	while (node)
	{
		fwrite(&node->name, 1, 1, file);
		fwrite(&node->matrix->rows, 1, 1, file);
		fwrite(&node->matrix->cols, 1, 1, file);

		for (row = 0, p = 0, count++; row < node->matrix->rows; row++)
			for (col = 0; col < node->matrix->cols; col++, p++)
				fwrite(node->matrix->data + p, sizeof(float), 1, file);

		node = node->prev;
	}

	fseek(file, 0, SEEK_SET);
	fwrite(&count, sizeof(count), 1, file);
	fclose(file);

	return count;
}

int mem_read(Memory* memory)
{
	FILE* file = fopen(FILE_NAME, "rb");

	if (!file)
		return -1;

	mem_remove_all(memory);

	uint8_t count;
	if (fread(&count, sizeof(count), 1, file) != 1)
		return -1;

	char name;
	int p;
	uint8_t rows, cols, row, col;
	float* data;

	for (uint8_t c = 0; c < count; c++)
	{
		if (fread(&name, sizeof(name), 1, file) != 1)
		{
			mem_remove_all(memory);
			return -1;
		}

		if (fread(&rows, sizeof(rows), 1, file) != 1)
		{
			mem_remove_all(memory);
			return -1;
		}

		if (fread(&cols, sizeof(cols), 1, file) != 1)
		{
			mem_remove_all(memory);
			return -1;
		}

		data = malloc(rows * cols * sizeof(float));
		if (!data)
		{
			mem_remove_all(memory);
			return -1;
		}

		for (row = 0, p = 0; row < rows; row++)
			for (col = 0; col < cols; col++, p++)
			{
				if (fread(data + p, sizeof(float), 1, file) != 1)
				{
					free(data);
					mem_remove_all(memory);
					return -1;
				}
			}

		mem_new(memory, name, rows, cols, data);
	}

	fclose(file);
	return count;
}