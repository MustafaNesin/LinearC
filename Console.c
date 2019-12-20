#include "Console.h"
#include "Matrix.h"
#include "Utils.h"

#ifndef CMD_SYNTAX
#define CMD_SYNTAX

#define ASSERT__FAIL(cond) if (!(cond)) goto fail;
#define IS_SPACE isspace(*in)
#define EAT_SPACE if (IS_SPACE) { in++; continue; }
#define _EAT_SPACE while (*in) { EAT_SPACE; break; }

#define IS_NUMBER isdigit(*in)
#define IS_MATRIX isupper(*in)
#define IS_NAME islower(*in)
#define IS_FUNCTION islower(*in)
#define IS_ASSIGNMENT (result->assignment = *in++ == '=')
#define IS_PARENTHESIS (*in == '(')
#define IS_PARAMETER (*in == ',')
#define IS_FACTOR (IS_NUMBER || IS_MATRIX || IS_NAME || IS_FUNCTION || IS_PARENTHESIS)
#define IS_TSIGN (*in == '+' || *in == '-')
#define IS_NEGATIVE (*in == '-')
#define IS_ONLY_FUNCTION !input->assignment && input->right && \
	!input->right->next && input->right->factors && \
	!input->right->factors->next && input->right->factors->type == FACTOR_FUNCTION && \
	input->right->factors->value

#define ADD_TERM \
	if (tail) \
		tail = tail->next = calloc(1, sizeof(PTerm)); \
	else \
		tail = result = calloc(1, sizeof(PTerm)); \
	ASSERT__FAIL(tail); \
	tail->negative = negative;

#define IS_FSIGN (*in == '*' || *in == '/')
#define IS_DIVISOR (*in == '/')
#define ADD_FACTOR \
	if (tail) \
		tail = tail->next = calloc(1, sizeof(PFactor)); \
	else \
		tail = result = calloc(1, sizeof(PFactor)); \
	ASSERT__FAIL(tail); \
	tail->divisor = divisor;

#define CURRENT_PARAMTYPE result->argtypes[result->argcount]
#define CURRENT_PARAMETER result->args[result->argcount]

/* Function Parsing States */
#define FPS_NAME (uint8_t)1
#define FPS_SPACE (uint8_t)2
#define FPS_EMPTY_PARAM (uint8_t)3
#define FPS_PARAMETER (uint8_t)4
#endif

#ifndef CMD_SEMANTICS
#define CMD_SEMANTICS

#define ASSERT__ERROR(cond, error) if (!(cond)) return error;
#endif

PExpression* parse_formula(Memory* memory, char* in, char** out)
{
	PExpression* result;
	*out = in;

	ASSERT__FAIL(result = calloc(1, sizeof(PExpression)));

	_EAT_SPACE;

	if (IS_MATRIX)
	{
		result->leftname = *in;
		result->left = mem_query(memory, *in++);

		_EAT_SPACE;

		if (!IS_ASSIGNMENT)
		{
			result->left = NULL;
			result->leftname = '\0';
			in = *out;
		}
	}

	ASSERT__FAIL(result->right = parse_expression(memory, in, &in));

	*out = in;
	return result;

	fail:
	free_formula(result);
	return NULL;
}

PTerm* parse_expression(Memory* memory, char* in, char** out)
{
	PTerm* result = NULL;
	PTerm* tail = NULL;
	bool sign = false, negative = false;
	*out = in;

	while (*in)
	{
		EAT_SPACE;

		if (IS_TSIGN)
		{
			ASSERT__FAIL(!sign);
			ADD_TERM;
			negative = IS_NEGATIVE;
			in++;
			sign = true;
		}

		else if (IS_FACTOR)
		{
			if (!tail)
				ADD_TERM;

			ASSERT__FAIL(tail->factors = parse_term(memory, in, &in));
			tail->negative = negative;
			sign = false;
		}

		else break;
	}

	if (tail && !sign)
	{
		*out = in;
		return result;
	}

	fail:
	free_expression(result);
	return NULL;
}

