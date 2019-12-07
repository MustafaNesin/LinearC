// Visual Studio derleyicisi için uyumluluk direktifleri
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

	// Program ana yaþam döngüsü
	while (state)
	{
		opt = getOption(m_home); // Ana sayfa menüsünü göster ve seçenek al
		clear(); // Ekraný temizle
		state = m_home.functions[opt](); // Seçeneðe karþýlýk gelen fonksiyonu çaðýr ve dönütü al
		getSingleChar(); // Ekranýn kalmasý için kullanýcýdan bir karakter al
	}

	return 0;
}
