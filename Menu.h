#pragma once
#include "Memory.h"

void loop_menu(Menu* menu, Memory* memory);
int show_menu(Menu* menu);

void menu_define(MENU_FUNC_PARAMS);
void menu_list(MENU_FUNC_PARAMS);
void menu_console(MENU_FUNC_PARAMS);
void menu_equation(MENU_FUNC_PARAMS);
void menu_save(MENU_FUNC_PARAMS);
void menu_load(MENU_FUNC_PARAMS);