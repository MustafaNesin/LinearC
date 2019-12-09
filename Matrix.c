#include "Matrix.h"
#include <stdlib.h>

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
		return 0;

	for (int i = 0; i < result->rowCount * result->columnCount; i++)
		*(result->data + i) = *(matrix1->data + i) + *(matrix2->data + i);

	return result;
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