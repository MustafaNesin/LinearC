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



/* MENU FONKSÝYONLARI */

int mf_back(MxMemory*);

// Home.c

int mf_define(MxMemory*);



/* ARAÇLAR */

// Menüden çýkýlana kadar fonksiyonlarýný çalýþtýrmaya ve menüyü listelemeye devam et
void loop_menu(Menu*, MxMemory*);

// Kullanýcýnýn ekrana girdiði karakterlerden ilkini döndürür, kalanýný da imha eder
// Dönüþ deðeri -1 ise karakter girilmemiþ demektir, yoksa karakterin kendisini döndürür
int get_char();

// Seçenek menüsü gösterir ve seçilen geçerli bir seçeneðin rakamýný döndürür
int show_menu(Menu*);

// Konsol ekranýný temizler
void clear();