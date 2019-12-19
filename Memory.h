#pragma once

#pragma region Include
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Utils.h"

#ifdef _WIN32
	#include <Windows.h>
#endif
#pragma endregion

#pragma region Definitions
#define CMD_COUNT       (uint8_t)2    // Her yeni komut eklendiðinde artýrýlmalýdýr
#define CMD_PARAM_COUNT (uint8_t)5
#define CON_BUFFER_SIZE (uint8_t)200 
#define MIN_MATRIX_SIZE (uint8_t)1
#define MAX_MATRIX_SIZE (uint8_t)10 
#define BEGIN_CMD_INIT	c++
#pragma endregion

#pragma region Type Definitions
typedef struct Function    Function;
typedef struct Parameter   Parameter;
typedef struct PExpression PExpression;
typedef struct PTerm       PTerm;
typedef struct PFactor     PFactor;
typedef struct PFunction   PFunction;

typedef struct Matrix      Matrix;
typedef struct Operation   Operation;

typedef struct Memory      Memory;
typedef struct Node        Node;

typedef struct Menu        Menu;

#define MENU_PARAM_DECL Memory* memory
#define CMD_PARAM_DECL  Memory* memory
#pragma endregion

#pragma region Structures
struct Function
{
	char     *name;
	char     *help;
	void     (*function)(CMD_PARAM_DECL, ...);
	uint8_t  ret;
	uint8_t  paramcount;
	uint8_t  params[CMD_PARAM_COUNT];
};

struct Menu
{
	char     *title;
	char     *options[10];
	void     (*functions[10])(MENU_PARAM_DECL);
};

struct Node
{
	Node     *prev;
	Node     *next;
	Matrix   *matrix;
	char     name;
};

struct Memory
{
	Node     *tail;
	Menu     home;
	Function commands[CMD_COUNT];
};
#pragma endregion

Memory*   init_mem       (void);
void      free_mem       (Memory* memory);

Node*     mem_new        (Memory* memory, char name, uint8_t rows, uint8_t cols, float* data);
Node*     mem_add        (Memory* memory, char name, Matrix* matrix);
Node*     mem_query      (Memory* memory, char name);
void      mem_remove     (Memory* memory, Node* node);
void      mem_remove_all (Memory* memory);
int       mem_save       (Memory* memory);
int       mem_read       (Memory* memory);