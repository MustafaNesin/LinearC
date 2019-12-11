#include <stdio.h>
#include <stdlib.h>
#include "Main.h"

Menu m_home = {
	// Baþlýk
	"LineerC | Ana Menu",
	// Seçenekler
	{
		"Cikis",
		"Yeni Matris",
		"Matris Listesi"
	},
	// Seçeneklerin çalýþtýracaðý fonksiyonlar
	{
		mf_back,
		mf_define,
		mf_list
	}
};

int mf_define(MxMemory* memory)
{
	if (!memory)
		return 1;

	int rows, cols;
	char c, name;
	Node* node;

	// Matris adý al, ayný adda varsa üstüne yazýlýp yazýlmayacaðýný sor
	{
		printf("Matris Adi (Buyuk harf): ");
		scanl("%c", &name);
		if (name < 'A' || name > 'Z')
			return mf_error("\nMatris adi buyuk bir harf olmalidir. (Yalnizca Ingiliz alfabesi harfleri)");
		if (node = search_node(memory, name))
		{
			printf("\nAyni adda bir matris zaten var. Uzerine yazilsin mi? (E/H): ");
			scanl("%c", &c);
			if ((c | 0x20) != 'e')
				return mf_error("\nMatris tanimlanmadi.");
			free_node(memory, node);
		}
	}

	// Satýr ve sütun sayýsý al
	{
		printf("Satir Sayisi: ");
		scanl("%d", &rows);
		if (rows < MIN_DEF_MX_SIZE || rows > MAX_DEF_MX_SIZE)
			return mf_error("\nMatris satir sayisi %d ile %d arasinda olmalidir.", MIN_DEF_MX_SIZE, MAX_DEF_MX_SIZE);

		printf("Sutun Sayisi: ");
		scanl("%d", &cols);
		if (cols < MIN_DEF_MX_SIZE || cols > MAX_DEF_MX_SIZE)
			return mf_error("\nMatris sutun sayisi %d ile %d arasinda olmalidir.", MIN_DEF_MX_SIZE, MAX_DEF_MX_SIZE);
	}

	// Matris oluþtur
	Matrix* matrix = malloc(sizeof(Matrix));
	{
		if (!matrix)
			return mf_error("\nYeni matris icin bellek yeri ayirilirken hata olustu.");
		matrix->data = calloc(rows * cols, sizeof(float));
		if (!matrix->data)
		{
			free(matrix);
			return mf_error("\nYeni matris verisi icin bellek yeri ayirilirken hata olustu.");
		}
		matrix->rows = rows;
		matrix->cols = cols;
		node = new_node(memory, name, matrix);
	}

	// Matris içeriðini al
	float* d = matrix->data;
	for (int i = 0, j; i < rows; i++)
		for (j = 0; j < cols; j++)
		{
			printf("%c[%d, %d] = ", name, i + 1, j + 1);
			scanl("%f", d++);
		}

	return 1;
}

int mf_list(MxMemory* memory)
{
	if (!memory)
		return 1;

	int count = 0;

	printf("\tAd\tSat\tSut");

	Node* node = memory->tail;
	while (node)
	{
		printf("\n%d.\t%c\t%d\t%d", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("\n\n%d adet matris bulundu.", count);
	get_char();
	return 1;
}