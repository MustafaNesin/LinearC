#pragma once
typedef struct Matrix
{
	int rowCount;
	int columnCount;
	float* data;
} Matrix;

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