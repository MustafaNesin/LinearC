#include <stdio.h>
#include "Main.h"

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
		mf_back,
		mf_define
	}
};

int mf_define(MxMemory* memory)
{
	printf("Henuz tanimlanmadi.");
	(void)getchar();
	return 1;
}