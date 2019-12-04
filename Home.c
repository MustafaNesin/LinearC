#include <stdio.h>
#include "Menu.h"

Menu m_home = {
	// Baþlýk
	"LineerC | Ana Menu",
	// Seçenekler
	{
		"Cikis",
		"Matris Tanimla"
	},
	// Seçeneklerin çalýþtýracaðý fonksiyonlar
	{
		kill,
		matrisDefinition
	}
};

// Özel fonksiyonlar

int kill()
{
	printf("Program sonlandirildi.");
	return 0;
}

int matrisDefinition()
{
	printf("Henuz tanimlanmadi.");
	return 1;
}