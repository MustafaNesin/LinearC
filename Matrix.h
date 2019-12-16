#pragma once
#include <stdint.h>
#include "Memory.h"

Matrix* mx_new(uint8_t rows, uint8_t cols, float* data);
void mx_free(Matrix* matrix);
void mx_print(Matrix* matrix);
Matrix* mx_copy(Matrix* matrix);
Matrix* mx_create_all(uint8_t rows, uint8_t cols, float value);
Matrix* mx_create_diag(uint8_t size, float value);
Matrix* mx_create_low(uint8_t size, float value);
Matrix* mx_create_up(uint8_t size, float value);
float* mx_get(Matrix* matrix, uint8_t row, uint8_t col);
void mx_set(Matrix* matrix, uint8_t row, uint8_t col, float value);
int mx_isequal(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_add(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_multiply(float scalar, Matrix* matrix);
Matrix* mx_dot(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_transpose(Matrix* matrix);
void mx_rowswitch(Matrix* matrix, uint8_t row1, uint8_t row2);
void mx_rowcoeff(Matrix* matrix, uint8_t row, float coeff);
void mx_rowadd(Matrix* matrix, uint8_t row1, float coeff, uint8_t row2);