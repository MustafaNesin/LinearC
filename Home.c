#include <stdio.h>
#include "Menu.h"

Menu m_home = {
	// Ba�l�k
	"LineerC | Ana Menu",
	// Se�enekler
	{
		"Cikis",
		"Matris Tanimla"
	},
	// Se�eneklerin �al��t�raca�� fonksiyonlar
	{
		kill,
		matrisDefinition
	}
};

// �zel fonksiyonlar

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