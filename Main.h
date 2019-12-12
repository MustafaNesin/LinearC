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



/* ~~~ MENU FONKSÝYONLARI ~~~ */

// Üst menüye geriye dön.
int mf_back(Memory*);

// Menüden matris tanýmla
int mf_define(Memory*);

// Tanýmlanmýþ matrisleri listele
int mf_list(Memory*);

// Ýþlemler konsolu
int mf_console(Memory*);



/* ~~~ ARAÇLAR ~~~ */

// Yeni bir matris düðümü oluþturur ve hafýzanýn sonuna (tail) ekler.
Node* new_node(Memory*, char, Matrix*);

// Hafýzada matris adýnda düðüm arar. Bulunan düðümü dönderir. 
// Eðer bulunamazsa NULL (0) döndürür.
Node* search_node(Memory*, char);

// Verilen düðümü listeden siler, düðümü ve içerdiði matrisin hafýzasýný serbest býrakýr.
void free_node(Memory*, Node*);

// Tüm düðümleri siler ve hafýzayý serbest býrakýr.
void free_memory(Memory*);

// Menüden çýkýlana kadar fonksiyonlarýný çalýþtýrmaya ve menüyü listelemeye devam et
void loop_menu(Menu*, Memory*);

// Seçenek menüsü gösterir ve seçilen geçerli bir seçeneðin rakamýný döndürür
int show_menu(Menu*);

// Konsol ekranýný temizler
void clear();

// Formata göre konsolu oku, satýrýn kalanýný imha et
void scanl(char*, ...);

// Tek karakter al, satýrýn kalanýný imha et
int get_char();

// Hata mesajý göster.
int print_error(char*, ...);