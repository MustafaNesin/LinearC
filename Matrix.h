#pragma once
#include "Memory.h"

Matrix* mx_assign(Matrix* dest, Matrix* src);
Matrix* mx_copy(Matrix* matrix);
float* mx_get(Matrix* matrix, int row, int col);
int mx_iseq(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_add(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_sdot(float scalar, Matrix* matrix);
Matrix* mx_dot(Matrix* matrix1, Matrix* matrix2);
Matrix* mx_t(Matrix* matrix);
void mx_rowswitch(Matrix* matrix, int row1, int row2);
void mx_rowcoeff(Matrix* matrix, int row, float coeff);
void mx_rowadd(Matrix* matrix, int row1, float coeff, int row2);