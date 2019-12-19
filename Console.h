#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

#pragma region Definitions
#define FACTOR_NUMBER      (uint8_t)1
#define FACTOR_NODE        (uint8_t)2
#define FACTOR_FUNCTION    (uint8_t)3
#define FACTOR_PARENTHESIS (uint8_t)4
#pragma endregion

#pragma region Structures
struct PFunction
{
	uint8_t  argcount;
	uint8_t  namelen;
	char     *name;
	PTerm    *args[CMD_PARAM_COUNT];
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
	bool     assignment;
};
#pragma endregion


PExpression* parse_expression(Memory* memory, char* in, char** out);
PTerm* parse_terms(Memory* memory, char* in, char** out);
PFactor* parse_factors(Memory* memory, char* in, char** out);
PFunction* parse_function(Memory* memory, char* in, char** out);
void free_expression(PExpression* expresion);
void free_terms(PTerm* term);
void free_factors(PFactor* factor);
void free_function(PFunction* function);

void cmd_clear(CMD_PARAM_DECL);

char* strfactortype(uint8_t type);
void print_indentation(uint8_t indent);
void print_factor(PFactor* factor, uint8_t indent);
void print_term(PTerm* term, uint8_t indent);
void print_expression(PExpression* expression);