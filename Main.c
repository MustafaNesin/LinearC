// Visual Studio derleyicisi i�in uyumluluk direktifleri
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:6031)

#include <stdio.h>
#include <Windows.h>
#include "Menu.h"
#include "Utils.h"

extern Menu m_home;

int main()
{
	int opt, state = 1;

	// Program ana ya�am d�ng�s�
	while (state)
	{
		opt = getOption(m_home); // Ana sayfa men�s�n� g�ster ve se�enek al
		clear(); // Ekran� temizle
		state = m_home.functions[opt](); // Se�ene�e kar��l�k gelen fonksiyonu �a��r ve d�n�t� al
		getSingleChar(); // Ekran�n kalmas� i�in kullan�c�dan bir karakter al
	}

	return 0;
}
