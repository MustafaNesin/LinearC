#pragma once
#include "Matrix.h"

#define MIN_DEF_MX_SIZE 1
#define MAX_DEF_MX_SIZE 10

typedef struct Node
{
	struct Node* prev;
	struct Node* next;
	Matrix* matrix;
	char name;
} Node;

typedef struct
{
	Node* tail;
} Memory;

typedef struct
{
	char* title;
	char* options[10];
	int (*functions[10])(Memory*);
} Menu;



/* ~~~ MENU FONKS�YONLARI ~~~ */

// �st men�ye geriye d�n.
int mf_back(Memory*);

// Men�den matris tan�mla
int mf_define(Memory*);

// Tan�mlanm�� matrisleri listele
int mf_list(Memory*);

// ��lemler konsolu
int mf_console(Memory*);



/* ~~~ ARA�LAR ~~~ */

// Yeni bir matris d���m� olu�turur ve haf�zan�n sonuna (tail) ekler.
Node* new_node(Memory*, char, Matrix*);

// Haf�zada matris ad�nda d���m arar. Bulunan d���m� d�nderir. 
// E�er bulunamazsa NULL (0) d�nd�r�r.
Node* search_node(Memory*, char);

// Verilen d���m� listeden siler, d���m� ve i�erdi�i matrisin haf�zas�n� serbest b�rak�r.
void free_node(Memory*, Node*);

// T�m d���mleri siler ve haf�zay� serbest b�rak�r.
void free_memory(Memory*);

// Men�den ��k�lana kadar fonksiyonlar�n� �al��t�rmaya ve men�y� listelemeye devam et
void loop_menu(Menu*, Memory*);

// Se�enek men�s� g�sterir ve se�ilen ge�erli bir se�ene�in rakam�n� d�nd�r�r
int show_menu(Menu*);

// Konsol ekran�n� temizler
void clear();

// Formata g�re konsolu oku, sat�r�n kalan�n� imha et
void scanl(char*, ...);

// Tek karakter al, sat�r�n kalan�n� imha et
int get_char();

// Hata mesaj� g�ster.
int print_error(char*, ...);