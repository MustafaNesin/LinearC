#include "Console.h"
#include "Matrix.h"
#include "Utils.h"

#define ASSERT__FAIL(X_COND) if (!(X_COND)) goto fail;
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

#define ASSERT__ERROR(X_COND, X_ERR) if (!(X_COND)) return X_ERR;

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
	RETURN_RESULT;

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
		RETURN_RESULT;
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
		RETURN_RESULT;
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
		RETURN_RESULT;
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
		
		for (uint8_t i = 0; i < CMD_COUNT; i++)
		{
			func = &memory->commands[i];

			if (func->paramcount != pfunc->argcount || pfunc->argcount)
				continue;

			if (strcmp(func->name, pfunc->name))
				continue;

			if (func->returns)
				continue;

			if (!memory->commands[i].function)
				return false;

			setcolor(11);
			(void)memory->commands[i].function(memory, 0, &error);
			
			if (!strcmp(func->name, "clear"))
				*newline = false;
			else
				printf("\n");

			return true;
		}
	}

	if (error = check_formula(memory, input))
	{
		setcolor(12);
		printf("%s", error);
	}
	else
	{
		EValue result = evaluate_formula(memory, input, &error);

		if (error)
		{
			setcolor(12);
			printf("%s", error);
		}
		else if (result.number)
		{
			setcolor(11);
			printf("= %g", froundf(result.value.number));
		}
		else
		{
			setcolor(11);
			mx_print(result.value.matrix);
			mx_free(result.value.matrix);
		}
	}

	printf("\n");
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
				ASSERT__ERROR(func->returns, 
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
		RETURN_RESULT;

	if (input->assignment)
	{
		if (result.number)
		{
			*error = "Matris skaler bir sayiya esitlenemez.";
			RETURN_RESULT;
		}

		if (input->left)
			mem_remove(memory, input->left);

		Matrix* copy = mx_copy(result.value.matrix);
		if (!copy)
		{
			*error = "Kritik hafiza hatasi.";
			mx_free(result.value.matrix);
			RETURN_RESULT;
		}
		if (!(input->left = mem_add(memory, input->leftname, copy)))
		{
			*error = "Kritik hafiza hatasi.";
			mx_free(copy);
			mx_free(result.value.matrix);
			RETURN_RESULT;
		}
	}

	RETURN_RESULT;
}

EValue evaluate_expression(Memory* memory, PTerm* input, char** error)
{
	PTerm* tail = input;
	EValue result = evaluate_term(memory, tail->factors, error), term;

	if (*error)
		RETURN_RESULT;

	if (tail->negative)
	{
		if (result.number)
			result.value.number *= -1;
		else
			mx_multiply(result.value.matrix, -1);
	}

	while (tail = tail->next)
	{
		term = evaluate_term(memory, tail->factors, error);

		if (*error)
		{
			if (!result.number)
				mx_free(result.value.matrix);
			RETURN_RESULT;
		}

		if (term.number)
		{
			if (!result.number)
			{
				*error = "Skaler sayilar matrislerle toplanamaz.";
				mx_free(result.value.matrix);
				RETURN_RESULT;
			}

			if (tail->negative)
				term.value.number *= -1;

			result.value.number += term.value.number;
		}
		else
		{
			if (result.number)
			{
				*error = "Skaler sayilar matrislerle toplanamaz.";
				mx_free(term.value.matrix);
				RETURN_RESULT;
			}

			if (result.value.matrix->rows != term.value.matrix->rows ||
				result.value.matrix->cols != term.value.matrix->cols)
			{
				*error = "Toplama/cikarma yapabilmek icin matrislerin boyutlari birbirine esit olmalidir.";
				mx_free(term.value.matrix);
				mx_free(result.value.matrix);
				RETURN_RESULT;
			}

			if (tail->negative)
				mx_multiply(term.value.matrix, -1);

			mx_add(result.value.matrix, term.value.matrix);
			mx_free(term.value.matrix);
		}
	}

	RETURN_RESULT;
}

