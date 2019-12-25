#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

#pragma region Definitions
#define EQU_READ_LINE              (len = readl(buffer, CON_BUFFER_SIZE))
#pragma endregion

void      loop_menu      (Menu* menu, Memory* memory);
int       show_menu      (Menu* menu);
void      print_equterm  (float value, char name, bool first, bool last);

void      menu_define    (MENU_PARAM_DECL);
void      menu_list      (MENU_PARAM_DECL);
void      menu_console   (MENU_PARAM_DECL);
void      menu_equation  (MENU_PARAM_DECL);
void      menu_save      (MENU_PARAM_DECL);
void      menu_read      (MENU_PARAM_DECL);