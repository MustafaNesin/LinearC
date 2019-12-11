#include <stdio.h>
#include <Windows.h>
#include "Main.h"

int main()
{
	MxMemory memory = { 0 };

	free_memory(&memory);

	return 0;
}