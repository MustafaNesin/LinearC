#include "Utils.h"

void clear(void)
{
#ifdef _WIN32
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
#elif __linux__
	printf("\e[1;1H\e[2J");
#else
	printf("\n\n");
#endif
}

void scanl(char const* const format, ...)
{
	va_list args;
	va_start(args, format);
	(void)vscanf(format, args);
	va_end(args);

	(void)get_char();
}

int get_char(void)
{
	int c = -1, _ = -1;
	while ((_ = getchar()) != -1 && _ != '\n')
		if (c == -1)
			c = _;
	return c;
}

float froundf(float value)
{
	float integer = roundf(value);
	if (fabsf(value - integer) < 0.001)
	{
		if (integer == -0.0f)
			return 0;
		return integer;
	}
	return value;
}

float deg2rad(float degree)
{
	return degree * MATH_PI / 180.0f;
}

float rad2deg(float radian)
{
	return radian / MATH_PI * 180.0f;
}