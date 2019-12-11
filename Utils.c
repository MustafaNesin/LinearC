#include "Main.h"
#include <stdio.h>
#include <Windows.h>

Node* new_node(MxMemory* memory, char name, Matrix* matrix)
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

Node* search_node(MxMemory* memory, char name)
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

	free(node->matrix->data);
	free(node->matrix);
	free(node);
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

void scanl(char* format, ...)
{
	char c;
	va_list args;
	va_start(args, format);
	(void)vscanf(format, args);
	va_end(args);

	(void)get_char();
}

int get_char()
{
	int c = -1, _ = -1;
	while ((_ = getchar()) != -1 && _ != '\n')
		if (c == -1)
			c = _;
	return c;
}