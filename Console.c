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

void get_one_mx(CMD_PARAMS, Matrix** matrix)
{
	*matrix = NULL;

	if (parsed->argcount > 0 && *parsed->args[0] >= 'A' && *parsed->args[0] <= 'Z')
	{
		Node* node = mem_query(memory, *parsed->args[0]);

		if (!node)
			CMD_SEARCH_FAIL(0);

		*matrix = node->matrix;
		return;
	}

	if (!memory->matrix)
		CMD_MATRIX_FAIL;

	*matrix = memory->matrix;
}

void get_two_mx(CMD_PARAMS, Matrix** matrix1, Matrix** matrix2)
{
	*matrix1 = *matrix2 = NULL;

	if (*parsed->args[1] >= 'A' && *parsed->args[1] <= 'Z')
	{
		Node* node1 = mem_query(memory, *parsed->args[0]);

		if (!node1)
			CMD_SEARCH_FAIL(0);

		Node* node2 = mem_query(memory, *parsed->args[1]);

		if (!node2)
			CMD_SEARCH_FAIL(1);

		*matrix1 = node1->matrix;
		*matrix2 = node2->matrix;
	}
	else
	{
		if (!memory->matrix)
			CMD_MATRIX_FAIL;

		Node* node = mem_query(memory, *parsed->args[0]);

		if (!node)
			CMD_SEARCH_FAIL(0);

		*matrix1 = memory->matrix;
		*matrix2 = node->matrix;
	}
}

void apply_op(CMD_PARAMS, uint8_t colmode)
{
	if (parsed->argcount == 2 || parsed->argcount > 5)
		CMD_PARAMS_FAIL;

	Matrix* matrix;
	get_one_mx(memory, parsed, &matrix);
	if (!matrix)
		return;

	if (parsed->argcount < 2)
	{
		Operation op = mx_next_op(matrix, colmode);
		char n = colmode ? 'c' : 'r';
		int v1 = op.vec1 + 1;
		int v2 = op.vec2 + 1;

		switch (op.type)
		{
			case OP_ADD:
				printf("\t%c%d -> %c%d + %g%c%d\n\n", n, v2, n, v2, op.coeff, n, v1);
				break;
			case OP_MULTIPLY:
				printf("\t%c%d -> %g%c%d\n\n", n, v1, op.coeff, n, v1);
				break;
			case OP_SWITCH:
				printf("\t%c%d <-> %c%d\n\n", n, v1, n, v2);
				break;
			default:
				printf("\tMatris zaten indirgenmis eselon formunda.\n\n");
				return;
		}
		mx_apply_op(matrix, op);
		mx_print(matrix);
		return;
	}

	uint8_t i = *parsed->args[0] >= 'A' && *parsed->args[0] <= 'Z';
	Operation op = { 0 };
	op.colmode = colmode;

	if (!strcmp(parsed->args[i], "switch"))
		op.type = OP_SWITCH;
	else if (!strcmp(parsed->args[i], "multiply"))
		op.type = OP_MULTIPLY;
	else if (!strcmp(parsed->args[i], "add"))
		op.type = OP_ADD;
	else
		CMD_PARAMS_FAIL;

	uint8_t vecs = colmode ? matrix->cols : matrix->rows;

	int vec1;
	(void)sscanf(parsed->args[i + 1], "%d", &vec1);
	if (vec1 < MIN_MATRIX_SIZE || vec1 > vecs)
	{
		printf("\t%s indisi %d ile %d arasinda olmalidir.\n\n", colmode ? "Sutun" : "Satir", MIN_MATRIX_SIZE, vecs);
		return;
	}
	op.vec1 = vec1 - 1;

	if (op.type == OP_MULTIPLY)
		(void)sscanf(parsed->args[i + 2], "%f", &op.coeff);
	else
	{
		int vec2;
		(void)sscanf(parsed->args[i + 2 + (op.type == OP_ADD)], "%d", &vec2);
		if (vec2 < MIN_MATRIX_SIZE || vec2 > vecs)
		{
			printf("\t%s indisi %d ile %d arasinda olmalidir.\n\n", colmode ? "Sutun" : "Satir", MIN_MATRIX_SIZE, vecs);
			return;
		}
		op.vec2 = vec2 - 1;
	}

	if (op.type == OP_ADD)
	{
		if (op.vec1 == op.vec2)
		{
			printf("\tIki %s indisi birbirinden farkli olmalidir.\n\n", colmode ? "sutun" : "satir");
			return;
		}
		(void)sscanf(parsed->args[i + 2], "%f", &op.coeff);
	}

	if (!op.coeff)
	{
		printf("\tKatsayi sifirdan farkli bir gercel sayi olmalidir.\n\n");
		return;
	}

	mx_apply_op(matrix, op);
	printf("\n\n");
}

