#include "Console.h"

#ifndef CMDPS
#define CMDPS
#define ASSERT(cond) if (!(cond)) goto fail;
#define EATSPACE if (isspace(*in)) continue;
#define _EATSPACE while (*in) { EATSPACE; break; }

#define IS_MATRIX isupper(*in)
#define IS_ASSIGNMENT (expression->assignment = *in++ == '=')
#endif

PExpression* parse_expression(Memory* memory, char* in, char** out)
{
	PExpression* expression;
	Node* node;
	*out = in;

	ASSERT(expression = calloc(1, sizeof(PExpression)));

	_EATSPACE;

	if (IS_MATRIX)
	{
		expression->left = mem_query(memory, *in++);

		_EATSPACE;

		if (!IS_ASSIGNMENT)
		{
			expression->left = NULL;
			in = *out;
		}
	}

	ASSERT(expression->right = parse_terms(memory, in, &in));

	*out = in;

	fail:
	free(expression);
	return NULL;
}

PTerm* parse_terms(Memory* memory, char* in, char** out)
{
	PTerm* result;
	PTerm* tail;


	if (tail)
		tail = tail->next = malloc(sizeof(PTerm));
	else
		tail = result = malloc(sizeof(PTerm));


	fail:
	while (result)
	{
		tail = result->next;
		//free_factors(result->terms->factors)
		free(result);
		result = tail;
	}
	return result;
}