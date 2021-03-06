#pragma once

#pragma region Include
#include <math.h>
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

#if _DEBUG
	#define _CRTDBG_MAP_ALLOC  
	#include <crtdbg.h>
#endif
#pragma endregion

#pragma region Definitions
#define MENU_COUNT       (uint8_t)3
#define HOME_MENU        (uint8_t)0
#define MATRIX_MENU      (uint8_t)1
#define FILE_MENU        (uint8_t)2
#define CMD_COUNT        (uint8_t)75    // Her yeni komut eklendiğinde artırılmalıdır
#define CMD_PARAM_COUNT  (uint8_t)5
#define CON_BUFFER_SIZE  (uint8_t)200 
#define MIN_MATRIX_SIZE  (uint8_t)1
#define MAX_MATRIX_SIZE  (uint8_t)10 
#define BEGIN_CMD_INIT   cmd++
#define CMD_MATRIX_PARAM (bool)false
#define CMD_NUMBER_PARAM (bool)true
#define MATH_PI          3.14159265358979323846f
#define MATH_E           2.71828182845904523536f

#define CMD_INIT(CMD_NAME, CMD_FUNCTION) \
	BEGIN_CMD_INIT; \
	memory->commands[cmd].name = CMD_NAME; \
	memory->commands[cmd].function = CMD_FUNCTION;

#define CMD_INIT2(CMD_NAME1, CMD_NAME2, CMD_FUNCTION) \
	CMD_INIT(CMD_NAME1, CMD_FUNCTION) \
	CMD_INIT(CMD_NAME2, CMD_FUNCTION)

#define CMD_INIT_0(CMD_NAME, CMD_FUNCTION) \
	CMD_INIT(CMD_NAME, CMD_FUNCTION) \
	memory->commands[cmd].returns = true;

#define CMD_INIT_1(CMD_NAME, CMD_FUNCTION, CMD_PARAM) \
	CMD_INIT_0(CMD_NAME, CMD_FUNCTION) \
	memory->commands[cmd].paramcount = 1; \
	memory->commands[cmd].params[0] = CMD_PARAM;

#define CMD_INIT_S(CMD_NAME, CMD_FUNCTION) \
	CMD_INIT_1(CMD_NAME, CMD_FUNCTION, CMD_NUMBER_PARAM)

#define CMD_INIT_S2(CMD_NAME1, CMD_NAME2, CMD_FUNCTION) \
	CMD_INIT_1(CMD_NAME1, CMD_FUNCTION, CMD_NUMBER_PARAM) \
	CMD_INIT_1(CMD_NAME2, CMD_FUNCTION, CMD_NUMBER_PARAM)

#define CMD_INIT_M(CMD_NAME, CMD_FUNCTION) \
	CMD_INIT_1(CMD_NAME, CMD_FUNCTION, CMD_MATRIX_PARAM)

#define CMD_INIT_M2(CMD_NAME1, CMD_NAME2, CMD_FUNCTION) \
	CMD_INIT_1(CMD_NAME1, CMD_FUNCTION, CMD_MATRIX_PARAM) \
	CMD_INIT_1(CMD_NAME2, CMD_FUNCTION, CMD_MATRIX_PARAM)

#pragma endregion

#pragma region Type Definitions
typedef struct Function    Function;
typedef struct PExpression PExpression;
typedef struct PTerm       PTerm;
typedef struct PFactor     PFactor;
typedef struct PFunction   PFunction;
typedef struct EValue      EValue;

typedef struct Matrix      Matrix;
typedef struct Operation   Operation;

typedef struct Memory      Memory;
typedef struct Node        Node;

typedef struct Menu        Menu;

#define MENU_PARAM_DECL Memory* memory
#define CMD_PARAM_DECL  Memory* memory, EValue args[CMD_PARAM_COUNT], char** error
#define CMD_PARAMS      memory, args, error
#pragma endregion

#pragma region Structures
struct Function
{
	char     *name;
	EValue   (*function)(CMD_PARAM_DECL);
	bool     returns;
	uint8_t  paramcount;
	bool     params[CMD_PARAM_COUNT]; //true: number, false: matrix
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
	Menu     menus[MENU_COUNT];
	Function commands[CMD_COUNT];
	bool     radian;
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