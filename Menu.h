#pragma once
typedef struct Menu
{
	char* title;
	char* options[10];
	int (*functions[10])();
} Menu;

// Home.c �zel fonksiyon prototipleri
int kill();
int matrisDefinition();