#include <stdio.h>
#include <Windows.h>
#include "Utils.h"

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