PFactor* parse_term(Memory* memory, char* in, char** out)
{
	PFactor* result = NULL;
	PFactor* tail = NULL;
	bool sign = false, divisor = false;
	int length;
	*out = in;
	
	while (*in)
	{
		EAT_SPACE;

		if (IS_FSIGN)
		{
			ASSERT__FAIL(tail && !sign);
			divisor = IS_DIVISOR;
			in++;
			sign = true;
			continue;
		}
		
		if (IS_NUMBER)
		{
			ADD_FACTOR;
			tail->type = FACTOR_NUMBER;

			ASSERT__FAIL(tail->value = malloc(sizeof(float)));
			ASSERT__FAIL(sscanf(in, "%f%n", (float*)tail->value, &length));

			in += length;
		}

		else if (IS_MATRIX)
		{
			ADD_FACTOR;
			tail->type = FACTOR_NODE;
			tail->value = mem_query(memory, *in);
			in++;
		}

		else if (IS_FUNCTION)
		{
			ADD_FACTOR;
			tail->type = FACTOR_FUNCTION;

			ASSERT__FAIL(tail->value = parse_function(memory, in, &in));
		}

		else if (IS_PARENTHESIS)
		{
			ADD_FACTOR;
			tail->type = FACTOR_PARENTHESIS;

			in++; // ( karakteri atlandý
			ASSERT__FAIL(tail->value = parse_expression(memory, in, &in));
			ASSERT__FAIL(*in++ == ')');
		}

		else break;

		sign = false;
	}

	if (tail && !sign)
	{
		*out = in;
		return result;
	}

	fail:
	free_term(result);
	return NULL;
}

PFunction* parse_function(Memory* memory, char* in, char** out)
{
	PFunction* result;
	uint8_t state = FPS_NAME;
	int namelen = 0;
	*out = in;

	ASSERT__FAIL(result = calloc(1, sizeof(PFunction)));

	while (*in)
		switch (state)
		{
			case FPS_NAME:
			{
				if (IS_FUNCTION)
				{
					in++;
					namelen++;
				}
				else
				{
					ASSERT__FAIL(namelen);
					state = FPS_SPACE;
				}
				break;
			}

			case FPS_SPACE:
			{
				if (IS_PARENTHESIS)
					state = FPS_EMPTY_PARAM;
				else if (!IS_SPACE) 
					goto end;

				in++;
				break;
			}

			case FPS_EMPTY_PARAM:
			{
				if (!IS_SPACE)
				{
					if (*in == ')')
					{
						in++;
						state = 0;
						goto end;
					}

					state = FPS_PARAMETER;
					break;
				}

				in++;
				break;
			}

			case FPS_PARAMETER:
			{
				ASSERT__FAIL(CURRENT_PARAMETER = parse_expression(memory, in, &in));
				result->argcount++;

				if (IS_PARAMETER)
				{
					in++;
					break;
				}
				else if (*in == ')')
				{
					in++;
					state = 0;
					goto end;
				}
				else
					goto end;
			}
		}

	end:
	if (state != FPS_EMPTY_PARAM && state != FPS_PARAMETER && namelen)
	{
		result->name = malloc(namelen + 1);
		ASSERT__FAIL(result->name);
		memcpy(result->name, *out, namelen);
		result->name[namelen] = '\0';

		*out = in;
		return result;
	}

	fail:
	free_function(result);
	return NULL;
}

bool run_command(Memory* memory, PExpression* input, bool* newline)
{
	*newline = true;
	char* error = NULL;

	if (IS_ONLY_FUNCTION)
	{
		Function* func;
		PFunction* pfunc = input->right->factors->value;

		for (int i = 0; i < CMD_COUNT; i++)
		{
			func = &memory->commands[i];

			if (func->paramcount != pfunc->argcount)
				continue;

			if (strcmp(func->name, pfunc->name))
				continue;

			if (func->ret)
				continue;

			if (!memory->commands[i].function)
				return false;

			memory->commands[i].function(memory, func, pfunc);

			if (!strcmp(func->name, "clear"))
				*newline = false;

			return true;
		}
	}

	if (error = check_formula(memory, input))
		printf("%s", error);
	else
	{
		EValue result = evaluate_formula(memory, input, &error);

		if (error)
			printf("%s", error);
		else if (result.scalar)
			printf("= %g", result.value.scalar);
		else
		{
			mx_print(result.value.matrix);
			mx_free(result.value.matrix);
		}
	}

	return true;
}

