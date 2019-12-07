#pragma once
typedef struct Matrix
{
	int rowCount;
	int columnCount;
	float* data;
} Matrix;

// Parametre olarak verilen iki matrisi toplar ve sonucu döndürür.
// Eðer matrislerin boyutu birbirine eþit deðilse NULL (0) döndürür.
Matrix* matrix_addition(Matrix*, Matrix*);