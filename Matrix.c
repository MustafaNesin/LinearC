#include "Matrix.h"
#include <stdlib.h>

float* mx_get(Matrix* matrix, int row, int column)
{
	if (!matrix)
		return 0;

	if (row < 0 || row >= matrix->rowCount || column < 0 || column >= matrix->columnCount)
		return 0;

	return matrix->data + row * matrix->columnCount + column;
}

Matrix* mx_add(Matrix* matrix1, Matrix* matrix2)
{
	if (!matrix1 || !matrix2)
		return 0;

	if (matrix1->columnCount != matrix2->columnCount || matrix1->rowCount != matrix2->rowCount)
		return 0;

	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return 0;

	result->rowCount = matrix1->rowCount;
	result->columnCount = matrix1->columnCount;
	result->data = calloc(result->rowCount * result->columnCount, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	for (int i = 0; i < result->rowCount * result->columnCount; i++)
		*(result->data + i) = *(matrix1->data + i) + *(matrix2->data + i);

	return result;
}

Matrix* mx_dot(Matrix* matrix1, Matrix* matrix2)
{
	if (!matrix1 || !matrix2)
		return 0;

	if (matrix1->columnCount != matrix2->rowCount)
		return 0;

	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return 0;

	result->rowCount = matrix1->rowCount;
	result->columnCount = matrix2->columnCount;
	result->data = calloc(result->rowCount * result->columnCount, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	for (int i = 0, j, k, p = 0; i < result->rowCount; i++)
		for (j = 0; j < result->columnCount; j++, p++)
			for (k = 0; k < matrix1->columnCount; k++)
				*(result->data + p) += *mx_get(matrix1, i, k) * *mx_get(matrix2, k, j);
}

Matrix* mx_sdot(float scalar, Matrix* matrix)
{
	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return 0;

	result->rowCount = matrix->rowCount;
	result->columnCount = matrix->columnCount;
	result->data = calloc(result->rowCount * result->columnCount, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	for (int i = 0; i < result->rowCount * result->columnCount; i++)
		*(result->data + i) = *(matrix->data + i) *  scalar;
}

Matrix* mx_t(Matrix* matrix)
{
	Matrix* result = malloc(sizeof(Matrix));

	if (!result)
		return 0;

	result->rowCount = matrix->columnCount;
	result->columnCount = matrix->rowCount;
	result->data = calloc(result->rowCount * result->columnCount, sizeof(float));

	if (!result->data)
	{
		free(result);
		return 0;
	}

	// To be continued...
}

int mx_is_eq(Matrix matrix1, Matrix matrix2)
{
	if (matrix1.rowCount != matrix2.rowCount || matrix1.columnCount != matrix2.columnCount)
		return 0;

	for (int i = 0; i < matrix1.rowCount * matrix1.columnCount; i++)
		if (*matrix1.data++ != *matrix2.data++)
			return 0;

	return 1;
}