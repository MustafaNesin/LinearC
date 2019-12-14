#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "Command.h"
#include "Menu.h"
#include "Utils.h"

void loop_menu(Menu* menu, Memory* memory)
{
	if (!menu || !memory)
		return;

	void (*func)(void*);

	while (func = menu->functions[show_menu(menu)])
	{
		clear();
		func(memory);
	}
}

int show_menu(Menu* menu)
{
	if (!menu)
		return 0;

	int opt, i, error = 0;

	do
	{
		clear();

		printf("%s", menu->title);

		for (i = 1; i < 10; i++)
		{
			printf("\n");
			if (!menu->options[i])
				continue;
			printf("%d. %s", i, menu->options[i]);
		}
		if (menu->options[0])
			printf("\n0. %s", menu->options[0]);

		printf("\n%s", error ? "Lutfen uygun bir secenek secin." : " ");
		printf("\n>>> ");

		scanl("%d", &opt);
	} while (error = (opt < 0 || opt > 9 || !menu->options[opt]));

	return opt;
}

void menu_define(Memory* memory)
{
	if (!memory)
		return;

	int rows, cols;
	char c, name;
	Node* node;

	// Matris adý al, ayný adda varsa üstüne yazýlýp yazýlmayacaðýný sor
	{
		printf("Matris Adi (Buyuk harf): ");
		scanl("%c", &name);
		if (name < 'A' || name > 'Z')
		{
			printf("\nMatris adi buyuk bir harf olmalidir. (Yalnizca Ingiliz alfabesi harfleri)");
			get_char();
			return;
		}
		if (node = mem_search(memory, name))
		{
			printf("\nAyni adda bir matris zaten var. Uzerine yazilsin mi? (E/H): ");
			scanl("%c", &c);
			if ((c | 0x20) != 'e')
			{
				printf("\nMatris tanimlanamadi.");
				get_char();
				return;
			}
			mem_remove(memory, node);
		}
	}

	// Satýr ve sütun sayýsý al
	{
		printf("Satir Sayisi: ");
		scanl("%d", &rows);
		if (rows < MIN_MATRIX_SIZE || rows > MAX_MATRIX_SIZE)
		{
			printf("\nMatris satir sayisi %d ile %d arasinda olmalidir.", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			get_char();
			return;
		}

		printf("Sutun Sayisi: ");
		scanl("%d", &cols);
		if (cols < MIN_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
		{
			printf("\nMatris sutun sayisi %d ile %d arasinda olmalidir.", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			get_char();
			return;
		}
	}

	// Matris oluþtur
	Matrix* matrix = malloc(sizeof(Matrix));
	{
		if (!matrix)
		{
			printf("\nYeni matris icin bellek yeri ayirilirken hata olustu.");
			get_char();
			return;
		}
		matrix->data = calloc(rows * cols, sizeof(float));
		if (!matrix->data)
		{
			free(matrix);
			printf("\nYeni matris verisi icin bellek yeri ayirilirken hata olustu.");
			get_char();
			return;
		}
		matrix->rows = rows;
		matrix->cols = cols;
		node = mem_new(memory, name, matrix);
	}

	// Matris içeriðini al
	float* d = matrix->data;
	for (int i = 0, j; i < rows; i++)
		for (j = 0; j < cols; j++)
		{
			printf("%c[%d, %d] = ", name, i + 1, j + 1);
			scanl("%f", d++);
		}
}

void menu_list(Memory* memory)
{
	if (!memory)
		return;

	int count = 0;

	printf("\tAd\tSat\tSut");

	Node* node = memory->tail;
	while (node)
	{
		printf("\n%d.\t%c\t%d\t%d", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("\n\n%d adet matris bulundu.", count);
	get_char();
}

void menu_console(Memory* memory)
{
	if (!memory)
		return;

	Command* command = 0;
	ParsedCommand parsed = { 0 };
	int input, length = 0, cmd;
	char buffer[CON_BUFFER_SIZE];

	printf("Konsol hakkinda yardim almak icin help, geri donmek icin return yazin.\n> ");

	next:
	input = getchar();

	if (input == -1)
		goto end;

	if (input == '\n')
	{
		buffer[length] = '\0';
		parse_command(buffer, &parsed);

		if (!parsed.name || !*parsed.name)
		{
			printf("> ");
			length = 0;
			goto next;
		}

		for (cmd = 0; cmd < CMD_COUNT; cmd++)
			if (!strcmp(parsed.name, memory->commands[cmd]->name))
				command = memory->commands[cmd];

		if (!command)
			printf("Suna iliskin bir komut bulunamadi: %s", parsed.name);
		else
		{
			if (!command->function)
				goto end;
			command->function(memory, &parsed);
		}

		if (strcmp(parsed.name, memory->commands[CMD_CLEAR]->name))
			printf("\n");

		printf("> ");
		length = 0;
		command = NULL;
		goto next;
	}

	buffer[length++] = (char)input;
	if (length == CON_BUFFER_SIZE)
	{
		printf("\nKonsola yazilabilecek maksimum karakter sayisi (%d) asildi.", length);
		get_char();
		length = 0;
		goto next;
	}
	goto next;

	end:
	if (memory->matrix)
	{
		free(memory->matrix);
		memory->matrix = NULL;
	}
}

void menu_equation(Memory* memory)
{
	if (!memory)
		return;

	// ...
}

void menu_save(Memory* memory)
{
	if (!memory)
		return;

	// ...
}

void menu_load(Memory* memory)
{
	if (!memory)
		return;

	// ...
}