void cmd_help(CMD_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_PARAMS_FAIL;

	if (!parsed->argcount)
	{
		for (int cmd = 0; cmd < CMD_COUNT; cmd++)
			printf("\t%s\n", memory->commands[cmd]->help);
		printf("\n");
		return;
	}

	for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		if (!strcmp(parsed->args[0], memory->commands[cmd]->name))
		{
			printf("\t%s\n\n", memory->commands[cmd]->help);
			return;
		}
	printf("\tKomut bulunamadi: %s\n\n", parsed->args[0]);
}

void cmd_clear(CMD_PARAMS)
{
	if (parsed->argcount)
		CMD_PARAMS_FAIL;

	clear();
}

void cmd_list(CMD_PARAMS)
{
	if (parsed->argcount)
		CMD_PARAMS_FAIL;

	uint8_t count = 0;
	Node* node = memory->tail;
	while (node)
	{
		printf("\t%2d. %c %dx%d\n", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("\t%d adet matris bulundu.\n\n", count);
}

void cmd_print(CMD_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_PARAMS_FAIL;

	Matrix* matrix;
	get_one_mx(memory, parsed, &matrix);
	if (!matrix)
		return;

	if (!parsed->argcount)
	{
		mx_print(matrix);
		return;
	}

	matrix = mx_copy(matrix);
	CMD_SET_MEMORY;
}

void cmd_define(CMD_PARAMS)
{
	if (!parsed->argcount || parsed->argcount > 5)
		CMD_PARAMS_FAIL;

	char name = *parsed->args[0];

	if (*(parsed->args[0] + 1))
	{
		printf("\tTanimlanacak olan matris degiskeninin adi tek harften olusmalidir.\n\n");
		return;
	}

	if (name < 'A' || name > 'Z')
	{
		printf("\tMatris adi buyuk bir harf olmalidir. (Yalnizca Ingiliz alfabesi harfleri)\n\n");
		return;
	}

	if (mem_query(memory, name))
	{
		printf("\t%c adinda bir matris zaten var.\n\n", name);
		return;
	}

	Matrix* matrix;

	if (parsed->argcount == 1)
	{
		if (!memory->matrix)
			CMD_MATRIX_FAIL;

		matrix = mx_copy(memory->matrix);
		if (!matrix)
			CMD_MEMORY_FAIL;
	}

	else if (parsed->argcount == 5)
	{
		if (strcmp(parsed->args[3], "all"))
			CMD_PARAMS_FAIL;

		int rows, cols;
		float value;

		(void)sscanf(parsed->args[1], "%d", &rows);
		(void)sscanf(parsed->args[2], "%d", &cols);
		(void)sscanf(parsed->args[4], "%f", &value);

		if (rows < MIN_MATRIX_SIZE || rows > MAX_MATRIX_SIZE)
		{
			printf("\tMatris satir sayisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			return;
		}

		if (cols < MIN_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
		{
			printf("\tMatris sutun sayisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			return;
		}

		matrix = mx_create_all(rows, cols, value);
	}

	else
	{
		int size;
		float value;

		(void)sscanf(parsed->args[1], "%d", &size);
		(void)sscanf(parsed->args[3], "%f", &value);

		if (size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE)
		{
			printf("\tMatris satir ve sutun sayisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			return;
		}

		if (!strcmp(parsed->args[2], "diag"))
			matrix = mx_create_diag(size, value);
		else if (!strcmp(parsed->args[2], "low"))
			matrix = mx_create_low(size, value);
		else if (!strcmp(parsed->args[2], "up"))
			matrix = mx_create_up(size, value);
		else
			CMD_PARAMS_FAIL;
	}

	if (!mem_add(memory, name, matrix))
	{
		free(matrix->data);
		free(matrix);
		CMD_MEMORY_FAIL;
	}

	if (parsed->argcount == 1)
	{
		mx_print(matrix);
		return;
	}

	matrix = mx_copy(matrix);
	CMD_SET_MEMORY;
}

void cmd_delete(CMD_PARAMS)
{
	if (parsed->argcount != 1)
		CMD_PARAMS_FAIL;

	if (!strcmp(parsed->args[0], "all"))
	{
		mem_remove_all(memory);
		printf("\tTanimlanmis tum matrisler silindi.\n\n");
		return;
	}

	Node* node = mem_query(memory, *parsed->args[0]);
	if (!node)
		CMD_SEARCH_FAIL(0);

	mem_remove(memory, node);
	printf("\t%c matrisi silindi.\n\n", *parsed->args[0]);
}

void cmd_get(CMD_PARAMS)
{
	if (parsed->argcount < 2 || parsed->argcount > 3)
		CMD_PARAMS_FAIL;

	Matrix* matrix;
	get_one_mx(memory, parsed, &matrix);
	if (!matrix)
		return;

	uint8_t i = parsed->argcount - 2;
	int row, col;
	(void)sscanf(parsed->args[i++], "%d", &row);
	(void)sscanf(parsed->args[i++], "%d", &col);

	if (row < MIN_MATRIX_SIZE || row > matrix->rows)
	{
		printf("\tSatir indisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, matrix->rows);
		return;
	}

	if (col < MIN_MATRIX_SIZE || col > matrix->cols)
	{
		printf("\tSutun indisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, matrix->cols);
		return;
	}

	printf("%g", *mx_get(matrix, row - 1, col - 1));
}

void cmd_set(CMD_PARAMS)
{
	if (parsed->argcount < 3 || parsed->argcount > 4)
		CMD_PARAMS_FAIL;

	Matrix* matrix;
	get_one_mx(memory, parsed, &matrix);
	if (!matrix)
		return;

	uint8_t i = parsed->argcount - 3;
	int row, col;
	float value;
	(void)sscanf(parsed->args[i++], "%d", &row);
	(void)sscanf(parsed->args[i++], "%d", &col);
	(void)sscanf(parsed->args[i++], "%f", &value);

	if (row < MIN_MATRIX_SIZE || row > matrix->rows)
	{
		printf("\tSatir indisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, matrix->rows);
		return;
	}

	if (col < MIN_MATRIX_SIZE || col > matrix->cols)
	{
		printf("\tSutun indisi %d ile %d arasinda olmalidir.\n\n", MIN_MATRIX_SIZE, matrix->cols);
		return;
	}

	mx_set(matrix, row - 1, col - 1, value);
	mx_print(matrix);
}

void cmd_isequal(CMD_PARAMS)
{
	if (!parsed->argcount || parsed->argcount > 2)
		CMD_PARAMS_FAIL;

	Matrix *matrix, *matrix2;
	get_two_mx(memory, parsed, &matrix, &matrix2);
	if (!matrix)
		return;

	if (matrix->rows != matrix2->rows || matrix->cols != matrix2->cols)
	{
		printf("\tBoyutlari birbirine esit olmadigi icin verilen matrisler birbirine esit degildir.\n\n");
		return;
	}

	if (memcmp(matrix->data, matrix2->data, matrix->rows * matrix->cols * sizeof(float)))
	{
		printf("\tVerilen matrisler birbirine esit degildir.\n\n");
		return;
	}

	printf("\tVerilen matrisler birbirine esit.\n\n");
}

void cmd_transpose(CMD_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_PARAMS_FAIL;

	Matrix* matrix;
	get_one_mx(memory, parsed, &matrix);
	if (!matrix)
		return;

	matrix = mx_transpose(matrix);
	CMD_SET_MEMORY;
}

void cmd_add(CMD_PARAMS)
{
	if (!parsed->argcount || parsed->argcount > 2)
		CMD_PARAMS_FAIL;

	Matrix *matrix, *matrix2;
	get_two_mx(memory, parsed, &matrix, &matrix2);
	if (!matrix)
		return;

	if (matrix->rows != matrix2->rows || matrix->cols != matrix2->cols)
	{
		printf("\tToplama yapabilmek icin matrislerin boyutlari birbirine esit olmalidir.\n\n");
		return;
	}

	matrix = mx_add(matrix, matrix2);
	CMD_SET_MEMORY;
}

void cmd_rowop(CMD_PARAMS)
{
	apply_op(memory, parsed, 0);
}

void cmd_colop(CMD_PARAMS)
{
	apply_op(memory, parsed, 1);
}