EValue evaluate_term(Memory* memory, PFactor* input, char** error)
{
	PFactor* tail = input;
	EValue result = evaluate_factor(memory, tail->value, tail->type, error), factor;

	if (*error)
		RETURN_RESULT;

	while (tail = tail->next)
	{
		factor = evaluate_factor(memory, tail->value, tail->type, error);

		if (*error)
		{
			if (!result.number)
				mx_free(result.value.matrix);
			RETURN_RESULT;
		}

		if (factor.number)
		{
			if (tail->divisor)
				factor.value.number = 1 / factor.value.number;

			if (result.number)
				result.value.number *= factor.value.number;
			else
				mx_multiply(result.value.matrix, factor.value.number);
		}
		else
		{
			if (tail->divisor)
			{
				*error = "Bolen bir matris olamaz.";
				mx_free(factor.value.matrix);
				if (!result.number)
					mx_free(result.value.matrix);
				RETURN_RESULT;
			}

			if (result.number)
			{
				float s = result.value.number;
				result.number = false;
				result.value.matrix = factor.value.matrix;
				mx_multiply(result.value.matrix, s);
			}

			else if (result.value.matrix->cols != factor.value.matrix->rows)
			{
				*error = "Matris boyutlari carpmaya uygun degil.";
				mx_free(factor.value.matrix);
				mx_free(result.value.matrix);
				RETURN_RESULT;
			}

			else
			{
				mx_dot(result.value.matrix, factor.value.matrix);
				mx_free(factor.value.matrix);
			}
		}
	}

	RETURN_RESULT;
}

EValue evaluate_factor(Memory* memory, void* input, uint8_t type, char** error)
{
	INIT_RESULT;

	switch (type)
	{
		case FACTOR_NUMBER:
			result.number = true;
			result.value.number = *(float*)input;
			break;
		case FACTOR_NODE:
			if (!input)
			{
				*error = "Hata: Tanimlanmamis matris kullanimi.";
				RETURN_RESULT;
			}
			result.number = false;
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
				int p;
				for (p = 0; p < func->paramcount; p++)
				{
					args[p] = evaluate_expression(memory, pfunc->args[p], error);
					if (*error)
						RETURN_RESULT;
					if (func->params[p] != args[p].number)
						goto cont;
				}

				result = func->function(memory, args, error);
				for (p = 0; p < func->paramcount; p++)
					if (!args[p].number)
						mx_free(args[p].value.matrix);

				RETURN_RESULT;
				cont:;
			}

			*error = "Fonksiyon parametreleri uyusmuyor.";
			break;
		}
		case FACTOR_PARENTHESIS:
			result = evaluate_expression(memory, input, error);
			break;
		default:
			*error = "Kritik hata."; //check fonksiyonunda zaten kontrol edildi
			break;
	}

	RETURN_RESULT;
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

#pragma region Commands
EValue cmd_clear(CMD_PARAM_DECL)
{
	INIT_RESULT;

	clear();

	RETURN_RESULT;
}

#pragma region Matrix
EValue cmd_list(CMD_PARAM_DECL)
{
	INIT_RESULT;

	int count = 0;
	Node* tail = memory->tail;

	while (tail)
	{
		if (!count++)
			printf("\tAd\tBoyut\n");

		printf("\t%c\t%dx%d\n", tail->name, tail->matrix->rows, tail->matrix->cols);

		tail = tail->prev;
	}

	if (count)
		printf("\n");

	printf("\t%d sonuc bulundu.", count);

	RETURN_RESULT;
}

EValue cmd_get(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	float row = GET_NUMBER_ARG(1), col = GET_NUMBER_ARG(2);

	if (ceilf(row) != row || ceilf(col) != col)
	{
		*error = "Satir ve sutun numaralari tam sayi olmalidir.";
		RETURN_RESULT;
	}

	if (row < MIN_MATRIX_SIZE || row > matrix->rows || col < MIN_MATRIX_SIZE || col > matrix->cols)
	{
		*error = "Satir ve sutun numaralari matris boyutunu asiyor.";
		RETURN_RESULT;
	}

	SET_NUMBER_RESULT(*mx_get(matrix, (uint8_t)(row - 1), (uint8_t)(col - 1)));
	RETURN_RESULT;
}

