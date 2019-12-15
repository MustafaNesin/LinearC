#include <stdio.h>
#include <Windows.h>
#include "Memory.h"
#include "Menu.h"

#if _DEBUG
#define _CRTDBG_MAP_ALLOC  
#include <stdlib.h>  
#include <crtdbg.h>
#endif

int main()
{
	Memory* memory = init_mem();

	if (!memory)
		return EXIT_FAILURE;

	loop_menu(memory->home, memory);
	free_mem(memory);

	#if _DEBUG
	_CrtDumpMemoryLeaks();
	#endif
	return EXIT_SUCCESS;
}