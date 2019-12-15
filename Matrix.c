#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Matrix.h"

Matrix* mx_new(int rows, int cols, float* data)
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
	for (int i = 0, j, p = 0; i < matrix->rows; i++)
	{
		printf("\t| ");

		for (j = 0; j < matrix->cols; j++, p++)
			printf("%10g ", *(matrix->data + p));

		printf("  | ");
		if (i == matrix->rows - 1)
			printf("%dx%d", matrix->rows, matrix->cols);
		printf("\n");
	}
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

Matrix* mx_create_all(int rows, int cols, float value)
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

	for (int i = 0, j; i < rows; i++)
		for (j = 0; j < cols; j++)
			*data++ = value;

	return matrix;
}

Matrix* mx_create_diag(int size, float value)
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

	for (int i = 0, j, p = 0; i < size; i++)
		for (j = 0; j < size; j++, p++)
			if (i == j)
				*(data + p) = value;

	return matrix;
}

Matrix* mx_create_low(int size, float value)
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

	for (int i = 0, j, p = 0; i < size; i++)
		for (j = 0; j < size; j++, p++)
			if (i >= j)
				*(data + p) = value;

	return matrix;
}

Matrix* mx_create_up(int size, float value)
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

	for (int i = 0, j, p = 0; i < size; i++)
		for (j = 0; j < size; j++, p++)
			if (i <= j)
				*(data + p) = value;

	return matrix;
}

float* mx_get(Matrix* matrix, int row, int col)
{
	return matrix->data + row * matrix->cols + col;
}

int mx_isequal(Matrix* matrix1, Matrix* matrix2)
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

	for (int j = 0, i, p = 0; j < result->rows; j++)
		for (i = 0; i < result->cols; i++, p++)
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

	(void)memcpy(temp, r1, row_size);
	(void)memcpy(r1, r2, row_size);
	(void)memcpy(r2, temp, row_size);

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