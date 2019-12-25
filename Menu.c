#include "Console.h"
#include "Matrix.h"
#include "Menu.h"
#include "Utils.h"

#define EQ_ASSERT(X_COND, X_ERR) if(!(X_COND)) { printf("Hata: %s", X_ERR); goto end; }

void loop_menu(Menu* menu, Memory* memory)
{
	void (*func)(MENU_PARAM_DECL);

	while (func = menu->functions[show_menu(menu)])
	{
		clear();
		func(memory);
	}
}

int show_menu(Menu* menu)
{
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
		printf("\n> ");

		scanl("%d", &opt);
	} while (error = (opt < 0 || opt > 9 || !menu->options[opt]));

	return opt;
}

void print_equterm(float value, char name, bool first, bool last)
{
	if (iszero(value))
	{
		if (first && last)
			printf("0");
		return;
	}

	if (first)
	{
		if (last)
			printf("%g", value);
		else if (value == -1)
			printf("-%c", name);
		else if (value == 1)
			printf("%c", name);
		else
			printf("%g%c", value, name);
	}
	else
	{
		printf(value > 0 ? " + " : " - ");

		if (value == -1 || value == 1)
			printf("%c", name);
		else
		{
			printf("%g%c", fabsf(value), name);
		}
	}
}

void menu_define(MENU_PARAM_DECL)
{
	int rows, cols;
	char c, name;
	float* data;

	// Matris adý al, ayný adda varsa üzerine yazýlýp yazýlmayacaðýný sor
	{
		printf("Matris Adi (Buyuk harf): ");
		scanl("%c", &name);
		if (name < 0 || !isupper(name))
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
	char buffer[CON_BUFFER_SIZE] = { 0 }, *endp;
	bool newline = true;
	PExpression* input;

	printf("Geri donmek icin return yazin.\n> ");

	while (true)
	{
		(void)readl(buffer, CON_BUFFER_SIZE);

		input = parse_formula(memory, (char*)buffer, &endp);

		newline = input;

		if (*endp)
			printf("Sozdizimi hatasi.");

		else if (input && !run_command(memory, input, &newline))
		{
			free_formula(input);
			break;
		}

		if (newline)
			printf("\n");

		printf("> ");
		free_formula(input);
	}
}

void menu_equation(MENU_PARAM_DECL)
{
	//                                                                                            col
	// 31 30 29 28 27 26 25 24 23 22 21 20 19 18 27 16 15 14 13 12 11 10 9  8  7  6  5  4  3  2  1  0
	//                   z  y  x  w  v  u  t  s  r  q  p  o  n  m  l  k  j  i  h  g  f  e  d  c  b  a
	uint32_t flags = 0, _flags;
	char buffer[CON_BUFFER_SIZE], var[MAX_MATRIX_SIZE], b;
	int i = -1, state = 0, rows = 0, row, col, len, cols = 0, _ = 0;
	Matrix *matrix1 = NULL, *matrix2 = NULL;
	float* temp, number, con[MAX_MATRIX_SIZE];
	bool negative, first = true;

	printf("Denklem sisteminde bulunan degisken isimlerini boslukla ayirarak giriniz.\n");
	printf("Degisken isimleri kucuk harf olmalidir. (e haric ve maks. %d degisken)\n\n> ", MAX_MATRIX_SIZE);

	EQU_READ_LINE;

	while (++i < len)
	{
		b = buffer[i];

		if (isspace(b))
			continue;

		EQ_ASSERT(islower(b) && b != 'e', "Degisken isimleri kucuk harf olmalidir. (e haric)");

		flags |= 1 << (b - 'a');
	}

	EQ_ASSERT(flags, "Hicbir degisken belirtilmedi.");

	do
	{
		EQ_ASSERT(cols < MAX_MATRIX_SIZE, "Maksimum degisken sayisi asildi.");
		if (flags & 1)
			var[cols++] = _ + 'a';
		_++;
	} while (flags >>= 1);

	printf("\n\nHer satira bir esitlik yazin. En fazla %d esitlik yazilabilir.\n", MAX_MATRIX_SIZE);
	printf("Denklem sistemini cozmek icin bos satir girin.\n\n> ");

	while (EQU_READ_LINE)
	{
		EQ_ASSERT(rows < MAX_MATRIX_SIZE, "Maksimum esitlik sayisi asildi.");

		if (!rows)
		{
			EQ_ASSERT(matrix1 = mx_create_all(1, cols, 0), "Hafiza hatasi.");
		}
		else
		{
			EQ_ASSERT(temp = realloc(matrix1->data, cols * ++matrix1->rows * sizeof(float)), "Hafiza hatasi.");
			matrix1->data = temp;
			memset(matrix1->data + cols * rows, 0, cols * sizeof(float));
		}

		i = 0;
		while (i < len)
		{
			b = buffer[i];

			if (isspace(b))
			{
				i++;
				continue;
			}

			switch (state)
			{
				case -1: //son
					printf("Hata: Esitligin sag tarafi sadece bir sayidan olusmalidir.\nKarakter: %c", b);
					goto end;
				case 0: //opsiyonel iþaret
					negative = false;
					if (b == '-' || b == '+')
						state = 2;
					else
						state = 1;
					break;
				case 1: //sayý ya da deðiþken
					number = 1;
					if (isdigit(b))
						state = 3;
					else if (islower(b))
						state = 4;
					else
					{
						printf("Hata: Sayi ya da degisken adi bekleniyordu.\nKarakter: %c", b);
						goto end;
					}
					break;
				case 2: //zorunlu iþaret
					if (b != '-' && b != '+')
					{
						printf("Hata: Isaret (+ ya da -) bekleniyordu.\nKarakter: %c", b);
						goto end;
					}
					negative = b == '-';
					i++;
					state = 1;
					break;
				case 3: //sayý
					if (!sscanf(buffer + i, "%f%n", &number, &_))
					{
						printf("Hata: Sayi bekleniyordu.");
						goto end;
					}
					i += _;
					state = 4;
					break;
				case 4: //degisken
					if (!islower(b))
					{
						printf("Hata: Degisken bekleniyordu.\nKarakter: %c", b);
						goto end;
					}

					_ = 0;
					do
					{
						if (var[_] == b)
							break;
					} while (++_ < cols);

					if (_ == cols)
					{
						printf("Hata: %c degiskeni basta tanimlanmamis.", b);
						goto end;
					}

					temp = matrix1->data + (matrix1->cols * rows) + _;
					if (negative)
						*temp -= number;
					else
						*temp += number;

					i++;
					state = 5;
					break;
				case 5: //iþaret ya da eþittir
					if (b == '=')
						state = 6;
					else if (b == '-' || b == '+')
						state = 2;
					else
					{
						printf("Hata: Isaret (+ ya da -) ya da esittir (=) bekleniyordu.\nKarakter: %c", b);
						goto end;
					}
					break;
				case 6: //eþittir
					if (b != '=')
					{
						printf("Hata: Esittir (=) bekleniyordu.\nKarakter: %c", b);
						goto end;
					}
					i++;
					state = 7;
					break;
				case 7: //sabit
					if (!sscanf(buffer + i, "%f%n", &con[rows], &_))
					{
						printf("Hata: Sayi bekleniyordu.");
						goto end;
					}
					i += _;
					state = -1;
					break;
				default:
					break;
			}
		}

		EQ_ASSERT(state == -1, "Girdiginiz denklem anlasilamadi.");
		state = 0;
		rows++;
		printf("> ");
	}

	EQ_ASSERT(rows, "Hicbir denklem belirtilmedi.");
	printf("\n");

	EQ_ASSERT(temp = malloc(rows * sizeof(float)), "Hafiza hatasi.");
	for (i = 0; i < rows; i++)
		temp[i] = con[i];
	matrix2 = mx_new(rows, 1, temp);

	printf("\n");

	EQ_ASSERT(check_system(matrix1, matrix2), "Denklem sistemi tutarsiz oldugundan cozum yok.");
	
	for (row = 0, col = 0; col < cols; col++)
	{
		number = *mx_get(matrix1, row, col);

		if (iszero(number))
		{
			printf("%c: Herhangi bir deger.\n", var[col]);
			continue;
		}

		printf("%c = ", var[col]);

		first = true;
		for (i = col + 1; i < cols; i++)
		{
			if (iszero(number = *mx_get(matrix1, row, i)))
				continue;

			print_equterm(-number, var[i], first, false);
			first = false;
		}

		print_equterm(*mx_get(matrix2, row, 0), ' ', first, true);
		printf("\n");

		if (++row == rows)
		{
			for (; ++col < cols;)
				printf("%c: Herhangi bir deger.\n", var[col]);
			break;
		}
	}

	end:
	mx_free(matrix1);
	mx_free(matrix2);
	get_char();
}

void menu_save(MENU_PARAM_DECL)
{
	int count = mem_save(memory);
	if (count >= 0)
		printf("Tanimlanmis %d matris dosyaya basariyla kaydedildi.", count);
	else
		printf("Tanimlanmis matrisler dosyaya kaydedilemedi.");

	get_char();
}

void menu_read(MENU_PARAM_DECL)
{
	if (memory->tail)
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