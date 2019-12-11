#include "Main.h"
#include <stdio.h>
#include <Windows.h>

void scanl(char* format, ...)
{
	char c;
	va_list args;
	va_start(args, format);
	(void)vscanf(format, args);
	va_end(args);

	while ((c = getchar()) != '\n' && c != EOF) {}
}

void free_memory(MxMemory* memory)
{
	if (!memory)
		return;

	while (memory->tail)
		free_node(memory, memory->tail);
}

void free_node(MxMemory* memory, Node* node)
{
	if (!memory || !node)
		return;

	if (node->prev)
		node->prev->next = node->next;

	if (node->next)
		node->next->prev = node->prev;
	else
		memory->tail = node->prev;

	free(node->mx->data);
	free(node->mx);
	free(node);
}

void loop_menu(Menu* menu, MxMemory* memory)
{
	int opt;
	do
	{
		clear();
		opt = show_menu(menu);
		clear();
	} while (menu->functions[opt](memory));
}

int show_menu(Menu* menu)
{
	char opt;
	int i, error = 0;
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

		scanl("%c", &opt); 
		opt -= '0';
	} while (error = (opt < 0 || opt > 9 || !menu->options[opt]));

	return opt;
}

void clear()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 };
	DWORD cCharsWritten;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD dwConSize;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

	if (!FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
		return;

	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return;

	if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten))
		return;

	SetConsoleCursorPosition(hConsole, coordScreen);
}