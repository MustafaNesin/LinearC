#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

#pragma region Definitions
#define OP_ADD          (uint8_t)1
#define OP_MULTIPLY     (uint8_t)2
#define OP_SWITCH       (uint8_t)3
#pragma endregion

#pragma region Structures
struct Operation
{
	bool     colmode;
	uint8_t  type;
	uint8_t  vec1;
	uint8_t  vec2;
	float    coeff;
};

struct Matrix
{
	uint8_t  rows;
	uint8_t  cols;
	float    *data;
};
#pragma endregion

Matrix*   mx_new         (uint8_t rows, uint8_t cols, float* data);
void      mx_free        (Matrix* matrix);
void      mx_print       (Matrix* matrix);
Matrix*   mx_copy        (Matrix* matrix);
Matrix*   mx_create_all  (uint8_t rows, uint8_t cols, float value);
Matrix*   mx_create_diag (uint8_t size, float value);
Matrix*   mx_create_low  (uint8_t size, float value);
Matrix*   mx_create_up   (uint8_t size, float value);
float*    mx_get         (Matrix* matrix, uint8_t row, uint8_t col);
void      mx_set         (Matrix* matrix, uint8_t row, uint8_t col, float value);
bool      mx_isequal     (Matrix* matrix1, Matrix* matrix2);
Matrix*   mx_add         (Matrix* matrix1, Matrix* matrix2);
Matrix*   mx_multiply    (float scalar, Matrix* matrix);
Matrix*   mx_dot         (Matrix* matrix1, Matrix* matrix2);
Matrix*   mx_transpose   (Matrix* matrix);
Operation mx_next_op     (Matrix* matrix, bool colmode, bool reduce);
void      mx_apply_op    (Matrix* matrix, Operation operation);