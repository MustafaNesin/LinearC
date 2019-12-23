#include "Console.h"
#include "Matrix.h"
#include "Utils.h"

#ifndef CMD_SYNTAX
#define CMD_SYNTAX

#define ASSERT__FAIL(X_COND) if (!(X_COND)) goto fail;
#define IS_SPACE (*in < -1 || isspace(*in))
#define EAT_SPACE if (IS_SPACE) { in++; continue; }
#define _EAT_SPACE while (*in) { EAT_SPACE; break; }

#define IS_NUMBER (*in > -1 && isdigit(*in))
#define IS_MATRIX (*in > -1 && isupper(*in))
#define IS_NAME (*in > -1 && islower(*in))
#define IS_FUNCTION (*in > -1 && islower(*in))
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

#define ASSERT__ERROR(X_COND, X_ERR) if (!(X_COND)) return X_ERR;
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

		for (int i = 0; i < CMD_COUNT; i++)
		{
			func = &memory->commands[i];

			if (func->paramcount != pfunc->argcount)
				continue;

			if (strcmp(func->name, pfunc->name))
				continue;

			if (func->returns)
				continue;

			if (!memory->commands[i].function)
				return false;

			(void)memory->commands[i].function(memory, 0, &error);
			
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
			printf("= %g", froundf(result.value.scalar));
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
		if (result.scalar)
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
			RETURN_RESULT;
		}

		if (term.scalar)
		{
			if (!result.scalar)
			{
				*error = "Skaler sayilar matrislerle toplanamaz.";
				mx_free(result.value.matrix);
				RETURN_RESULT;
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
			if (!result.scalar)
				mx_free(result.value.matrix);
			RETURN_RESULT;
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
				RETURN_RESULT;
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
			result.scalar = true;
			result.value.scalar = *(float*)input;
			break;
		case FACTOR_NODE:
			if (!input)
			{
				*error = "Hata: Tanimlanmamis matris kullanimi.";
				RETURN_RESULT;
			}
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
				int p;
				for (p = 0; p < func->paramcount; p++)
				{
					args[p] = evaluate_expression(memory, pfunc->args[p], error);
					if (*error)
						RETURN_RESULT;
					if (func->params[p] != args[p].scalar)
						goto cont;
				}

				result = func->function(memory, args, error);
				for (p = 0; p < func->paramcount; p++)
					if (!args[p].scalar)
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
EValue cmd_transpose(CMD_PARAM_DECL)
{
	INIT_RESULT;

	result.scalar = false;
	result.value.matrix = mx2_transpose(GET_MATRIX_ARG(0));

	RETURN_RESULT;
}

EValue cmd_inverse(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	if (matrix->rows == matrix->cols)
	{
		result.scalar = false;
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
		result.scalar = true;
		result.value.scalar = mx_determinant(matrix);
	}
	else
		*error = "Yalnizca kare matrislerin determinanti olabilir.";

	RETURN_RESULT;
}

EValue cmd_adjoint(CMD_PARAM_DECL)
{
	INIT_RESULT;

	Matrix* matrix = GET_MATRIX_ARG(0);
	if (matrix->rows != matrix->cols)
	{
		result.scalar = false;
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

	result.scalar = true;
	result.value.scalar = mx_rank(GET_MATRIX_ARG(0));

	RETURN_RESULT;
}

EValue cmd_power(CMD_PARAM_DECL)
{
	INIT_RESULT;

	result.scalar = args[0].scalar;

	if (result.scalar)
		result.value.scalar = powf(GET_SCALAR_ARG(0), GET_SCALAR_ARG(1));
	else
	{
		Matrix* matrix = GET_MATRIX_ARG(0), *copy;
		float power = GET_SCALAR_ARG(1);
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
EValue cmd_pi(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( MATH_PI ); }

EValue cmd_e(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( MATH_E ); }

EValue cmd_sqrt(CMD_PARAM_DECL)
{
	INIT_RESULT;

	float input = GET_SCALAR_ARG(0);

	if (input >= 0)
	{
		result.scalar = true;
		result.value.scalar = sqrtf(input);
	}
	else
		*error = "Karekoku alinacak sayi negatif olamaz.";

	RETURN_RESULT;
}

EValue cmd_ln(CMD_PARAM_DECL)
{
	INIT_RESULT;

	float x = GET_SCALAR_ARG(0);

	if (x > 0)
	{
		result.scalar = true;
		result.value.scalar = logf(x);
	}
	else
		*error = "Dogal logaritmasi hesaplanacak sayi pozitif olmalidir.";

	RETURN_RESULT;
}

EValue cmd_log(CMD_PARAM_DECL)
{
	INIT_RESULT;

	float x = GET_SCALAR_ARG(0);
	float base = 10;

	if (args[1].scalar)
		base = GET_SCALAR_ARG(1);

	if (base > 0 && base != 1)
	{
		if (x > 0)
		{
			result.scalar = true;

			result.value.scalar = log10f(x);

			if (base != 10)
				result.value.scalar /= log10f(base);
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

EValue cmd_sin(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( sinf(GET_RADIAN_ARG(0)) ); }

EValue cmd_cos(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( cosf(GET_RADIAN_ARG(0)) ); }

EValue cmd_tan(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( tanf(GET_RADIAN_ARG(0)) ); }

EValue cmd_cot(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( 1.0f / tanf(GET_RADIAN_ARG(0)) ); }

EValue cmd_sec(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( 1.0f / cosf(GET_RADIAN_ARG(0)) ); }

EValue cmd_csc(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT( 1.0f / sinf(GET_RADIAN_ARG(0)) ); }

EValue cmd_sinh(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(sinhf(GET_SCALAR_ARG(0))); }

EValue cmd_cosh(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(coshf(GET_SCALAR_ARG(0))); }

EValue cmd_tanh(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(tanhf(GET_SCALAR_ARG(0))); }

EValue cmd_coth(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(1.0f / tanhf(GET_SCALAR_ARG(0))); }

EValue cmd_sech(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(1.0f / coshf(GET_SCALAR_ARG(0))); }

EValue cmd_csch(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(1.0f / sinhf(GET_SCALAR_ARG(0))); }

EValue cmd_asin(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(asinf(GET_SCALAR_ARG(0))); }

EValue cmd_acos(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(acosf(GET_SCALAR_ARG(0))); }

EValue cmd_atan(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(atanf(GET_SCALAR_ARG(0))); }

EValue cmd_acot(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(atanf(1.0f / GET_SCALAR_ARG(0))); }

EValue cmd_asec(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(acosf(1.0f / GET_SCALAR_ARG(0))); }

EValue cmd_acsc(CMD_PARAM_DECL) { RETURN_UNIT_RESULT(asinf(1.0f / GET_SCALAR_ARG(0))); }

EValue cmd_asinh(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(asinhf(GET_SCALAR_ARG(0))); }

EValue cmd_acosh(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(acoshf(GET_SCALAR_ARG(0))); }

EValue cmd_atanh(CMD_PARAM_DECL) { RETURN_SCALAR_RESULT(atanhf(GET_SCALAR_ARG(0))); }

EValue cmd_acoth(CMD_PARAM_DECL)
{
	float x = GET_SCALAR_ARG(0);
	x = logf((x + 1) / (x - 1)) / 2;
	RETURN_SCALAR_RESULT(x);
}

EValue cmd_asech(CMD_PARAM_DECL)
{
	float x = GET_SCALAR_ARG(0);
	x = logf(1 / x + sqrtf(1 / powf(x, 2) - 1));
	RETURN_SCALAR_RESULT(x);
}

EValue cmd_acsch(CMD_PARAM_DECL) 
{ 
	float x = GET_SCALAR_ARG(0);
	x = logf(1 / x + sqrtf(1 / powf(x, 2) + 1));
	RETURN_SCALAR_RESULT(x);
}
#pragma endregion
#pragma endregion