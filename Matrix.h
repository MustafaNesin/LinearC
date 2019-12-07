#pragma once
typedef struct Matrix
{
	short rowCount;
	short columnCount;
	float* data;
} Matrix;

// Parametre olarak verilen iki matrisi toplar ve d�nd�r�r.
// E�er matrislerin boyutu birbirine e�it de�ilse NULL (0) d�nd�r�r.
Matrix* matrix_addition(Matrix*, Matrix*);