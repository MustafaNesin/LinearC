#pragma once
#include "Memory.h"
#define CMD_COUNT 10	// Dikkat! Her yeni komut eklenildiðinde bu sayý artýrýlmalýdýr.

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
define(X, all, A, R, C)       Tum elemanlari A olan, R satirli, C sutunlu bir X matrisi tanimlar.\n\t\
define(X, diag, A, S)         Kosegen elemanlari A olan, S satirli bir X matrisi tanimlar.\n\t\
define(X, low, A, S)          Elemanlari A olan, S satirli bir X alt ucgen matrisi tanimlar.\n\t\
define(X, up, A, S)           Elemanlari A olan, S satirli bir X ust ucgen matrisi tanimlar."

#define CMD_HELP_DELETE		"\
delete(X)                     Tanimlanmis X matrisini siler.\n\t\
delete(all)                   Tanimlanmis tum matrisleri siler."

#define CMD_HELP_EQUAL		"\
equal(X)                      Son matris ile X matrisinin esit olup olmadigini yazar.\n\t\
equal(X, Y)                   X matrisi ile Y matrisinin esit olup olmadigini yazar."

#define CMD_HELP_TRANSPOSE	"\
transpose                     Son matrisin transpozunu alir.\n\t\
transpose(X)                  X matrisinin transpozunu alir."

#define CMD_HELP_ADD		"\
add(X)                        Son matris ile X matrisini toplar.\n\t\
add(X, Y)                     X matrisi ile Y matrisini toplar."


void parse_command(char* input, Parsed* parsed);
void get_one_mx(CMD_PARAMS, Matrix** matrix);
void get_two_mx(CMD_PARAMS, Matrix** matrix1, Matrix** matrix2);

void cmd_help(CMD_PARAMS);
void cmd_clear(CMD_PARAMS);
void cmd_list(CMD_PARAMS);
void cmd_print(CMD_PARAMS);
void cmd_define(CMD_PARAMS);
void cmd_delete(CMD_PARAMS);
void cmd_equal(CMD_PARAMS);
void cmd_transpose(CMD_PARAMS);
void cmd_add(CMD_PARAMS);