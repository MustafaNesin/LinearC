#include <stdlib.h>
#include <string.h>
#include "Main.h"

float* mx_get(Matrix* matrix, int row, int column)
{
	if (!matrix)
		return 0;

	if (row < 0 || row >= matrix->rows || column < 0 || column >= matrix->cols)
		return 0;

	return matrix->data + row * matrix->cols + column;
}

int mx_is_eq(Matrix* matrix1, Matrix* matrix2)
{
	if (matrix1->rows != matrix2->rows || matrix1->cols != matrix2->cols)
		return 0;

	for (int i = 0; i < matrix1->rows * matrix1->cols; i++)
		if (*(matrix1->data + i) != *(matrix2->data + i))
			return 0;

	return 1;
}

Matrix* mx_add(Matrix* matrix1, Matrix* matrix2)
{
	if (!matrix1 || !matrix2)
		return 0;

	if (matrix1->cols != matrix2->cols || matrix1->rows != matrix2->rows)
		return 0;

	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return 0;

	result->rows = matrix1->rows;
	result->cols = matrix1->cols;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	for (int i = 0; i < result->rows * result->cols; i++)
		*(result->data + i) = *(matrix1->data + i) + *(matrix2->data + i);

	return result;
}

Matrix* mx_sdot(float scalar, Matrix* matrix)
{
	Matrix* result = malloc(sizeof(matrix));

	if (!result)
		return 0;

	result->rows = matrix->rows;
	result->cols = matrix->cols;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	for (int i = 0; i < result->rows * result->cols; i++)
		*(result->data + i) = *(matrix->data + i) * scalar;

	return result;
}

Matrix* mx_dot(Matrix* matrix1, Matrix* matrix2)
{
	if (!matrix1 || !matrix2)
		return 0;

	if (matrix1->cols != matrix2->rows)
		return 0;

	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return 0;

	result->rows = matrix1->rows;
	result->cols = matrix2->cols;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
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
		return 0;

	result->rows = matrix->cols;
	result->cols = matrix->rows;
	result->data = calloc(result->rows * result->cols, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	for (int j = 0, p = 0; j < result->rows; j++)
		for (int i = 0; i < result->cols; i++, p++)
			*(result->data + p) = *mx_get(matrix, i, j);

	return result;
}

int mx_row_op_switch(Matrix* matrix, int row1, int row2)
{
	if (!matrix)
		return 0;

	if (row1 == row2 || row1 < 0 || row1 >= matrix->rows || row2 < 0 || row2 >= matrix->rows)
		return 0;

	int row_size = matrix->cols * sizeof(float);
	float* temp = malloc(row_size);
	if (!temp)
		return 0;

	float* r1 = matrix->data + row1 * matrix->cols;
	float* r2 = matrix->data + row2 * matrix->cols;

	memcpy(temp, r1, row_size);	// temp	= r1
	memcpy(r1, r2, row_size);	// r1 = r2
	memcpy(r2, temp, row_size);	// r2 = temp

	free(temp);

	return 1;
}

int mx_row_op_coeff(Matrix* matrix, int row, float coeff)
{
	if (!matrix)
		return 0;

	if (coeff == 0 || row < 0 || row >= matrix->rows)
		return 0;

	 for (int j = 0; j < matrix->cols; j++)
		*mx_get(matrix, row, j) *= coeff;

	 return 1;
}

int mx_row_op_add(Matrix* matrix, int row1, float coeff, int row2)
{
	if (!matrix)
		return 0;

	if (coeff == 0 || row1 == row2 || row1 < 0 || row1 >= matrix->rows || row2 < 0 || row2 >= matrix->rows)
		return 0;

	for (int j = 0; j < matrix->cols; j++)
		*mx_get(matrix, row1, j) += coeff * *mx_get(matrix, row2, j);

	return 1;
}