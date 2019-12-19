#include "Console.h"
#include "Matrix.h"
#include "Menu.h"
#include "Utils.h"

void loop_menu(Menu* menu, Memory* memory)
{
	if (!menu || !memory)
		return;

	void (*func)(MENU_PARAM_DECL);

	while (func = menu->functions[show_menu(menu)])
	{
		clear();
		func(memory);
	}
}

int show_menu(Menu* menu)
{
	if (!menu)
		return 0;

	int opt, i, error = 0;

	do
	{
		clear();

		printf("%s", menu->title);

		for (i = 1; i < 10; i++)
		{
			printf("\n");
			if (!menu->options[i])
				continue;
			printf("%d. %s", i, menu->options[i]);
		}
		if (menu->options[0])
			printf("\n0. %s", menu->options[0]);

		printf("\n%s", error ? "Lutfen uygun bir secenek secin." : " ");
		printf("\n>>> ");

		scanl("%d", &opt);
	} while (error = (opt < 0 || opt > 9 || !menu->options[opt]));

	return opt;
}

void menu_define(MENU_PARAM_DECL)
{
	if (!memory)
		return;

	int rows, cols;
	char c, name;
	float* data;

	// Matris adý al, ayný adda varsa üzerine yazýlýp yazýlmayacaðýný sor
	{
		printf("Matris Adi (Buyuk harf): ");
		scanl("%c", &name);
		if (!isupper(name))
		{
			printf("\nMatris adi buyuk bir harf olmalidir. (Yalnizca Ingiliz alfabesi harfleri)");
			get_char();
			return;
		}
		Node* node;
		if (node = mem_query(memory, name))
		{
			printf("\nAyni adda bir matris zaten var. Uzerine yazilsin mi? (E/H): ");
			scanl("%c", &c);
			if ((c | 0x20) != 'e')
			{
				printf("\nIslem kullanici tarafindan iptal edildi.");
				get_char();
				return;
			}
			mem_remove(memory, node);
		}
	}

	// Satýr ve sütun sayýsý al
	{
		printf("Satir Sayisi: ");
		scanl("%d", &rows);
		if (rows < MIN_MATRIX_SIZE || rows > MAX_MATRIX_SIZE)
		{
			printf("\nMatris satir sayisi %d ile %d arasinda olmalidir.", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			get_char();
			return;
		}

		printf("Sutun Sayisi: ");
		scanl("%d", &cols);
		if (cols < MIN_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
		{
			printf("\nMatris sutun sayisi %d ile %d arasinda olmalidir.", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			get_char();
			return;
		}
	}

	// Matris oluþtur
	{
		data = calloc(rows * cols, sizeof(float));
		if (!data)
		{
			printf("\nYeni matris verisi icin bellek yeri ayirilirken hata olustu.");
			get_char();
			return;
		}

		if (!mem_new(memory, name, rows, cols, data))
		{
			free(data);
			printf("\nYeni matris icin bellek yeri ayirilirken hata olustu.");
			get_char();
			return;
		}
	}

	// Matris içeriðini al;
	int p = 0;
	for (uint8_t i = 0, j; i < rows; i++)
		for (j = 0; j < cols; j++, p++)
		{
			printf("%c[%d, %d] = ", name, i + 1, j + 1);
			scanl("%f", data + p);
		}
}

void menu_list(MENU_PARAM_DECL)
{
	if (!memory)
		return;

	uint8_t count = 0;

	printf("\tAd\tSat\tSut");

	Node* node = memory->tail;
	while (node)
	{
		printf("\n%d.\t%c\t%d\t%d", ++count, node->name, node->matrix->rows, node->matrix->cols);
		node = node->prev;
	}

	printf("\n\n%d adet matris bulundu.", count);
	get_char();
}

void menu_console(MENU_PARAM_DECL)
{
	int input;
	char buffer[CON_BUFFER_SIZE] = { 0 };
	uint8_t length = 0;
	bool exceed = false;

	printf("Konsol hakkinda yardim almak icin help, geri donmek icin return yazin.\n> ");

	while (true)
	{
		input = getchar();

		if (input != '\n')
		{
			if (exceed)
				continue;

			if (input == EOF)
				break;

			buffer[length++] = (char)input;

			// çýkarýlan sayý parse_formula fonksiyonunda aþma yaþanmamasý için
			if (length == CON_BUFFER_SIZE - 5)
			{
				printf("\nKonsola yazilabilecek maksimum karakter sayisi asildi.");
				exceed = true;
				continue;
			}

			continue;
		}

		if (exceed)
			exceed = false;
		else
		{
			// ...
		}

		memset(buffer, 0, CON_BUFFER_SIZE);
		length = 0;
	}
}

void menu_equation(MENU_PARAM_DECL)
{
	if (!memory)
		return;

	printf("Her satira bir esitlik girin.");
}

void menu_save(MENU_PARAM_DECL)
{
	if (!memory)
		return;

	int count = mem_save(memory);
	if (count >= 0)
		printf("Tanimlanmis %d matris dosyaya basariyla kaydedildi.", count);
	else
		printf("Tanimlanmis matrisler dosyaya kaydedilemedi.");

	get_char();
}

void menu_read(MENU_PARAM_DECL)
{
	if (!memory)
		return;

	{
		char c;
		printf("Mevcut tanimli matrisler silinecektir, devam edilsin mi? (E/H): ");
		scanl("%c", &c);
		if ((c | 0x20) != 'e')
		{
			printf("Islem kullanici tarafindan iptal edildi.");
			get_char();
			return;
		}
	}

	int count = mem_read(memory);
	if (count >= 0)
		printf("%d adet matris kayit dosyasindan basariyla yuklendi.", count);
	else
		printf("Kayit dosyasi bozuk, matrisler okunamadi.");

	get_char();
}