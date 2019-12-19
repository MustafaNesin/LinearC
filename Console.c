#include "Console.h"

#ifndef CMDPS
#define CMDPS
#define ASSERT(cond) if (!(cond)) goto fail;
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
#define ADD_TERM \
	if (tail) \
		tail = tail->next = calloc(1, sizeof(PTerm)); \
	else \
		tail = result = calloc(1, sizeof(PTerm)); \
	ASSERT(tail); \
	tail->negative = negative;

#define IS_FSIGN (*in == '*' || *in == '/')
#define IS_DIVISOR (*in == '/')
#define ADD_FACTOR \
	if (tail) \
		tail = tail->next = calloc(1, sizeof(PFactor)); \
	else \
		tail = result = calloc(1, sizeof(PFactor)); \
	ASSERT(tail); \
	tail->divisor = divisor;

#define CURRENT_PARAMTYPE result->argtypes[result->argcount]
#define CURRENT_PARAMETER result->args[result->argcount]

/* Function Parsing States */
#define FPS_NAME (uint8_t)1
#define FPS_SPACE (uint8_t)2
#define FPS_EMPTY_PARAM (uint8_t)3
#define FPS_PARAMETER (uint8_t)4
#endif

PExpression* parse_expression(Memory* memory, char* in, char** out)
{
	PExpression* result;
	*out = in;

	ASSERT(result = calloc(1, sizeof(PExpression)));

	_EAT_SPACE;

	if (IS_MATRIX)
	{
		result->left = mem_query(memory, *in++);

		_EAT_SPACE;

		if (!IS_ASSIGNMENT)
		{
			result->left = NULL;
			in = *out;
		}
	}
	//if factor ekle
	ASSERT(result->right = parse_terms(memory, in, &in));

	*out = in;
	return result;

	fail:
	free_expression(result);
	return NULL;
}

PTerm* parse_terms(Memory* memory, char* in, char** out)
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
			ASSERT(!sign);
			ADD_TERM;
			negative = IS_NEGATIVE;
			in++;
			sign = true;
		}

		else if (IS_FACTOR)
		{
			if (!tail)
				ADD_TERM;

			ASSERT(tail->factors = parse_factors(memory, in, &in));
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
	free_terms(result);
	return NULL;
}

PFactor* parse_factors(Memory* memory, char* in, char** out)
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
			ASSERT(tail && !sign);
			divisor = IS_DIVISOR;
			in++;
			sign = true;
			continue;
		}
		
		if (IS_NUMBER)
		{
			ADD_FACTOR;
			tail->type = FACTOR_NUMBER;

			ASSERT(tail->value = malloc(sizeof(float)));
			ASSERT(sscanf(in, "%f%n", (float*)tail->value, &length));

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

			ASSERT(tail->value = parse_function(memory, in, &in));
		}

		else if (IS_PARENTHESIS)
		{
			ADD_FACTOR;
			tail->type = FACTOR_PARENTHESIS;

			in++; // ( karakteri atlandý
			ASSERT(tail->value = parse_terms(memory, in, &in));
			ASSERT(*in++ == ')');
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
	free_factors(result);
	return NULL;
}

PFunction* parse_function(Memory* memory, char* in, char** out)
{
	PFunction* result;
	uint8_t state = FPS_NAME;
	*out = in;

	ASSERT(result = calloc(1, sizeof(PFunction)));

	result->name = in;
	while (*in)
		switch (state)
		{
			case FPS_NAME:
			{
				if (IS_FUNCTION)
				{
					in++;
					result->namelen++;
				}
				else
				{
					ASSERT(result->namelen);
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
				ASSERT(CURRENT_PARAMETER = parse_terms(memory, in, &in));
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
	if (state != FPS_EMPTY_PARAM && state != FPS_PARAMETER && result->namelen)
	{
		*out = in;
		return result;
	}

	fail:
	free_function(result);
	return NULL;
}

void free_expression(PExpression* expression)
{
	if (!expression)
		return;

	free_terms(expression->right);
	free(expression);
}

void free_terms(PTerm* term)
{
	PTerm* temp;
	while (term)
	{
		temp = term->next;

		free_factors(term->factors);
		free(term);

		term = temp;
	}
}

void free_factors(PFactor* factor)
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
				free_terms(factor->value);
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

	for (uint8_t i = 0; i < CMD_PARAM_COUNT; i++)
		free_terms(function->args[i]);

	free(function);
}

char* strfactortype(uint8_t type)
{
	switch (type)
	{
		case FACTOR_NUMBER:
			return "Sayi";
		case FACTOR_NODE:
			return "Matris";
		case FACTOR_FUNCTION:
			return "Fonksiyon";
		case FACTOR_PARENTHESIS:
			return "Parantez";
		default:
			return "Bilinmeyen";
	}
}

void print_indentation(uint8_t indent)
{
	for (uint8_t i = 0; i < indent; i++)
		printf("\t");
}

void print_factor(PFactor* factor, uint8_t indent)
{
	PFunction* pfunc;
	int i;

	print_indentation(indent);
	printf("- %s Carpan (%c)", strfactortype(factor->type), factor->divisor ? '/' : '*');

	switch (factor->type)
	{
		case FACTOR_NUMBER:
			printf(": %g", *(float*)factor->value);
			break;
		case FACTOR_NODE:
			if (factor->value)
				printf(": %c", ((Node*)factor->value)->name);
			else
				printf(": Matris bulunamadi.");
			break;
		case FACTOR_FUNCTION:
		{
			pfunc = factor->value;
			printf(": %.*s\n", pfunc->namelen, pfunc->name);
			print_indentation(indent);
			printf("  Argumanlar\n");
			print_indentation(indent);
			printf("  (\n");
			for (i = 0; i < pfunc->argcount; i++)
			{
				print_term(pfunc->args[i], indent + 1);
				if (i != pfunc->argcount - 1)
					printf("\n\n");
			}
			printf("\n");
			print_indentation(indent);
			printf("  )");
			break;
		}
		case FACTOR_PARENTHESIS:
			if (factor->value)
			{
				printf("\n");
				print_term(factor->value, indent + 1);
			}
			else
				printf(": Terim bulunamadi.");
			break;
		default:
			break;
	}

	if (factor->next)
	{
		printf("\n");
		print_factor(factor->next, indent);
	}
}

void print_term(PTerm* term, uint8_t indent)
{
	print_indentation(indent);
	printf("Terim (%c)\n", term->negative ? '-' : '+');
	print_factor(term->factors, indent);
	if (term->next)
	{
		printf("\n\n");
		print_term(term->next, indent);
	}
}

void print_expression(PExpression* expression)
{
	if (!expression)
	{
		printf("Ayristirma basarisiz.");
		return;
	}

	printf("%s\n", expression->assignment ? "Atama" : "Ifade");

	if (expression->assignment)
	{
		if (expression->left)
			printf("Sol Taraf: %c", expression->left->name);
		else
			printf("Sol Taraf: Matris bulunamadi.");
		printf("\n\n");
	}

	if (!expression->right)
	{
		printf("Terim bulunamadi.");
		return;
	}

	print_term(expression->right, 0);
}

void cmd_clear(CMD_PARAM_DECL)
{

}