#pragma once
#include "Menu.h"

// Kullan�c�n�n ekrana girdi�i karakterlerden ilkini d�nd�r�r, kalan�n� da imha eder
// D�n�� de�eri -1 ise karakter girilmemi� demektir, yoksa karakterin kendisini d�nd�r�r
int getSingleChar();

// Se�enek men�s� g�sterir ve ge�erli bir se�ene�in rakam�n� d�nd�r�r
int getOption(Menu menu);

// Konsol ekran�n� temizler
void clear();