EValue cmd_set(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	float value = GET_NUMBER_ARG(1), row = GET_NUMBER_ARG(2), col = GET_NUMBER_ARG(3);
	
	if (ceilf(row) != row || ceilf(col) != col)
	{
		*error = "Satir ve sutun numaralari tam sayi olmalidir.";
		RETURN_RESULT;
	}

	if (row < MIN_MATRIX_SIZE || row > matrix->rows || col < MIN_MATRIX_SIZE || col > matrix->cols)
	{
		*error = "Satir ve sutun numaralari matris boyutunu asiyor.";
		RETURN_RESULT;
	}

	mx_set(matrix, (uint8_t)(row - 1), (uint8_t)(col - 1), value);
	SET_MATRIX_RESULT(mx_copy(matrix));
	RETURN_RESULT;
}

EValue cmd_transpose(CMD_PARAM_DECL)
{
	INIT_RESULT;

	result.number = false;
	result.value.matrix = mx2_transpose(GET_MATRIX_ARG(0));

	RETURN_RESULT;
}

EValue cmd_inverse(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	if (matrix->rows == matrix->cols)
	{
		result.number = false;
		result.value.matrix = mx2_inverse(matrix);

		if (!result.value.matrix)
			*error = "Matrisin tersi yok.";
	}
	else
		*error = "Yalnizca kare matrislerin tersi olabilir.";

	RETURN_RESULT;
}

EValue cmd_determinant(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	if (matrix->rows == matrix->cols)
	{
		result.number = true;
		result.value.number = mx_determinant(matrix);
	}
	else
		*error = "Yalnizca kare matrislerin determinanti olabilir.";

	RETURN_RESULT;
}

EValue cmd_adjoint(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	if (matrix->rows == matrix->cols)
	{
		result.number = false;
		result.value.matrix = mx2_adjoint(matrix);

		if (!result.value.matrix)
			*error = "Determinanti sifir olan matrisin ek matrisini bulma desteklenmiyor.";
	}
	else
		*error = "Yalnizca kare matrislerin ek matrisi olabilir.";

	RETURN_RESULT;
}

EValue cmd_rank(CMD_PARAM_DECL)
{
	INIT_RESULT;

	result.number = true;
	result.value.number = mx_rank(GET_MATRIX_ARG(0));

	RETURN_RESULT;
}

EValue cmd_id(CMD_PARAM_DECL)
{
	INIT_RESULT;
	float size = GET_NUMBER_ARG(0);

	if (ceilf(size) != size || size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE)
	{
		*error = "Gecersiz matris boyutu.";
		RETURN_RESULT;
	}

	SET_MATRIX_RESULT(mx_create_diag((uint8_t)size, 1));
	RETURN_RESULT;
}

EValue cmd_diag(CMD_PARAM_DECL)
{
	INIT_RESULT;
	float value = GET_NUMBER_ARG(0), size = GET_NUMBER_ARG(1);

	if (ceilf(size) != size || size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE)
	{
		*error = "Gecersiz matris boyutu.";
		RETURN_RESULT;
	}

	SET_MATRIX_RESULT(mx_create_diag((uint8_t)size, value));
	RETURN_RESULT;
}

EValue cmd_low(CMD_PARAM_DECL)
{
	INIT_RESULT;
	float value = GET_NUMBER_ARG(0), size = GET_NUMBER_ARG(1);

	if (ceilf(size) != size || size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE)
	{
		*error = "Gecersiz matris boyutu.";
		RETURN_RESULT;
	}

	SET_MATRIX_RESULT(mx_create_low((uint8_t)size, value));
	RETURN_RESULT;
}

