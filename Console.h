#pragma once
#include "Memory.h"
#define CMD_COUNT 14	// Dikkat! Her yeni komut eklenildiðinde bu sayý artýrýlmalýdýr.

#define CMD_PARAMS_FAIL \
{ \
	printf("\tParametreler uyusmuyor.\n\tYardim: help(%s)\n\n", parsed->name); \
	return; \
}

#define CMD_MEMORY_FAIL \
{ \
	printf("\tHafiza yetersiz.\n\n"); \
	return; \
}

#define CMD_MATRIX_FAIL \
{ \
	printf("\tKonsolda bellege alinmis bir matris yok.\n\n"); \
	return; \
}

#define CMD_SEARCH_FAIL(i) \
{ \
	printf("\t%s adinda bir matris bulunamadi.\n\n", parsed->args[i]); \
	return; \
}

#define CMD_SET_MEMORY				\
	if (!matrix)					\
		CMD_MEMORY_FAIL;			\
	if (memory->matrix)				\
		mx_free(memory->matrix);	\
	memory->matrix = matrix;		\
	mx_print(matrix);

#define CMD_HELP_RETURN		"\
return                        Ana menuye geri dondurur."

#define CMD_HELP_HELP		"\
help                          Tum fonksiyonlarin kullanim bilgisini listeler.\n\t\
help(X)                       X fonksiyonunun kullanimi hakkinda bilgi verir."

#define CMD_HELP_CLEAR		"\
clear                         Konsolu temizler."

#define CMD_HELP_LIST		"\
list                          Tanimlanmis tum matrislerin adlarini ve boyutlarini listeler."

#define CMD_HELP_PRINT		"\
print                         Son matrisi yazdirir.\n\t\
print(X)                      X matrisini yazdirir."

#define CMD_HELP_DEFINE		"\
define(X)                     Son matrisi X adiyla tanimlar.\n\t\
define(X, R, C, all, A)       Tum elemanlari A olan, R satirli, C sutunlu bir X matrisi tanimlar.\n\t\
define(X, S, diag, A)         Kosegen elemanlari A olan, S satirli bir X matrisi tanimlar.\n\t\
define(X, S, low, A)          Elemanlari A olan, S satirli bir X alt ucgen matrisi tanimlar.\n\t\
define(X, S, up, A)           Elemanlari A olan, S satirli bir X ust ucgen matrisi tanimlar."

#define CMD_HELP_DELETE		"\
delete(X)                     Tanimlanmis X matrisini siler.\n\t\
delete(all)                   Tanimlanmis tum matrisleri siler."

#define CMD_HELP_GET		"\
get(R, C)                     Son matrisin R'inci satir C'inci sutunundaki degeri alir.\n\t\
get(X, R, C)                  X matrisinin R'inci satir C'inci sutunundaki degeri alir."

#define CMD_HELP_SET		"\
set(R, C, A)                  Son matrisin R'inci satir C'inci sutunundaki degerini A'ya esitler.\n\t\
set(X, R, C, A)               X matrisinin R'inci satir C'inci sutunundaki degerini A'ya esitler."

#define CMD_HELP_ISEQUAL	"\
isequal(X)                    Son matris ile X matrisinin esit olup olmadigini yazar.\n\t\
isequal(X, Y)                 X matrisi ile Y matrisinin esit olup olmadigini yazar."

#define CMD_HELP_TRANSPOSE	"\
transpose                     Son matrisin transpozunu alir.\n\t\
transpose(X)                  X matrisinin transpozunu alir."

#define CMD_HELP_ADD		"\
add(X)                        Son matris ile X matrisini toplar.\n\t\
add(X, Y)                     X matrisi ile Y matrisini toplar."

#define CMD_HELP_ROWOP		"\
rowop                         Son matrisi RREF matrisi yapmak icin siradaki elementer satir islemini uygular.\n\t\
rowop(X)                      X matrisini RREF matrisi yapmak icin siradaki elementer satir islemini uygular.\n\t\
rowop(switch, R1, R2)         Son matrisin R1'inci satiriyla R2'inci satirini degistir.\n\t\
rowop(X, switch, R1, R2)      X matrisinin R1'inci satiriyla R2'inci satirini degistir.\n\t\
rowop(multiply, R, K)         Son matrisin R'inci satirinin C katini al.\n\t\
rowop(X, multiply, R, K)      X matrisinin R'inci satirinin C katini al.\n\t\
rowop(add, R1, K, R2)         Son matrisin R1'inci satirinin C katini R2'inci satira ekle.\n\t\
rowop(X, add, R1, K, R2)      X matrisinin R1'inci satirinin C katini R2'inci satira ekle."

#define CMD_HELP_COLOP		"\
colop                         Son matrisi RCEF matrisi yapmak icin siradaki elementer sutun islemini uygular.\n\t\
colop(X)                      X matrisini RCEF matrisi yapmak icin siradaki elementer sutun islemini uygular.\n\t\
colop(switch, C1, C2)         Son matrisin C1'inci sutunuyla C2'inci sutununu degistir.\n\t\
colop(X, switch, C1, C2)      X matrisinin C1'inci sutunuyla C2'inci sutununu degistir.\n\t\
colop(multiply, C, K)         Son matrisin C'inci sutununun K katini al.\n\t\
colop(X, multiply, C, K)      X matrisinin C'inci sutununun K katini al.\n\t\
colop(add, C1, K, C2)         Son matrisin C1'inci sutununun K katini C2'inci sutuna ekle.\n\t\
colop(X, add, C1, K, C2)      X matrisinin C1'inci sutununun K katini C2'inci sutuna ekle."

struct Parsed
{
	char* name;
	uint8_t argcount;
	char* args[MAX_PARAM_COUNT];
};

struct Command
{
	char* name; // Küçük harflerden ve alt çizgi karakterinden oluþmalýdýr.
	char* help;
	void (*function)(CMD_PARAMS);
};

void parse_command(char* input, Parsed* parsed);
void get_one_mx(CMD_PARAMS, Matrix** matrix);
void get_two_mx(CMD_PARAMS, Matrix** matrix1, Matrix** matrix2);
void apply_op(CMD_PARAMS, uint8_t colmode);

void cmd_help(CMD_PARAMS);
void cmd_clear(CMD_PARAMS);
void cmd_list(CMD_PARAMS);
void cmd_print(CMD_PARAMS);
void cmd_define(CMD_PARAMS);
void cmd_delete(CMD_PARAMS);
void cmd_get(CMD_PARAMS);
void cmd_set(CMD_PARAMS);
void cmd_isequal(CMD_PARAMS);
void cmd_transpose(CMD_PARAMS);
void cmd_add(CMD_PARAMS);
void cmd_rowop(CMD_PARAMS);
void cmd_colop(CMD_PARAMS);