char* check_formula(Memory* memory, PExpression* input)
{
	ASSERT__ERROR(input, "Sozdizimi hatasi.");

	return check_expression(memory, input->right);
}

char* check_expression(Memory* memory, PTerm* input)
{
	ASSERT__ERROR(input, "Sozdizimi hatasi.");

	PTerm* tail = input;
	char* error;

	do
	{ 
		ASSERT__ERROR(!(error = check_term(memory, tail->factors)), error); 
	}
	while (tail = tail->next);

	return NULL;
}

char* check_term(Memory* memory, PFactor* input)
{
	ASSERT__ERROR(input, "Sozdizimi hatasi.");

	PFactor* tail = input;
	char* error;

	ASSERT__ERROR(tail, "Kritik hata.");
	ASSERT__ERROR(!tail->divisor, "Kritik hata.");

	do 
	{
		ASSERT__ERROR(!(error = check_factor(memory, tail->value, tail->type)), error);
	} while (tail = tail->next);

	return NULL;
}

char* check_factor(Memory* memory, void* input, uint8_t type)
{
	switch (type)
	{
		case FACTOR_NUMBER:
			return NULL;
		case FACTOR_NODE:
			ASSERT__ERROR(input, "Hata: Tanimlanmamis matris kullanimi.");
			return NULL;
		case FACTOR_FUNCTION:
			ASSERT__ERROR(input, "Sozdizimi hatasi.");
			Function* func;
			PFunction* pfunc = input;

			for (int i = 0; i < CMD_COUNT; i++)
			{
				func = &memory->commands[i];
				if (func->paramcount != pfunc->argcount)
					continue;
				if (strcmp(func->name, pfunc->name))
					continue;
				ASSERT__ERROR(func->ret, 
					"Hata: Deger dondurmeyen bir fonksiyon ifade icinde kullanilamaz.");
				return NULL;
			}

			return "Hata: Bilinmeyen fonksiyon/komut kullanimi.";
		case FACTOR_PARENTHESIS:
			return check_expression(memory, input);
		default:
			return "Hata: Bilinmeyen carpan tipi.";
	}
}

EValue evaluate_formula(Memory* memory, PExpression* input, char** error)
{
	*error = NULL;
	EValue result = evaluate_expression(memory, input->right, error);

	if (*error)
		return result;

	if (input->assignment)
	{
		if (result.scalar)
		{
			*error = "Matris skaler bir sayiya esitlenemez.";
			return result;
		}

		if (input->left)
			mem_remove(memory, input->left);

		Matrix* copy = mx_copy(result.value.matrix);
		if (!copy)
		{
			*error = "Kritik hafiza hatasi.";
			mx_free(result.value.matrix);
			return result;
		}
		if (!(input->left = mem_add(memory, input->leftname, copy)))
		{
			*error = "Kritik hafiza hatasi.";
			mx_free(copy);
			mx_free(result.value.matrix);
			return result;
		}
	}

	return result;
}

EValue evaluate_expression(Memory* memory, PTerm* input, char** error)
{
	PTerm* tail = input;
	EValue result = evaluate_term(memory, tail->factors, error), term;

	if (*error)
		return result;

	if (tail->negative)
	{
		if (result.scalar)
			result.value.scalar *= -1;
		else
			mx_multiply(result.value.matrix, -1);
	}

	while (tail = tail->next)
	{
		term = evaluate_term(memory, tail->factors, error);

		if (*error)
		{
			if (!result.scalar)
				mx_free(result.value.matrix);
			return result;
		}

		if (term.scalar)
		{
			if (!result.scalar)
			{
				*error = "Skaler sayilar matrislerle toplanamaz.";
				mx_free(result.value.matrix);
				return result;
			}

			if (tail->negative)
				term.value.scalar *= -1;

			result.value.scalar += term.value.scalar;
		}
		else
		{
			if (result.scalar)
			{
				*error = "Skaler sayilar matrislerle toplanamaz.";
				mx_free(term.value.matrix);
				return result;
			}

			if (result.value.matrix->rows != term.value.matrix->rows ||
				result.value.matrix->cols != term.value.matrix->cols)
			{
				*error = "Toplama/cikarma yapabilmek icin matrislerin boyutlari birbirine esit olmalidir.";
				mx_free(term.value.matrix);
				mx_free(result.value.matrix);
				return result;
			}

			if (tail->negative)
				mx_multiply(term.value.matrix, -1);

			mx_add(result.value.matrix, term.value.matrix);
			mx_free(term.value.matrix);
		}
	}

	return result;
}

