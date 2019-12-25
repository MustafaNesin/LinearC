#include <stdio.h>
#include "Memory.h"
#include "Menu.h"

int main()
{
	#ifdef _WIN32
	SetConsoleTitle("LinearC by Mustafa Nesin & Cem Ufuk Yilmaz");
	#endif

	Memory* memory = init_mem();

	if (!memory)
		return EXIT_FAILURE;

	loop_menu(&memory->menus[HOME_MENU], memory);
	free_mem(memory);

#if _DEBUG
	_CrtDumpMemoryLeaks();
#endif

	return EXIT_SUCCESS;
}