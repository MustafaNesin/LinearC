#include <stdio.h>
#include <Windows.h>
#include "Main.h"

int main()
{
	extern Menu m_home;
	MxMemory memory = { 0 };

	loop_menu(&m_home, &memory);

	return 0;
}
