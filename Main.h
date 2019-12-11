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



/* ~~~ MENU FONKSÝYONLARI ~~~ */

// Üst menüye geriye dön.
int mf_back(MxMemory*);

// ~~~ Home.c ~~~

// Menüden matris tanýmla
int mf_define(MxMemory*);



/* ~~~ ARAÇLAR ~~~ */

// Verilen düðümü listeden siler, düðümü ve içerdiði matrisin hafýzasýný serbest býrakýr.
void free_node(Node*);

// Tüm düðümleri siler ve hafýzayý serbest býrakýr.
void free_memory(MxMemory*);

// Menüden çýkýlana kadar fonksiyonlarýný çalýþtýrmaya ve menüyü listelemeye devam et
void loop_menu(Menu*, MxMemory*);

// Seçenek menüsü gösterir ve seçilen geçerli bir seçeneðin rakamýný döndürür
int show_menu(Menu*);

// Konsol ekranýný temizler
void clear();

void scanl(char* format, ...);