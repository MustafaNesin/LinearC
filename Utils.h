#pragma once
#include "Menu.h"

// Kullanýcýnýn ekrana girdiði karakterlerden ilkini döndürür, kalanýný da imha eder
// Dönüþ deðeri -1 ise karakter girilmemiþ demektir, yoksa karakterin kendisini döndürür
int getSingleChar();

// Seçenek menüsü gösterir ve geçerli bir seçeneðin rakamýný döndürür
int getOption(Menu menu);

// Konsol ekranýný temizler
void clear();