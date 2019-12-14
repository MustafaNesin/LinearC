#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "Command.h"
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
				if (++parsed->argcount == MAX_ARG_COUNT)
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

void cmd_help(CMD_FUNC_PARAMS)
{
	if (parsed->argcount > 1)
		CMD_FUNC_PARAM_COUNT_FAIL

	if (!parsed->argcount)
	{
		for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		{
			if (cmd)
				printf("\n\n");
			printf("%s", memory->commands[cmd]->name);
			printf(": %s", memory->commands[cmd]->help);
		}

		printf("\n");
		return;
	}

	for (int cmd = 0; cmd < CMD_COUNT; cmd++)
		if (!strcmp(parsed->args[0], memory->commands[cmd]->name))
		{
			printf("%s\n", memory->commands[cmd]->help);
			return;
		}
	printf("Komut bulunamadi: %s\n", parsed->args[0]);
}

void cmd_clear(CMD_FUNC_PARAMS)
{
	if (parsed->argcount)
		CMD_FUNC_PARAM_COUNT_FAIL

	clear();
}

void cmd_list(CMD_FUNC_PARAMS)
{
	if (parsed->argcount)
		CMD_FUNC_PARAM_COUNT_FAIL

	int count = 0;
	Node* node = memory->tail;
	while (node)
	{
		printf("%2d. %c %dx%d\n", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("%d adet matris bulundu.\n", count);
}

void cmd_print(CMD_FUNC_PARAMS)
{
	if (parsed->argcount != 1)
		CMD_FUNC_PARAM_COUNT_FAIL

	Node* node = mem_search(memory, *parsed->args[0]);
	if (!node)
	{
		printf("%s adinda bir matris bulunamadi.\n", parsed->args[0]);
		return;
	}

	if (!mx_assign(memory->matrix, node->matrix))
		printf("Matris bellege alinamadi.");

	for (int i = 0, j, p = 0; i < node->matrix->rows; i++)
	{
		printf("| ");
		for (j = 0; j < node->matrix->cols; j++, p++)
			printf("%10g ", *(node->matrix->data + p));
		printf("|\n");
	}

	printf("Boyut: %d satir, %d sutun\n", node->matrix->rows, node->matrix->cols);
}

void cmd_assign(CMD_FUNC_PARAMS)
{
	if (parsed->argcount != 1)
		CMD_FUNC_PARAM_COUNT_FAIL

	if (!memory->matrix->data)
	{
		printf("Konsolda bellege alinmis bir matris yok.\n");
		return;
	}

	Node* node;
	char name = *parsed->args[0], c;

	if (*(parsed->args[0] + 1))
	{
		printf("Tanimlanacak olan matris degiskeninin adi tek harften olusmalidir.\n");
		return;
	}

	if (name < 'A' || name > 'Z')
	{
		printf("Matris adi buyuk bir harf olmalidir. (Yalnizca Ingiliz alfabesi harfleri)\n");
		return;
	}

	if (node = mem_search(memory, name))
	{
		printf("Ayni adda bir matris zaten var. Uzerine yazilsin mi? (E/H): ");
		scanl("%c", &c);
		if ((c | 0x20) != 'e')
		{
			printf("Matris tanimlanamadi.\n");
			return;
		}
		mem_remove(memory, node);
	}

	Matrix* copy = mx_copy(memory->matrix);
	if (!copy)
	{
		printf("Matris tanimlanamadi.\n");
		return;
	}

	if (!mem_add(memory, name, copy))
	{
		free(copy->data);
		free(copy);
		printf("Matris tanimlanamadi.\n");
		return;
	}

	printf("%c matrisi basariyla tanimlandi.\n", name);
}