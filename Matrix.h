#pragma once
typedef struct Matrix
{
	int rowCount;
	int columnCount;
	float* data;
} Matrix;

// Verilen iki matrisi toplamýný döndürür.
// Eðer matrislerin boyutu birbirine eþit deðilse NULL (0) döndürür.
Matrix* mx_add(Matrix*, Matrix*);

// Verilen sayý ile matrisin çarpýmýný döndürür.
Matrix* mx_sdot(float, Matrix*);

// Verilen iki matrisin çarpýmýný döndürür.
// Eðer birinci matrisin sütun sayýsý ikinci matrisin satýr sayýna eþit deðilse NULL (0) döndürür.
Matrix* mx_dot(Matrix*, Matrix*);

// Verilen matrisin transpozunu döndürür.
Matrix* mx_t(Matrix*);