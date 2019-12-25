#include <stdio.h>
#include <Windows.h>
#include "Memory.h"
#include "Menu.h"

int main()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
	Memory* memory = init_mem();

	if (!memory)
		return EXIT_FAILURE;

	loop_menu(&memory->home, memory);
	free_mem(memory);

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return EXIT_SUCCESS;
}