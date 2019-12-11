#pragma once
typedef struct
{
	int rows;
	int cols;
	float* data;
} Matrix;

// Verilen sat�r ve s�tun indisine g�re matrisin oraya konumland�r�lm�� g�stericisini d�nd�r�r.
float* mx_get(Matrix*, int, int);

// Verilen iki matris birbirine e�it de�ilse NULL (0) d�nd�r�r.
int mx_is_eq(Matrix*, Matrix*);

// Verilen iki matrisi toplam�n� d�nd�r�r.
// E�er matrislerin boyutu birbirine e�it de�ilse NULL (0) d�nd�r�r.
Matrix* mx_add(Matrix*, Matrix*);

// Verilen say� ile matrisin �arp�m�n� d�nd�r�r.
Matrix* mx_sdot(float, Matrix*);

// Verilen iki matrisin �arp�m�n� d�nd�r�r.
// E�er birinci matrisin s�tun say�s� ikinci matrisin sat�r say�na e�it de�ilse NULL (0) d�nd�r�r.
Matrix* mx_dot(Matrix*, Matrix*);

// Verilen matrisin transpozunu d�nd�r�r.
Matrix* mx_t(Matrix*);

// Elementer sat�r i�lemleri 1: Yer de�i�tirme
// E�er i�lem ba�ar�s�z olursa NULL (0) d�nd�r�r.
int mx_row_op_switch(Matrix*, int, int);

// Elementer sat�r i�lemleri 2: Bir katsay� ile �arpma
// E�er i�lem ba�ar�s�z olursa NULL (0) d�nd�r�r.
int mx_row_op_coeff(Matrix*, int, float);

// Elementer sat�r i�lemleri 3: Ba�ka bir sat�r�n kat�n� ekleme
// E�er i�lem ba�ar�s�z olursa NULL (0) d�nd�r�r.
int mx_row_op_add(Matrix*, int, float, int);