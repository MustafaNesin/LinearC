#pragma once
#include <stdint.h>
#define CON_BUFFER_SIZE	255
#define MIN_MATRIX_SIZE	1
#define MAX_MATRIX_SIZE	10 //UINT8_MAX (255) deðerinden fazla olamaz.
#define MAX_PARAM_COUNT	5
#define FILE_NAME "save.bin"

typedef struct Memory		Memory;
typedef struct Matrix		Matrix;
typedef struct Menu			Menu;
typedef struct Command		Command;
typedef struct Parsed		Parsed;
typedef struct Node			Node;
typedef struct Operation	Operation;

#define MENU_PARAMS	Memory* memory
#define CMD_PARAMS	Memory* memory, Parsed* parsed

struct Node
{
	Node* prev;
	Node* next;
	Matrix* matrix;
	char name;
};

struct Memory
{
	Node* tail;
	Matrix* matrix;
	Menu* home;
	Command** commands;
};

Memory* init_mem(void);
void free_mem(Memory* memory);

Node* mem_new(Memory* memory, char name, uint8_t rows, uint8_t cols, float* data);
Node* mem_add(Memory* memory, char name, Matrix* matrix);
Node* mem_query(Memory* memory, char name);
void mem_remove(Memory* memory, Node* node);
void mem_remove_all(Memory* memory);
int mem_save(Memory* memory);
int mem_read(Memory* memory);