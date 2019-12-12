#include <stdio.h>
#include <Windows.h>
#include "Main.h"

int main()
{
	extern Menu m_home;
	Memory memory = { 0 };

	loop_menu(&m_home, &memory);
	free_memory(&memory);

	return 0;
}