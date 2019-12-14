#include <stdio.h>
#include <Windows.h>
#include "Memory.h"
#include "Menu.h"

int main()
{
	Memory* memory = init_mem();

	if (!memory)
		return EXIT_FAILURE;

	loop_menu(memory->home, memory);
	free_mem(memory);

	return EXIT_SUCCESS;
}