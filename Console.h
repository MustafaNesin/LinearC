#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

#pragma region Definitions
#define FACTOR_NUMBER      (uint8_t)1
#define FACTOR_NODE        (uint8_t)2
#define FACTOR_FUNCTION    (uint8_t)3
#define FACTOR_PARENTHESIS (uint8_t)4

#define GET_SCALAR_ARG(i)  args[i].value.scalar
#define GET_MATRIX_ARG(i)  args[i].value.matrix
#pragma endregion

#pragma region Structures
struct PFunction
{
	char     *name;
	PTerm    *args[CMD_PARAM_COUNT];
	uint8_t  argcount;
};

struct PFactor
{
	PFactor  *next;
	void     *value;
	uint8_t  type;
	bool     divisor;
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
	char     leftname;
	bool     assignment;
};

struct EValue
{
	union
	{
		float scalar;
		Matrix* matrix;
	} value;
	bool     scalar;
};
#pragma endregion


PExpression* parse_formula(Memory* memory, char* in, char** out);
PTerm* parse_expression(Memory* memory, char* in, char** out);
PFactor* parse_term(Memory* memory, char* in, char** out);
PFunction* parse_function(Memory* memory, char* in, char** out);

bool run_command(Memory* memory, PExpression* input, bool* newline);

char* check_formula(Memory* memory, PExpression* input);
char* check_expression(Memory* memory, PTerm* input);
char* check_term(Memory* memory, PFactor* input);
char* check_factor(Memory* memory, void* input, uint8_t type);

EValue evaluate_formula(Memory* memory, PExpression* input, char** error);
EValue evaluate_expression(Memory* memory, PTerm* input, char** error);
EValue evaluate_term(Memory* memory, PFactor* input, char** error);
EValue evaluate_factor(Memory* memory, void* input, uint8_t type, char** error);

void free_formula(PExpression* expresion);
void free_expression(PTerm* term);
void free_term(PFactor* factor);
void free_function(PFunction* function);

EValue cmd_clear(CMD_PARAM_DECL);
EValue cmd_transpose(CMD_PARAM_DECL);
EValue cmd_inverse(CMD_PARAM_DECL);
EValue cmd_determinant(CMD_PARAM_DECL);
EValue cmd_adjoint(CMD_PARAM_DECL);
EValue cmd_rank(CMD_PARAM_DECL);
EValue cmd_power(CMD_PARAM_DECL);