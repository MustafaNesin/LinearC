#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

#pragma region Definitions
#define PARAM_INT       1
#define PARAM_FLOAT     2
#define PARAM_NODE      3
#define PARAM_FUNCTION  4
#pragma endregion

#pragma region Structures
struct PFunc
{
	uint8_t  argcount;
	char     *name;
	void     *args[CMD_PARAM_COUNT];
};

struct PFactor
{
	PFactor  *next;
	void     *value;
	uint8_t  type;
};

struct PTerm
{
	PTerm    *next;
	PFactor  *factors;
	bool     negative;
};

struct PExpression
{
	Node     *left;
	PTerm    *right;
	bool     assignment;
};
#pragma endregion

void cmd_clear(CMD_PARAM_DECL)
{

}