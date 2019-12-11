#pragma once
typedef struct
{
	int rows;
	int cols;
	float* data;
} Matrix;

// Verilen satýr ve sütun indisine göre matrisin oraya konumlandýrýlmýþ göstericisini döndürür.
float* mx_get(Matrix*, int, int);

// Verilen iki matris birbirine eþit deðilse NULL (0) döndürür.
int mx_is_eq(Matrix*, Matrix*);

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

// Elementer satýr iþlemleri 1: Yer deðiþtirme
// Eðer iþlem baþarýsýz olursa NULL (0) döndürür.
int mx_row_op_switch(Matrix*, int, int);

// Elementer satýr iþlemleri 2: Bir katsayý ile çarpma
// Eðer iþlem baþarýsýz olursa NULL (0) döndürür.
int mx_row_op_coeff(Matrix*, int, float);

// Elementer satýr iþlemleri 3: Baþka bir satýrýn katýný ekleme
// Eðer iþlem baþarýsýz olursa NULL (0) döndürür.
int mx_row_op_add(Matrix*, int, float, int);