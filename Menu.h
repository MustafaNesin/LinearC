#pragma once
#include "Memory.h"

#define HOME_TITLE	"LineerC | Mustafa Nesin & Cem Ufuk Yilmaz"
#define HOME_OPT0	"Cikis"
#define HOME_OPT1	"Yeni Matris Tanimla"
#define HOME_OPT2	"Matris Listesi"
#define HOME_OPT3	"Islemler"
#define HOME_OPT4	"Denklem Cozucu"
#define HOME_OPT5	"Kaydet"
#define HOME_OPT6	"Yukle"

void loop_menu(Menu* menu, Memory* memory);
int show_menu(Menu* menu);

void menu_define(MENU_PARAMS);
void menu_list(MENU_PARAMS);
void menu_console(MENU_PARAMS);
void menu_equation(MENU_PARAMS);
void menu_save(MENU_PARAMS);
void menu_read(MENU_PARAMS);
