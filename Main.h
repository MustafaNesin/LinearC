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

// Yeni bir matris d���m� olu�turur ve haf�zan�n sonuna (tail) ekler.
Node* new_node(MxMemory*, char, Matrix*);

// Haf�zada matris ad�nda d���m arar. Bulunan d���m� d�nderir. 
// E�er bulunamazsa NULL (0) d�nd�r�r.
Node* search_node(MxMemory*, char);

// Verilen d���m� listeden siler, d���m� ve i�erdi�i matrisin haf�zas�n� serbest b�rak�r.
void free_node(MxMemory*, Node*);

// T�m d���mleri siler ve haf�zay� serbest b�rak�r.
void free_memory(MxMemory*);

// Konsol ekran�n� temizler
void clear();

void scanl(char*, ...);

int get_char();