EValue cmd_up(CMD_PARAM_DECL)
{
	INIT_RESULT;
	float value = GET_NUMBER_ARG(0), size = GET_NUMBER_ARG(1);

	if (ceilf(size) != size || size < MIN_MATRIX_SIZE || size > MAX_MATRIX_SIZE)
	{
		*error = "Gecersiz matris boyutu.";
		RETURN_RESULT;
	}

	SET_MATRIX_RESULT(mx_create_up((uint8_t)size, value));
	RETURN_RESULT;
}

EValue cmd_all(CMD_PARAM_DECL)
{
	INIT_RESULT;
	float value = GET_NUMBER_ARG(0), rows = GET_NUMBER_ARG(1), cols = GET_NUMBER_ARG(2);

	if (ceilf(rows) != rows || rows < MIN_MATRIX_SIZE || rows > MAX_MATRIX_SIZE ||
		ceilf(cols) != cols || cols < MIN_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
	{
		*error = "Gecersiz matris boyutu.";
		RETURN_RESULT;
	}

	SET_MATRIX_RESULT(mx_create_all((uint8_t)rows, (uint8_t)cols, value));
	RETURN_RESULT;
}

EValue cmd_rowop(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0), *r;
	Operation op = mx_next_op(matrix, false, false), op2;

	if (!op.type)
		op = mx_next_op(matrix, false, true);

	switch (op.type)
	{
		case OP_ADD:
			printf("r%d -> r%d%+gr%d\n", op.vec2 + 1, op.vec2 + 1, op.coeff, op.vec1 + 1);
			break;
		case OP_MULTIPLY:
			printf("r%d -> %gr%d\n", op.vec1 + 1, op.coeff, op.vec1 + 1);
			break;
		case OP_SWITCH:
			printf("r%d <-> r%d\n", op.vec1 + 1, op.vec2 + 1);
			break;
		default:
			break;
	}

	r = mx2_apply_op(matrix, op);

	op = mx_next_op(r, false, false);
	op2 = mx_next_op(r, false, true);
	if (!op2.type)
		printf("RREF: Indirgenmis Satir Eselon Formu\n");
	else if (!op.type)
		printf("REF: Satir Eselon Formu\n");

	SET_MATRIX_RESULT(r);
	RETURN_RESULT;
}

EValue cmd_rowswt(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	float num1 = GET_NUMBER_ARG(1), num2 = GET_NUMBER_ARG(2);

	if (ceilf(num1) != num1 || ceilf(num2) != num2)
	{
		*error = "Satir numaralari tam sayi olmalidir.";
		RETURN_RESULT;
	}

	if (num1 < MIN_MATRIX_SIZE || num2 < MIN_MATRIX_SIZE || num1 > matrix->rows || num2 > matrix->rows)
	{
		*error = "Satir numaralari matris boyutunu asiyor.";
		RETURN_RESULT;
	}

	if (num1 == num2)
	{
		*error = "Satir numaralari ayni olamaz.";
		RETURN_RESULT;
	}

	printf("r%d <-> r%d\n", (int)num1, (int)num2);

	Operation op = { false, OP_SWITCH, (uint8_t)(num1 - 1), (uint8_t)(num2 - 1), 0.0f };
	SET_MATRIX_RESULT(mx2_apply_op(matrix, op));
	RETURN_RESULT;
}

EValue cmd_rowmul(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	float num1 = GET_NUMBER_ARG(1), num2 = GET_NUMBER_ARG(2);

	if (iszero(num2))
	{
		*error = "Satir ile carpilacak katsayi 0 olamaz.";
		RETURN_RESULT;
	}

	if (ceilf(num1) != num1)
	{
		*error = "Satir numarasi tam sayi olmalidir.";
		RETURN_RESULT;
	}

	if (num1 < MIN_MATRIX_SIZE || num1 > matrix->rows)
	{
		*error = "Satir numaralari matris boyutunu asiyor.";
		RETURN_RESULT;
	}

	printf("r%d -> %gr%d\n", (int)num1, num2, (int)num1);

	Operation op = { false, OP_MULTIPLY, (uint8_t)(num1 - 1), (uint8_t)0, num2 };
	SET_MATRIX_RESULT(mx2_apply_op(matrix, op));
	RETURN_RESULT;
}