EValue evaluate_term(Memory* memory, PFactor* input, char** error)
{
	PFactor* tail = input;
	EValue result = evaluate_factor(memory, tail->value, tail->type, error), factor;

	if (*error)
		return result;

	while (tail = tail->next)
	{
		factor = evaluate_factor(memory, tail->value, tail->type, error);

		if (*error)
		{
			if (!result.scalar)
				mx_free(result.value.matrix);
			return result;
		}

		if (factor.scalar)
		{
			if (tail->divisor)
				factor.value.scalar = 1 / factor.value.scalar;

			if (result.scalar)
				result.value.scalar *= factor.value.scalar;
			else
				mx_multiply(result.value.matrix, factor.value.scalar);
		}
		else
		{
			if (tail->divisor)
			{
				*error = "Bolen bir matris olamaz.";
				mx_free(factor.value.matrix);
				if (!result.scalar)
					mx_free(result.value.matrix);
				return result;
			}

			if (result.scalar)
			{
				float s = result.value.scalar;
				result.scalar = false;
				result.value.matrix = factor.value.matrix;
				mx_multiply(result.value.matrix, s);
			}

			else if (result.value.matrix->cols != factor.value.matrix->rows)
			{
				*error = "Matris boyutlari carpmaya uygun degil.";
				mx_free(factor.value.matrix);
				mx_free(result.value.matrix);
				return result;
			}

			else
			{
				mx_dot(result.value.matrix, factor.value.matrix);
				mx_free(factor.value.matrix);
			}
		}
	}

	return result;
}

EValue evaluate_factor(Memory* memory, void* input, uint8_t type, char** error)
{
	EValue result = { 0 };

	switch (type)
	{
		case FACTOR_NUMBER:
			result.scalar = true;
			result.value.scalar = *(float*)input;
			break;
		case FACTOR_NODE:
			result.scalar = false;
			result.value.matrix = mx_copy(((Node*)input)->matrix);
			break;
		case FACTOR_FUNCTION:
		{
			Function* func;
			PFunction* pfunc = input;

			for (int i = 0; i < CMD_COUNT; i++)
			{
				func = &memory->commands[i];
				if (func->paramcount != pfunc->argcount)
					continue;
				if (strcmp(func->name, pfunc->name))
					continue;

				EValue args[CMD_PARAM_COUNT] = { 0 };
				for (int p = 0; p < func->paramcount; p++)
				{
					args[p] = evaluate_expression(memory, pfunc->args[p], error);
					if (*error)
						return result;
					if (func->params[p] != args[p].scalar)
					{
						*error = "Fonksiyon parametreleri uyusmuyor.";
						return result;
					}
				}
				return func->function(memory, args, error);
			}

			*error = "Kritik hata."; //check fonksiyonunda zaten kontrol edildi
			break;
		}
		case FACTOR_PARENTHESIS:
			result = evaluate_expression(memory, input, error);
			break;
		default:
			*error = "Kritik hata."; //check fonksiyonunda zaten kontrol edildi
			break;
	}

	return result;
}

void free_formula(PExpression* expression)
{
	if (!expression)
		return;

	free_expression(expression->right);
	free(expression);
}

void free_expression(PTerm* term)
{
	PTerm* temp;
	while (term)
	{
		temp = term->next;

		free_term(term->factors);
		free(term);

		term = temp;
	}
}

void free_term(PFactor* factor)
{
	PFactor* temp;
	while (factor)
	{
		temp = factor->next;

		switch (factor->type)
		{
			case FACTOR_NUMBER:
				free(factor->value);
				break;
			case FACTOR_FUNCTION:
				free_function(factor->value);
				break;
			case FACTOR_PARENTHESIS:
				free_expression(factor->value);
				break;
			default:
				break;
		}

		free(factor);

		factor = temp;
	}
}

void free_function(PFunction* function)
{
	if (!function)
		return;

	free(function->name);

	for (uint8_t i = 0; i < CMD_PARAM_COUNT; i++)
		free_expression(function->args[i]);

	free(function);
}

void cmd_clear(CMD_PARAM_DECL)
{
	clear();
}