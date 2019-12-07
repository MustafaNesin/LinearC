#pragma once
typedef struct Matrix
{
	int rowCount;
	int columnCount;
	float* data;
} Matrix;

// Parametre olarak verilen iki matrisi toplar ve sonucu d�nd�r�r.
// E�er matrislerin boyutu birbirine e�it de�ilse NULL (0) d�nd�r�r.
Matrix* matrix_addition(Matrix*, Matrix*);