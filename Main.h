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



/* MENU FONKS�YONLARI */

int mf_back(MxMemory*);

// Home.c

int mf_define(MxMemory*);



/* ARA�LAR */

// Men�den ��k�lana kadar fonksiyonlar�n� �al��t�rmaya ve men�y� listelemeye devam et
void loop_menu(Menu*, MxMemory*);

// Kullan�c�n�n ekrana girdi�i karakterlerden ilkini d�nd�r�r, kalan�n� da imha eder
// D�n�� de�eri -1 ise karakter girilmemi� demektir, yoksa karakterin kendisini d�nd�r�r
int get_char();

// Se�enek men�s� g�sterir ve se�ilen ge�erli bir se�ene�in rakam�n� d�nd�r�r
int show_menu(Menu*);

// Konsol ekran�n� temizler
void clear();