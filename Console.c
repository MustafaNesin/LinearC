#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Console.h"
#include "Utils.h"
#include "Menu.h"
#include "Matrix.h"

void parse_command(char* input, Parsed* parsed)
{
	char* o_input = input;
	parsed->name = NULL;
	parsed->argcount = 0;

	if (!*input)
		return;

	int state = 1;
	char f = 0;

	loop:
	if (*input < 0 || input - o_input >= CON_BUFFER_SIZE)
	{
		parsed->argcount = 0;
		return;
	}
	switch (state)
	{
		case 1: // ws begin
			if (isspace(*input))
				input++;
			else
			{
				parsed->name = input;
				state++;
			}
			break;
		case 2: // name
			if (islower(*input) || *input == '_')
				input++;
			else
				state++;
			break;
		case 3: // ws middle
			if (isspace(*input))
				*input++ = '\0';
			else
				state++;
			break;
		case 4: // (
			if (*input == '(')
			{
				*input++ = '\0';
				// parsed->argcount 0 iken
				f = 0;
				parsed->args[parsed->argcount] = input;
				state++;
			}
			else
				return;
			break;
		case 5: // args
			if (!*input)
			{
				parsed->argcount = 0;
				return;
			}
			if (isspace(*input))
			{
				*input++ = '\0';
				if (f)
					state++;
				else
					parsed->args[parsed->argcount]++;
			}
			else if (*input == ',')
			{
				if (!f)
				{
					// Boþ argümanlar kabul edilemez
					parsed->argcount = 0;
					return;
				}
				*input++ = '\0';
				if (++parsed->argcount == MAX_PARAM_COUNT)
				{
					parsed->argcount = 0;
					return;
				}
				f = 0;
				parsed->args[parsed->argcount] = input;
			}
			else if (*input == ')')
			{
				if (!f)
				{
					// Boþ argümanlar kabul edilemez
					parsed->argcount = 0;
					return;
				}
				*input = '\0';
				parsed->argcount++;
				return;
			}
			else
			{
				input++;
				f = 1;
			}
			break;
		case 6: // ws ,
			if (isspace(*input))
				input++;
			else if (*input == ',')
				state--;
			else
			{
				// argümanlarýn içerisinde boþluk karakteri bulunamaz
				parsed->argcount = 0;
				return;
			}
			break;
	}
	goto loop;
}

Matrix* test_command(CMD_FUNC_PARAMS)
{
	Node* node;
	if (parsed->argcount)
	{
		if (!(node = mem_search(memory, *parsed->args[0])))
		{
			printf("%s adinda bir matris bulunamadi.\n\n", parsed->args[0]);
			return NULL;
		}
		return node->matrix;
	}
	if (!memory->matrix)
	{
		printf("Konsolda bellege alinmis bir matris yok.\n\n");
		return NULL;
	}
	return memory->matrix;
}

void cmd_help(CMD_FUNC_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_FUNC_PARAMS_FAIL;

	if (!parsed->argcount)
	{
		for (int cmd = 0; cmd < CMD_COUNT; cmd++)
			printf("%s\n", memory->commands[cmd]->help);
		printf("\n");
		return;
	}

	for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		if (!strcmp(parsed->args[0], memory->commands[cmd]->name))
		{
			printf("%s\n\n", memory->commands[cmd]->help);
			return;
		}
	printf("Komut bulunamadi: %s\n\n", parsed->args[0]);
}

void cmd_clear(CMD_FUNC_PARAMS)
{
	if (parsed->argcount)
		CMD_FUNC_PARAMS_FAIL;

	clear();
}

