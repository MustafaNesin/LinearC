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
} MxMemory;

// Yeni bir matris düðümü oluþturur ve hafýzanýn sonuna (tail) ekler.
Node* new_node(MxMemory*, char, Matrix*);

// Hafýzada matris adýnda düðüm arar. Bulunan düðümü dönderir. 
// Eðer bulunamazsa NULL (0) döndürür.
Node* search_node(MxMemory*, char);

// Verilen düðümü listeden siler, düðümü ve içerdiði matrisin hafýzasýný serbest býrakýr.
void free_node(MxMemory*, Node*);

// Tüm düðümleri siler ve hafýzayý serbest býrakýr.
void free_memory(MxMemory*);

// Konsol ekranýný temizler
void clear();

void scanl(char*, ...);

int get_char();