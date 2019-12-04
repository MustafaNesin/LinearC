#include <stdio.h>
#include <Windows.h>
#include "Utils.h"

int getSingleChar()
{
	int c = -1, _ = -1;
	while ((_ = getchar()) != -1 && _ != '\n')
		if (c == -1)
			c = _;
	return c;
}

int getOption(Menu menu)
{
	int opt, i, error = 0;
	do
	{
		clear();

		printf("%s", menu.title);

		for (i = 1; i < 10; i++)
		{
			printf("\n");
			if (!menu.options[i])
				continue;
			printf("%d. %s", i, menu.options[i]);
		}
		if (menu.options[0])
			printf("\n0. %s", menu.options[0]);

		printf("\n%s", error ? "Lutfen uygun bir secenek secin." : " ");
		printf("\n>>> ");

		opt = getSingleChar() - '0';
	} while (error = (opt < 0 || opt > 10 || !menu.options[opt]));

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