EValue cmd_rowadd(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	float num1 = GET_NUMBER_ARG(1), num2 = GET_NUMBER_ARG(2), num3 = GET_NUMBER_ARG(3);

	if (iszero(num3))
	{
		*error = "Satir ile carpilacak katsayi 0 olamaz.";
		RETURN_RESULT;
	}

	if (ceilf(num1) != num1 || ceilf(num2) != num2)
	{
		*error = "Satir numaralari tam sayi olmalidir.";
		RETURN_RESULT;
	}

	if (num1 < MIN_MATRIX_SIZE || num2 < MIN_MATRIX_SIZE || num1 > matrix->rows || num2 > matrix->rows)
	{
		*error = "Satir numaralari matris boyutunu asiyor.";
		RETURN_RESULT;
	}

	if (num1 == num2)
	{
		*error = "Satir numaralari ayni olamaz.";
		RETURN_RESULT;
	}

	printf("r%d -> r%d%+gr%d\n", (int)num1, (int)num1, num3, (int)num2);

	Operation op = { false, OP_ADD, (uint8_t)(num2 - 1), (uint8_t)(num1 - 1), num3 };
	SET_MATRIX_RESULT(mx2_apply_op(matrix, op));
	RETURN_RESULT;
}

EValue cmd_power(CMD_PARAM_DECL)
{
	INIT_RESULT;

	result.number = args[0].number;

	if (result.number)
		result.value.number = powf(GET_NUMBER_ARG(0), GET_NUMBER_ARG(1));
	else
	{
		Matrix* matrix = GET_MATRIX_ARG(0), *copy;
		float power = GET_NUMBER_ARG(1);
		if (matrix->rows == matrix->cols)
			if (ceilf(power) == power)
			{
				int p = (int)power;

				if (p < 0)
				{
					copy = mx2_inverse(matrix);
					if (!copy)
					{
						*error = "Tersi olmayan matrislerin negatif kuvveti alinamaz.";
						RETURN_RESULT;
					}
				}
				else
					copy = mx_copy(matrix);
				
				result.value.matrix = mx_create_diag(matrix->rows, 1);

				p = abs(p);
				for (uint8_t i = 0; i < p; i++)
					mx_dot(result.value.matrix, copy);

				mx_free(copy);
			}
			else
				*error = "Matrislerin yalnizca tam sayi kuvveti alinabilir.";
		else
			*error = "Yalnizca kare matrislerin kuvveti alinabilir.";
	}

	RETURN_RESULT;
}
#pragma endregion

#pragma region Math
EValue cmd_abs(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(fabsf(GET_NUMBER_ARG(0))); }

EValue cmd_pi(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( MATH_PI ); }

EValue cmd_e(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( MATH_E ); }

EValue cmd_sqrt(CMD_PARAM_DECL)
{
	INIT_RESULT;

	float input = GET_NUMBER_ARG(0);

	if (input >= 0)
	{
		result.number = true;
		result.value.number = sqrtf(input);
	}
	else
		*error = "Karekoku alinacak sayi negatif olamaz.";

	RETURN_RESULT;
}

EValue cmd_ln(CMD_PARAM_DECL)
{
	INIT_RESULT;

	float x = GET_NUMBER_ARG(0);

	if (x > 0)
	{
		result.number = true;
		result.value.number = logf(x);
	}
	else
		*error = "Dogal logaritmasi hesaplanacak sayi pozitif olmalidir.";

	RETURN_RESULT;
}

