#pragma once
#include "Matrix.h"

typedef struct Node
{
	struct Node* prev;
	struct Node* next;
	Matrix* mx;
	char name;
} Node;

typedef struct
{
	Node* tail;
} MxMemory;

typedef struct
{
	char* title;
	char* options[10];
	int (*functions[10])(MxMemory*);
} Menu;



/* ~~~ MENU FONKS�YONLARI ~~~ */

// �st men�ye geriye d�n.
int mf_back(MxMemory*);

// ~~~ Home.c ~~~

// Men�den matris tan�mla
int mf_define(MxMemory*);



/* ~~~ ARA�LAR ~~~ */

// Verilen d���m� listeden siler, d���m� ve i�erdi�i matrisin haf�zas�n� serbest b�rak�r.
void free_node(Node*);

// T�m d���mleri siler ve haf�zay� serbest b�rak�r.
void free_memory(MxMemory*);

// Men�den ��k�lana kadar fonksiyonlar�n� �al��t�rmaya ve men�y� listelemeye devam et
void loop_menu(Menu*, MxMemory*);

// Se�enek men�s� g�sterir ve se�ilen ge�erli bir se�ene�in rakam�n� d�nd�r�r
int show_menu(Menu*);

// Konsol ekran�n� temizler
void clear();

void scanl(char* format, ...);