#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

Matrix* mx_new(uint8_t rows, uint8_t cols, float* data)
{
	Matrix* matrix = malloc(sizeof(Matrix));

	if (!matrix)
		return NULL;

	matrix->rows = rows;
	matrix->cols = cols;
	matrix->data = data;

	return matrix;
}

void mx_free(Matrix* matrix)
{
	free(matrix->data);
	free(matrix);
}

void mx_print(Matrix* matrix)
{
	int i = 0;
	float element = 0;
	for (uint8_t row = 0, col; row < matrix->rows; row++)
	{
		printf("\t| ");

		for (col = 0; col < matrix->cols; col++, i++)
		{
			element = *(matrix->data + i);

			// -0'ı 0'a dönüştür
			if (element == -0.0f)
				element = 0;

			printf("%10g ", element);
		}

		printf("  | ");
		if (row == matrix->rows - 1)
			printf("%dx%d", matrix->rows, matrix->cols);
		printf("\n");
	}
	printf("\n");
}

Matrix* mx_copy(Matrix* matrix)
{
	int dsize = matrix->rows * matrix->cols * sizeof(float);
	float* data = malloc(dsize);

	if (!data)
		return NULL;

	(void)memcpy(data, matrix->data, dsize);

	Matrix* copy = mx_new(matrix->rows, matrix->cols, data);
	if (!copy)
	{
		free(data);
		return NULL;
	}
	return copy;
}

Matrix* mx_create_all(uint8_t rows, uint8_t cols, float value)
{
	float* data = malloc(rows * cols * sizeof(float));

	if (!data)
		return NULL;

	Matrix* matrix = mx_new(rows, cols, data);
	if (!matrix)
	{
		free(data);
		return NULL;
	}

	for (uint8_t row = 0, col; row < rows; row++)
		for (col = 0; col < cols; col++)
			*data++ = value;

	return matrix;
}

Matrix* mx_create_diag(uint8_t size, float value)
{
	float* data = calloc(size * size, sizeof(float));

	if (!data)
		return NULL;

	Matrix* matrix = mx_new(size, size, data);

	if (!matrix)
	{
		free(data);
		return NULL;
	}

	int i = 0;
	for (uint8_t row = 0, col; row < size; row++)
		for (col = 0; col < size; col++, i++)
			if (row == col)
				*(data + i) = value;

	return matrix;
}

Matrix* mx_create_low(uint8_t size, float value)
{
	float* data = calloc(size * size, sizeof(float));

	if (!data)
		return NULL;

	Matrix* matrix = mx_new(size, size, data);

	if (!matrix)
	{
		free(data);
		return NULL;
	}

	int i = 0;
	for (uint8_t row = 0, col; row < size; row++)
		for (col = 0; col < size; col++, i++)
			if (row >= col)
				*(data + i) = value;

	return matrix;
}

Matrix* mx_create_up(uint8_t size, float value)
{
	float* data = calloc(size * size, sizeof(float));

	if (!data)
		return NULL;

	Matrix* matrix = mx_new(size, size, data);

	if (!matrix)
	{
		free(data);
		return NULL;
	}

	int i = 0;
	for (uint8_t row = 0, col; row < size; row++)
		for (col = 0; col < size; col++, i++)
			if (row <= col)
				*(data + i) = value;

	return matrix;
}

float* mx_get(Matrix* matrix, uint8_t row, uint8_t col)
{
	return matrix->data + row * matrix->cols + col;
}

void mx_set(Matrix* matrix, uint8_t row, uint8_t col, float value)
{
	*mx_get(matrix, row, col) = value;
}

int mx_isequal(Matrix* matrix1, Matrix* matrix2)
{
	for (uint8_t i = 0; i < matrix1->rows * matrix1->cols; i++)
		if (*(matrix1->data + i) != *(matrix2->data + i))
			return 0;

	return 1;
}

Matrix* mx_add(Matrix* matrix1, Matrix* matrix2)
{
	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return NULL;

	result->rows = matrix1->rows;
	result->cols = matrix1->cols;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return NULL;
	}

	for (uint8_t i = 0; i < result->rows * result->cols; i++)
		*(result->data + i) = *(matrix1->data + i) + *(matrix2->data + i);

	return result;
}

Matrix* mx_multiply(float scalar, Matrix* matrix)
{
	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return NULL;

	result->rows = matrix->rows;
	result->cols = matrix->cols;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return NULL;
	}

	for (uint8_t i = 0; i < result->rows * result->cols; i++)
		*(result->data + i) = *(matrix->data + i) * scalar;

	return result;
}

