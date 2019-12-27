#include "Console.h"
#include "Matrix.h"
#include "Menu.h"
#include "Utils.h"

#define EQ_ASSERT(X_COND, X_ERR) if(!(X_COND)) { setcolor(12); printf("Hata: %s", X_ERR); goto end; }

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

		setcolor(7);
		printf("%s", menu->title);

		setcolor(11);
		for (i = 1; i < 10; i++)
		{
			printf("\n");
			if (!menu->options[i])
				continue;
			printf("%d. %s", i, menu->options[i]);
		}
		if (menu->options[0])
			printf("\n0. %s", menu->options[0]);

		setcolor(12);
		printf("\n%s", error ? "Lutfen uygun bir secenek secin." : " ");
		setcolor(7);
		printf("\n> ");

		scanl("%d", &opt);
	} while (error = (opt < 0 || opt > 9 || !menu->options[opt]));

	return opt;
}

void print_equterm(float value, char name, bool first, bool last)
{
	value = froundf(value);

	if (!value)
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
			printf("%g%c", fabsf(value), name);
	}
}


void menu_matrices(MENU_PARAM_DECL)
{
	loop_menu(&memory->menus[MATRIX_MENU], memory);
}

void menu_console(MENU_PARAM_DECL)
{
	char buffer[CON_BUFFER_SIZE] = { 0 }, *endp;
	bool newline = true;
	PExpression* input;

	printf("Islem Konsolu\n\n");
	setcolor(8);
	printf("Geri donmek icin return yazin.");
	setcolor(7);
	printf("\n> ");

	while (true)
	{
		(void)readl(buffer, CON_BUFFER_SIZE);

		input = parse_formula(memory, (char*)buffer, &endp);

		newline = input;

		if (*endp)
		{
			setcolor(12);
			printf("Sozdizimi hatasi.\n");
		}

		else if (input && !run_command(memory, input, &newline))
		{
			free_formula(input);
			break;
		}

		setcolor(7);
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

	printf("Denklem Cozucu\n\n");
	uint32_t flags = 0;
	char buffer[CON_BUFFER_SIZE], var[MAX_MATRIX_SIZE], b;
	int i = -1, state = 0, rows = 0, row, col, len, cols = 0, _ = 0;
	Matrix *matrix1 = NULL, *matrix2 = NULL;
	float* temp, number, con[MAX_MATRIX_SIZE];
	bool negative, first = true;

	setcolor(8);
	printf("Denklem sisteminde bulunan degisken isimlerini boslukla ayirarak giriniz.\n");
	printf("Degisken isimleri kucuk harf olmalidir. (maks. %d degisken)\n\n", MAX_MATRIX_SIZE);

	setcolor(7);
	printf("> ");
	EQU_READ_LINE;

	while (++i < len)
	{
		b = buffer[i];

		if (isspace(b))
			continue;

		EQ_ASSERT(islower(b), "Degisken isimleri kucuk harf olmalidir.");

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

	setcolor(8);
	printf("\nHer satira bir esitlik yazin. En fazla %d esitlik yazilabilir.\n", MAX_MATRIX_SIZE);
	printf("Denklem sistemini cozmek icin bos satir girin.\n\n");

	setcolor(7);
	printf("> ");
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
					setcolor(12);
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
						setcolor(12);
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
					if (!(_ = sscan_ufloat(buffer + i, &number)))
					{
						setcolor(12);
						printf("Hata: Sayi bekleniyordu.");
						goto end;
					}
					i += _;
					state = 4;
					break;
				case 4: //degisken
					if (!islower(b))
					{
						setcolor(12);
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
						setcolor(12);
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
						setcolor(12);
						printf("Hata: Isaret (+ ya da -) ya da esittir (=) bekleniyordu.\nKarakter: %c", b);
						goto end;
					}
					break;
				case 6: //eþittir
					if (b != '=')
					{
						setcolor(12);
						printf("Hata: Esittir (=) bekleniyordu.\nKarakter: %c", b);
						goto end;
					}
					i++;
					state = 7;
					break;
				case 7: //sabit: opsiyonel iþaret
					negative = false;

					if (b == '-' || b == '+')
					{
						negative = b == '-';
						i++;
					}

					state = 8;
					break;
				case 8: 
					if (!(_ = sscan_ufloat(buffer + i, &con[rows])))
					{
						setcolor(12);
						printf("Hata: Sayi bekleniyordu.");
						goto end;
					}

					if (negative)
						con[rows] *= -1;

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

	setcolor(11);
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
	setcolor(7);
	printf("\n");
	get_char();
}

void menu_file(MENU_PARAM_DECL)
{
	loop_menu(&memory->menus[FILE_MENU], memory);
}

void menu_about(MENU_PARAM_DECL)
{
	printf("Hakkinda\n\n");
	setcolor(11);
	printf("Bu yazilim Mustafa Nesin ve Cem Ufuk Yilmaz tarafindan hazirlanmistir.\n");
	printf("Temel Ozellikler\n");
	printf("- Temel Matematik Islemleri\n");
	printf("- Cesitli Matris Islemleri (transpoz, carpma, toplama, ters, eselon form vb.)\n");
	printf("- Lineer Denklem Sistemi Cozme\n");
	printf("Detayli bilgi: https://github.com/MustafaNesin/LinearC\n");
	get_char();
}

void menu_define(MENU_PARAM_DECL)
{
	int rows, cols;
	char c, name;
	float* data;

	printf("Matrisler | Yeni\n\n");

	// Matris adý al, ayný adda varsa üzerine yazýlýp yazýlmayacaðýný sor
	{
		printf("Matris Adi (Buyuk harf): ");
		scanl("%c", &name);
		if (name < 0 || !isupper(name))
		{
			setcolor(12);
			printf("Matris adi buyuk harf olmalidir.");
			get_char();
			return;
		}
		Node* node;
		if (node = mem_query(memory, name))
		{
			printf("Ayni adda bir matris zaten var. Uzerine yazilsin mi? (E/H): ");
			scanl("%c", &c);
			if ((c | 0x20) != 'e')
			{
				setcolor(12);
				printf("Islem iptal edildi.");
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
			setcolor(12);
			printf("Matris satir sayisi %d ile %d arasinda olmalidir.", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			get_char();
			return;
		}

		printf("Sutun Sayisi: ");
		scanl("%d", &cols);
		if (cols < MIN_MATRIX_SIZE || cols > MAX_MATRIX_SIZE)
		{
			setcolor(12);
			printf("Matris sutun sayisi %d ile %d arasinda olmalidir.", MIN_MATRIX_SIZE, MAX_MATRIX_SIZE);
			get_char();
			return;
		}
	}

	// Matris oluþtur
	{
		data = calloc(rows * cols, sizeof(float));
		if (!data)
		{
			setcolor(12);
			printf("Yeni matris verisi icin bellek yeri ayirilirken hata olustu.");
			get_char();
			return;
		}

		if (!mem_new(memory, name, rows, cols, data))
		{
			setcolor(12);
			free(data);
			printf("Yeni matris icin bellek yeri ayirilirken hata olustu.");
			get_char();
			return;
		}
	}

	// Matris içeriðini al;
	int p = 0;
	for (uint8_t i = 0, j; i < rows; i++)
		for (j = 0; j < cols; j++, p++)
		{
			setcolor(11);
			printf("%c[%d, %d] = ", name, i + 1, j + 1);
			scanl("%f", data + p);
		}
}

void menu_list(MENU_PARAM_DECL)
{
	uint8_t count = 0;

	printf("Matrisler | Liste\n\n");

	setcolor(11);
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

void menu_show(MENU_PARAM_DECL)
{
	printf("Matrisler | Goruntule\n\n");
	
	int len;
	char buffer[CON_BUFFER_SIZE], name;
	Node* node;

	setcolor(8);
	printf("Goruntulemek istediginiz matrisin adini yazin.\nCikmak icin bos satir girin.");

	while (true)
	{
		setcolor(7);
		printf("\n\n> ");

		if (!(len = readl(buffer, CON_BUFFER_SIZE)))
			break;

		if (len == 1)
		{
			name = buffer[0];
			if (!isupper(name))
			{
				setcolor(12);
				printf("Matris adi buyuk harf olmalidir.");
			}
			else if (node = mem_query(memory, name))
			{
				setcolor(11);
				mx_print(node->matrix);
			}
			else
			{
				setcolor(12);
				printf("Matris bulunamadi.");
			}
		}
		else
		{
			setcolor(12);
			printf("Gecersiz giris. Yalnizca matris adi girilmelidir.");
		}
	}
}

void menu_delete(MENU_PARAM_DECL)
{
	printf("Matrisler | Sil\n\n");

	char buffer[CON_BUFFER_SIZE], name, len;
	Node* node;

	setcolor(8);
	printf("Silmek istediginiz matrisin adini yazin.\nCikmak icin bos satir girin.");

	while (true)
	{
		setcolor(7);
		printf("\n\n> ");

		if (!(len = readl(buffer, CON_BUFFER_SIZE)))
			break;

		if (len == 1)
		{
			name = buffer[0];
			if (!isupper(name))
			{
				setcolor(12);
				printf("Matris adi buyuk harf olmalidir.");
			}
			else if (node = mem_query(memory, name))
			{
				setcolor(11);
				mem_remove(memory, node);
				printf("Matris basariyla silindi.");
			}
			else
			{
				setcolor(12);
				printf("Matris bulunamadi.");
			}
		}
		else
		{
			setcolor(12);
			printf("Gecersiz giris. Yalnizca matris adi girilmelidir.");
		}
	}
}

void menu_clear(MENU_PARAM_DECL)
{
	printf("Matrisler | Hafizayi Temizle\n\n");

	char c;
	printf("Tanimli olan tum matrisler silinecektir, devam edilsin mi? (E/H):");
	scanl("%c", &c);

	if ((c | 0x20) != 'e')
	{
		setcolor(12);
		printf("Islem iptal edildi.");
		get_char();
		return;
	}

	mem_remove_all(memory);
	printf("Hafizadaki tum matrisler basariyla silindi.\n");
	get_char();
}

void menu_save(MENU_PARAM_DECL)
{
	printf("Dosya Islemleri | Kaydet\n\n");

	int count = mem_save(memory);
	if (count >= 0)
	{
		setcolor(10);
		printf("Tanimlanmis %d matris dosyaya basariyla kaydedildi.", count);
	}
	else
	{
		setcolor(12);
		printf("Tanimlanmis matrisler dosyaya kaydedilemedi.");
	}

	get_char();
}

void menu_read(MENU_PARAM_DECL)
{
	printf("Dosya Islemleri | Yukle\n\n");
	if (memory->tail)
	{
		char c;
		printf("Mevcut tanimli matrislerin uzerine yazilacaktir, devam edilsin mi? (E/H): ");
		scanl("%c", &c);
		if ((c | 0x20) != 'e')
		{
			setcolor(12);
			printf("Islem iptal edildi.");
			get_char();
			return;
		}
	}

	int count = mem_read(memory);
	if (count >= 0)
	{
		setcolor(10);
		printf("%d adet matris kayit dosyasindan basariyla yuklendi.", count);
	}
	else
	{
		setcolor(12);
		printf("Kayit dosyasi yok veya bozuk, matrislerin tamami okunamadi.");
	}

	get_char();
}