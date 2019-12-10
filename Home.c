#include <stdio.h>
#include "Main.h"

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