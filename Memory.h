#pragma once
#define CON_BUFFER_SIZE 255
#define MIN_MATRIX_SIZE 1
#define MAX_MATRIX_SIZE 10

typedef struct
{
	int rows;
	int cols;
	float* data;
} Matrix;

typedef struct
{
	char* name;
	char* help;
	void (*function)(void* memory, char* args);
} Command;

typedef struct
{
	char* title;
	char* options[10];
	void (*functions[10])(void* memory);
} Menu;

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
	Menu* home;
	Command** commands;
} Memory;

Memory* init_mem();
void free_mem(Memory* memory);

Node* mem_new(Memory* memory, char name, Matrix* matrix);
Node* mem_search(Memory* memory, char name);
void mem_remove(Memory* memory, Node* node);