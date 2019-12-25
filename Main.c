#include <stdio.h>
#include "Memory.h"
#include "Menu.h"

int main()
{
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