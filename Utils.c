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

int readl(char buf[], int n)
{
	int c, i = 0;
	bool b = false;

	n--;
	while ((c = getchar()) != EOF && c != '\n' && i < n)
	{
		if (c > 127 || !isprint(c))
			b = true;

		if (!b)
			buf[i++] = c;
	}

	if (c != '\n')
		get_char();

	buf[i] = '\0';
	return i;
}

void scanl(char const* const format, ...)
{
	va_list args;
	va_start(args, format);
	(void)vscanf(format, args);
	va_end(args);

	(void)get_char();
}

int sscan_ufloat(char* buffer, float* value)
{
	char* _buffer = buffer;
	int state = 1, b, magnitude = 0, fraction = 0, f = 0;

	if (!(b = *buffer))
		return 0;

	do switch (state)
	{
		case 1: // 0-9
			if (b < '0' || b > '9')
				return 0;

			magnitude = b - '0';
			state++;
			break;
		case 2: // 0-9 | .
			if ((b < '0' || b > '9') && b != '.')
				goto success;

			if (b == '.')
				state++;
			else
			{
				magnitude *= 10;
				magnitude += b - '0';
			}
			break;
		case 3: // fraction: 0-9
			if (b < '0' || b > '9')
				return 0;

			f++;
			fraction = b - '0';
			state++;
			break;
		case 4: // 0-9
			if (b < '0' || b > '9')
				goto success;

			f++;
			fraction *= 10;
			fraction += b - '0';
			break;
	} while (b = *++_buffer);

	if (state % 2)
		return 0;

	success:
	*value = magnitude + fraction / powf(10.0, f);
	return _buffer - buffer;
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
		if (iszero(integer))
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

bool iszero(float value)
{
	return value == 0.0f || value == -0.0f;
}

void setcolor(uint16_t color)
{
	#ifdef _WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	#endif
}