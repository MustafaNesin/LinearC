#pragma once
#include "Memory.h"

void loop_menu(Menu* menu, Memory* memory);
int show_menu(Menu* menu);

void menu_define(Memory* memory);
void menu_list(Memory* memory);
void menu_console(Memory* memory);
void menu_equation(Memory* memory);