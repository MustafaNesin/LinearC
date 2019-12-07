#pragma once
typedef struct Matrix
{
	short rowCount;
	short columnCount;
	float* data;
} Matrix;

// Parametre olarak verilen iki matrisi toplar ve döndürür.
// Eðer matrislerin boyutu birbirine eþit deðilse NULL (0) döndürür.
Matrix* matrix_addition(Matrix*, Matrix*);