Matrix* mx_dot(Matrix* matrix1, Matrix* matrix2)
{
	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return NULL;

	result->rows = matrix1->rows;
	result->cols = matrix2->cols;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return NULL;
	}

	int i = 0;
	for (uint8_t row = 0, col, s; row < result->rows; row++)
		for (col = 0; col < result->cols; col++, i++)
			for (s = 0; s < matrix1->cols; s++)
				*(result->data + i) += *mx_get(matrix1, row, s) * *mx_get(matrix2, s, col);

	return result;
}

Matrix* mx_transpose(Matrix* matrix)
{
	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return NULL;

	result->rows = matrix->cols;
	result->cols = matrix->rows;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return NULL;
	}

	int i = 0;
	for (uint8_t col = 0, row; col < matrix->cols; col++)
		for (row = 0; row < matrix->rows; row++, i++)
			*(result->data + i) = *mx_get(matrix, row, col);

	return result;
}

Operation mx_next_op(Matrix* matrix, uint8_t colmode, uint8_t reduce)
{
	Operation op = { 0 };
	op.colmode = colmode;

	uint8_t
		vecs = colmode ? matrix->cols : matrix->rows,
		xvecs = colmode ? matrix->rows : matrix->cols,
		vec = 0,
		xvec = 0,
		_vec,
		factor1 = colmode ? 1 : xvecs,
		factor2 = colmode ? xvecs : 1;

	float element, _element;

	while (vec < vecs && xvec < xvecs)
	{
		op.vec1 = vec;
		// Satır işlemlerinde element;
		//  X[vec, xvec] yani vec row, xvec col
		// Sütun işlemlerinde element;
		//  X[xvec, vec] yani vec col, xvec row
		element = *(matrix->data + vec * factor1 + xvec * factor2);

		if (element == 0)
		{
			for (_vec = vec + 1; _vec < vecs; _vec++)
				if (_element = *(matrix->data + _vec * factor1 + xvec * factor2))
				{
					op.type = OP_SWITCH;
					op.vec2 = _vec;
					return op;
				}

			xvec++;
		}

		else if (element == 1)
		{
			for (_vec = vec + 1; _vec < vecs; _vec++)
				if (_element = *(matrix->data + _vec * factor1 + xvec * factor2))
				{
					op.type = OP_ADD;
					op.vec2 = _vec;
					op.coeff = -_element / element;
					return op;
				}

			// İndirgenmiş eşelon formu
			if (reduce && vec > 0)
			{
				_vec = vec - 1;
				do
					if (_element = *(matrix->data + _vec * factor1 + xvec * factor2))
					{
						op.type = OP_ADD;
						op.vec2 = _vec;
						op.coeff = -_element / element;
						return op;
					}
				while (_vec--);
			}

			xvec++;
			vec++;
		}

		else
		{
			op.type = OP_MULTIPLY;
			op.vec1 = vec;
			op.coeff = 1 / element;
			return op;
		}
	}

	op.vec1 = 0;
	return op;
}

void mx_apply_op(Matrix* matrix, Operation operation)
{
	uint8_t vecs = operation.colmode ? matrix->rows : matrix->cols;

	switch (operation.type)
	{
		case OP_ADD:
		{
			uint8_t vec = 0;
			if (operation.colmode)
				for (; vec < vecs; vec++)
					*mx_get(matrix, vec, operation.vec2) += operation.coeff * *mx_get(matrix, vec, operation.vec1);
			else
				for (; vec < vecs; vec++)
					*mx_get(matrix, operation.vec2, vec) += operation.coeff * *mx_get(matrix, operation.vec1, vec);
			break;
		}
		case OP_MULTIPLY:
		{
			uint8_t vec = 0;
			if (operation.colmode)
				for (; vec < vecs; vec++)
					*mx_get(matrix, vec, operation.vec1) *= operation.coeff;
			else
				for (; vec < vecs; vec++)
					*mx_get(matrix, operation.vec1, vec) *= operation.coeff;
			break;
		}
		case OP_SWITCH:
		{
			if (operation.colmode)
			{
				float v, *v1, *v2;
				for (uint8_t row = 0; row < vecs; row++)
				{
					v1 = mx_get(matrix, row, operation.vec1);
					v2 = mx_get(matrix, row, operation.vec2);

					v = *v1;
					*v1 = *v2;
					*v2 = v;
				}
			}

			else
			{
				int size = matrix->cols * sizeof(float);
				float* temp = malloc(size);
				if (!temp)
					return;

				float* row1 = matrix->data + operation.vec1 * vecs;
				float* row2 = matrix->data + operation.vec2 * vecs;

				(void)memcpy(temp, row1, size);
				(void)memcpy(row1, row2, size);
				(void)memcpy(row2, temp, size);

				free(temp);
			}
			break;
		}
		default:
			break;
	}
}
