#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

#pragma region Definitions
#define FACTOR_NUMBER              (uint8_t)1
#define FACTOR_NODE                (uint8_t)2
#define FACTOR_FUNCTION            (uint8_t)3
#define FACTOR_PARENTHESIS         (uint8_t)4

#define GET_SCALAR_ARG(X_ID)       (args[X_ID].value.scalar)
#define GET_MATRIX_ARG(X_ID)       (args[X_ID].value.matrix)
#define GET_RADIAN_ARG(X_ID)       (memory->radian ? GET_SCALAR_ARG(X_ID) : deg2rad(GET_SCALAR_ARG(X_ID)))

#define INIT_RESULT                EValue result = {0};
#define RETURN_RESULT              return result;

#define SET_MATRIX_RESULT(X_ID)    result.scalar = false; result.value.matrix = X_ID;
#define SET_SCALAR_RESULT(X_ID)    result.scalar = true; result.value.scalar = X_ID;
#define SET_UNIT_RESULT(X_ID)      result.scalar = true; result.value.scalar = memory->radian ? X_ID : rad2deg(X_ID);

#define RETURN_SCALAR_RESULT(X_ID) INIT_RESULT; SET_SCALAR_RESULT(X_ID); RETURN_RESULT;
#define RETURN_MATRIX_RESULT(X_ID) INIT_RESULT; SET_MATRIX_RESULT(X_ID); RETURN_RESULT;
#define RETURN_UNIT_RESULT(X_ID)   INIT_RESULT; SET_UNIT_RESULT(X_ID); RETURN_RESULT;
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

#pragma region Commands
EValue cmd_clear(CMD_PARAM_DECL);

#pragma region Matrix
EValue cmd_transpose(CMD_PARAM_DECL);
EValue cmd_inverse(CMD_PARAM_DECL);
EValue cmd_determinant(CMD_PARAM_DECL);
EValue cmd_adjoint(CMD_PARAM_DECL);
EValue cmd_rank(CMD_PARAM_DECL);
EValue cmd_power(CMD_PARAM_DECL);
#pragma endregion

#pragma region Math
EValue cmd_deg(CMD_PARAM_DECL);
EValue cmd_rad(CMD_PARAM_DECL);
EValue cmd_pi(CMD_PARAM_DECL);
EValue cmd_e(CMD_PARAM_DECL);
EValue cmd_sqrt(CMD_PARAM_DECL);
EValue cmd_ln(CMD_PARAM_DECL);
EValue cmd_log(CMD_PARAM_DECL);
EValue cmd_sin(CMD_PARAM_DECL);
EValue cmd_cos(CMD_PARAM_DECL);
EValue cmd_tan(CMD_PARAM_DECL);
EValue cmd_cot(CMD_PARAM_DECL);
EValue cmd_sec(CMD_PARAM_DECL);
EValue cmd_csc(CMD_PARAM_DECL);
EValue cmd_sinh(CMD_PARAM_DECL);
EValue cmd_cosh(CMD_PARAM_DECL);
EValue cmd_tanh(CMD_PARAM_DECL);
EValue cmd_coth(CMD_PARAM_DECL);
EValue cmd_sech(CMD_PARAM_DECL);
EValue cmd_csch(CMD_PARAM_DECL);
EValue cmd_asin(CMD_PARAM_DECL);
EValue cmd_acos(CMD_PARAM_DECL);
EValue cmd_atan(CMD_PARAM_DECL);
EValue cmd_acot(CMD_PARAM_DECL);
EValue cmd_asec(CMD_PARAM_DECL);
EValue cmd_acsc(CMD_PARAM_DECL);
EValue cmd_asinh(CMD_PARAM_DECL);
EValue cmd_acosh(CMD_PARAM_DECL);
EValue cmd_atanh(CMD_PARAM_DECL);
EValue cmd_acoth(CMD_PARAM_DECL);
EValue cmd_asech(CMD_PARAM_DECL);
EValue cmd_acsch(CMD_PARAM_DECL);
#pragma endregion
#pragma endregion