EValue cmd_log(CMD_PARAM_DECL)
{
	INIT_RESULT;

	float x = GET_NUMBER_ARG(0);
	float base = 10;

	if (args[1].number)
		base = GET_NUMBER_ARG(1);

	if (base > 0 && base != 1)
	{
		if (x > 0)
		{
			result.number = true;

			result.value.number = log10f(x);

			if (base != 10)
				result.value.number /= log10f(base);
		}
		else
			*error = "Logaritmasi hesaplanacak sayi pozitif olmalidir.";
	}
	else
		*error = "Logaritma tabani 1 haric bir pozitif sayi olmalidir.";

	RETURN_RESULT;
}

EValue cmd_deg(CMD_PARAM_DECL)
{
	INIT_RESULT;

	if (memory->radian)
	{
		memory->radian = false;
		printf("Gecerli aci birimi derece yapildi.");
	}
	else
		printf("Gecerli aci birimi: Derece.");

	RETURN_RESULT;
}

EValue cmd_rad(CMD_PARAM_DECL)
{
	INIT_RESULT;

	if (memory->radian)
		printf("Gecerli aci birimi: Radyan.");
	else
	{
		memory->radian = true;
		printf("Gecerli aci birimi radyan yapildi.");
	}

	RETURN_RESULT;
}

EValue cmd_sin(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( sinf(GET_RADIAN_ARG(0)) ); }

EValue cmd_cos(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( cosf(GET_RADIAN_ARG(0)) ); }

EValue cmd_tan(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( tanf(GET_RADIAN_ARG(0)) ); }

EValue cmd_cot(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( 1.0f / tanf(GET_RADIAN_ARG(0)) ); }

EValue cmd_sec(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( 1.0f / cosf(GET_RADIAN_ARG(0)) ); }

EValue cmd_csc(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT( 1.0f / sinf(GET_RADIAN_ARG(0)) ); }

EValue cmd_sinh(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(sinhf(GET_NUMBER_ARG(0))); }

EValue cmd_cosh(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(coshf(GET_NUMBER_ARG(0))); }

EValue cmd_tanh(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(tanhf(GET_NUMBER_ARG(0))); }

EValue cmd_coth(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(1.0f / tanhf(GET_NUMBER_ARG(0))); }

EValue cmd_sech(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(1.0f / coshf(GET_NUMBER_ARG(0))); }

EValue cmd_csch(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(1.0f / sinhf(GET_NUMBER_ARG(0))); }

EValue cmd_asin(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(asinf(GET_NUMBER_ARG(0))); }

EValue cmd_acos(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(acosf(GET_NUMBER_ARG(0))); }

EValue cmd_atan(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(atanf(GET_NUMBER_ARG(0))); }

EValue cmd_acot(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(atanf(1.0f / GET_NUMBER_ARG(0))); }

EValue cmd_asec(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(acosf(1.0f / GET_NUMBER_ARG(0))); }

EValue cmd_acsc(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(asinf(1.0f / GET_NUMBER_ARG(0))); }

EValue cmd_asinh(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(asinhf(GET_NUMBER_ARG(0))); }

EValue cmd_acosh(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(acoshf(GET_NUMBER_ARG(0))); }

EValue cmd_atanh(CMD_PARAM_DECL) { RETURN_NUMBER_RESULT(atanhf(GET_NUMBER_ARG(0))); }

EValue cmd_acoth(CMD_PARAM_DECL)
{
	float x = GET_NUMBER_ARG(0);
	x = logf((x + 1) / (x - 1)) / 2;
	RETURN_NUMBER_RESULT(x);
}

EValue cmd_asech(CMD_PARAM_DECL)
{
	float x = GET_NUMBER_ARG(0);
	x = logf(1 / x + sqrtf(1 / powf(x, 2) - 1));
	RETURN_NUMBER_RESULT(x);
}

EValue cmd_acsch(CMD_PARAM_DECL) 
{ 
	float x = GET_NUMBER_ARG(0);
	x = logf(1 / x + sqrtf(1 / powf(x, 2) + 1));
	RETURN_NUMBER_RESULT(x);
}
#pragma endregion
#pragma endregion