void cmd_list(CMD_FUNC_PARAMS)
{
	if (parsed->argcount)
		CMD_FUNC_PARAMS_FAIL;

	int count = 0;
	Node* node = memory->tail;
	while (node)
	{
		printf("%2d. %c %dx%d\n", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("%d adet matris bulundu.\n\n", count);
}

void cmd_print(CMD_FUNC_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_FUNC_PARAMS_FAIL;

	Matrix* matrix = test_command(memory, parsed);

	if (!matrix)
		return;

	if (parsed->argcount)
	{
		if (memory->matrix)
			mx_free(memory->matrix);

		memory->matrix = mx_copy(matrix);

		if (memory->matrix)
			matrix = memory->matrix;
		else
			printf("%c matrisi bellege kopyalanamadi.", *parsed->args[0]);
	}

	mx_print(matrix);
}

void cmd_define(CMD_FUNC_PARAMS)
{
	if (!parsed->argcount || parsed->argcount > 5)
		CMD_FUNC_PARAMS_FAIL;

	char name = *parsed->args[0];

	if (*(parsed->args[0] + 1))
	{
		printf("Tanimlanacak olan matris degiskeninin adi tek harften olusmalidir.\n\n");
		return;
	}

	if (name < 'A' || name > 'Z')
	{
		printf("Matris adi buyuk bir harf olmalidir. (Yalnizca Ingiliz alfabesi harfleri)\n\n");
		return;
	}

	if (mem_search(memory, name))
	{
		printf("%c adinda bir matris zaten var.\n\n", name);
		return;
	}

	Matrix* matrix;

	if (parsed->argcount == 1)
	{
		if (!memory->matrix)
		{
			printf("Konsolda bellege alinmis bir matris yok.\n\n");
			return;
		}
		matrix = mx_copy(memory->matrix);
		if (!matrix)
		{
			printf("Matris tanimlanamadi.\n\n");
			return;
		}
	}

	else if (parsed->argcount == 5)
	{
		if (strcmp(parsed->args[1], "all"))
			CMD_FUNC_PARAMS_FAIL;

		int rows, cols;
		float value;

		(void)sscanf(parsed->args[2], "%f", &value);
		(void)sscanf(parsed->args[3], "%d", &rows);
		(void)sscanf(parsed->args[4], "%d", &cols);

		if (rows < MIN_MATRIX_SIZE || rows > MAX_MATRIX_SIZE)
		{
			printf("Matris satir sayisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			return;
		}

		if (cols < MIN_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
		{
			printf("Matris sutun sayisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			return;
		}

		matrix = mx_create_all(rows, cols, value);
		if (memory->matrix)
			mx_free(memory->matrix);
		memory->matrix = mx_copy(matrix);
	}

	else
	{
		int size;
		float value;

		(void)sscanf(parsed->args[2], "%f", &value);
		(void)sscanf(parsed->args[3], "%d", &size);

		if (size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE)
		{
			printf("Matris satir ve sutun sayisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			return;
		}

		if (!strcmp(parsed->args[1], "diag"))
			matrix = mx_create_diag(size, value);
		else if (!strcmp(parsed->args[1], "low"))
			matrix = mx_create_low(size, value);
		else if (!strcmp(parsed->args[1], "up"))
			matrix = mx_create_up(size, value);
		else
			CMD_FUNC_PARAMS_FAIL;

		if (memory->matrix)
			mx_free(memory->matrix);
		memory->matrix = mx_copy(matrix);
	}

	if (!mem_add(memory, name, matrix))
	{
		free(matrix->data);
		free(matrix);
		printf("Matris tanimlanamadi.\n\n");
		return;
	}

	mx_print(matrix);
}

void cmd_delete(CMD_FUNC_PARAMS)
{
	if (parsed->argcount != 1)
		CMD_FUNC_PARAMS_FAIL;

	if (!strcmp(parsed->args[0], "all"))
	{
		while (memory->tail)
			mem_remove(memory, memory->tail);
		printf("Tanimlanmis tum matrisler silindi.\n\n");
		return;
	}

	Node* node;
	if (!(node = mem_search(memory, *parsed->args[0])))
	{
		printf("%s adinda bir matris bulunamadi.\n\n", parsed->args[0]);
		return;
	}

	mem_remove(memory, node);
	printf("%c matrisi silindi.\n\n", *parsed->args[0]);
}

void cmd_transpose(CMD_FUNC_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_FUNC_PARAMS_FAIL;

	Matrix* matrix = test_command(memory, parsed);

	if (!matrix)
		return;

	matrix = mx_transpose(matrix);
	if (memory->matrix)
		mx_free(memory->matrix);
	memory->matrix = matrix;
	mx_print(matrix);
}

void cmd_add(CMD_FUNC_PARAMS)
{
	if (!parsed->argcount || parsed->argcount > 2)
		CMD_FUNC_PARAMS_FAIL;

	Matrix *matrix1, *matrix2, *sum;
	if (parsed->argcount == 1)
	{
		if (!memory->matrix)
		{
			printf("Konsolda bellege alinmis bir matris yok.\n\n");
			return;
		}
		Node* node = mem_search(memory, *parsed->args[0]);
		if (!node)
		{
			printf("%s adinda bir matris bulunamadi.\n\n", parsed->args[0]);
			return;
		}

		matrix1 = memory->matrix;
		matrix2 = node->matrix;
	}
	else
	{
		Node* node1 = mem_search(memory, *parsed->args[0]);
		if (!node1)
		{
			printf("%s adinda bir matris bulunamadi.\n\n", parsed->args[0]);
			return;
		}
		Node* node2 = mem_search(memory, *parsed->args[1]);
		if (!node2)
		{
			printf("%s adinda bir matris bulunamadi.\n\n", parsed->args[1]);
			return;
		}

		matrix1 = node1->matrix;
		matrix2 = node2->matrix;
	}

	if (matrix1->rows != matrix2->rows || matrix1->cols != matrix2->cols)
	{
		printf("Toplama yapabilmek icin matrislerin boyutlari birbirine esit olmalidir.\n\n");
		return;
	}

	sum = mx_add(matrix1, matrix2);
	if (memory->matrix)
		mx_free(memory->matrix);
	memory->matrix = sum;
	mx_print(sum);
}