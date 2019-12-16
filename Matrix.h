#pragma once
#include "Memory.h"

Matrix* mx_new(int rows, int cols, float* data);
void mx_free(Matrix* matrix);
void mx_print(Matrix* matrix);
Matrix* mx_copy(Matrix* matrix);
Matrix* mx_create_all(int rows, int cols, float value);
Matrix* mx_create_diag(int size, float value);
Matrix* mx_create_low(int size, float value);
Matrix* mx_create_up(int size, float value);
float* mx_get(Matrix* matrix, int row, int col); 
void mx_set(Matrix* matrix, int row, int col, float value);
int mx_isequal(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_add(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_multiply(float scalar, Matrix* matrix);
Matrix* mx_dot(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_transpose(Matrix* matrix);
void mx_rowswitch(Matrix* matrix, int row1, int row2);
void mx_rowcoeff(Matrix* matrix, int row, float coeff);
void mx_rowadd(Matrix* matrix, int row1, float coeff, int row2);