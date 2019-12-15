#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

Matrix* mx_assign(Matrix* dest, Matrix* src)
{
	float* data = malloc(src->rows * src->cols * sizeof(float));
	if (!data)
		return NULL;

	dest->rows = src->rows;
	dest->cols = src->cols;

	if (dest->data)
		free(dest->data);
	dest->data = data;

	memcpy(dest->data, src->data, src->rows * src->cols * sizeof(float));
	return dest;
}

Matrix* mx_copy(Matrix* matrix)
{
	Matrix* copy = malloc(sizeof(Matrix));
	if (!copy)
		return NULL;
	memcpy(copy, matrix, sizeof(Matrix));

	float* data = malloc(matrix->rows * matrix->cols * sizeof(float));
	if (!data)
	{
		free(copy);
		return NULL;
	}
	memcpy(data, matrix->data, matrix->rows * matrix->cols * sizeof(float));

	copy->rows = matrix->rows;
	copy->cols = matrix->cols;
	copy->data = data;

	return copy;
}

float* mx_get(Matrix* matrix, int row, int col)
{
	return matrix->data + row * matrix->cols + col;
}

int mx_iseq(Matrix* matrix1, Matrix* matrix2)
{
	for (int i = 0; i < matrix1->rows * matrix1->cols; i++)
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

	for (int i = 0; i < result->rows * result->cols; i++)
		*(result->data + i) = *(matrix1->data + i) + *(matrix2->data + i);

	return result;
}

Matrix* mx_sdot(float scalar, Matrix* matrix)
{
	Matrix* result = malloc(sizeof(matrix));

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

	for (int i = 0; i < result->rows * result->cols; i++)
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

	for (int i = 0, j, k, p = 0; i < result->rows; i++)
		for (j = 0; j < result->cols; j++, p++)
			for (k = 0; k < matrix1->cols; k++)
				*(result->data + p) += *mx_get(matrix1, i, k) * *mx_get(matrix2, k, j);

	return result;
}

Matrix* mx_t(Matrix* matrix)
{
	Matrix* result = malloc(sizeof(matrix));

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

	for (int j = 0, p = 0; j < result->rows; j++)
		for (int i = 0; i < result->cols; i++, p++)
			*(result->data + p) = *mx_get(matrix, i, j);

	return result;
}

void mx_rowswitch(Matrix* matrix, int row1, int row2)
{
	int row_size = matrix->cols * sizeof(float);
	float* temp = malloc(row_size);
	if (!temp)
		return;

	float* r1 = matrix->data + row1 * matrix->cols;
	float* r2 = matrix->data + row2 * matrix->cols;

	memcpy(temp, r1, row_size);	// temp	= r1
	memcpy(r1, r2, row_size);	// r1 = r2
	memcpy(r2, temp, row_size);	// r2 = temp

	free(temp);
}

void mx_rowcoeff(Matrix* matrix, int row, float coeff)
{
	 for (int j = 0; j < matrix->cols; j++)
		*mx_get(matrix, row, j) *= coeff;
}

void mx_rowadd(Matrix* matrix, int row1, float coeff, int row2)
{
	for (int j = 0; j < matrix->cols; j++)
		*mx_get(matrix, row1, j) += coeff * *mx_get(matrix, row2, j);
}