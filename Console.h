#pragma once
#include "Memory.h"
#define CMD_COUNT 8	// Dikkat! Her yeni komut eklenildiðinde bu sayý artýrýlmalýdýr.

#define CMD_FUNC_PARAMS_FAIL \
{ \
	printf("Parametreler uyusmuyor.\nYardim: help(%s)\n\n", parsed->name); \
	return; \
}

#define CMD_HELP_RETURN		"\
return                        Ana menuye geri dondurur."

#define CMD_HELP_HELP		"\
help                          Tum fonksiyonlarin kullanim bilgisini listeler.\n\
help(X)                       X fonksiyonunun kullanimi hakkinda bilgi verir."

#define CMD_HELP_CLEAR		"\
clear                         Konsolu temizler."

#define CMD_HELP_LIST		"\
list                          Tanimlanmis tum matrislerin adlarini ve boyutlarini listeler."

#define CMD_HELP_PRINT		"\
print                         Son matrisi yazdirir.\n\
print(X)                      X matrisini yazdirir."

#define CMD_HELP_DEFINE		"\
define(X)                     Son matrisi X adiyla tanimlar.\n\
define(X, all, A, R, C)       Tum elemanlari A olan, R satirli, C sutunlu bir X matrisi tanimlar.\n\
define(X, diag, A, S)         Kosegen elemanlari A olan, S satirli bir X matrisi tanimlar.\n\
define(X, low, A, S)          Elemanlari A olan, S satirli bir X alt ucgen matrisi tanimlar.\n\
define(X, up, A, S)           Elemanlari A olan, S satirli bir X ust ucgen matrisi tanimlar."

#define CMD_HELP_DELETE		"\
delete(X)                     Tanimlanmis X matrisini siler.\n\
delete(all)                   Tanimlanmis tum matrisleri siler."

#define CMD_HELP_TRANSPOSE	"\
transpose                     Son matrisin transpozunu alir.\n\
transpose(X)                  X matrisinin transpozunu alir."


void parse_command(char* input, Parsed* parsed);
Matrix* test_command(CMD_FUNC_PARAMS);

void cmd_help(CMD_FUNC_PARAMS);
void cmd_clear(CMD_FUNC_PARAMS);
void cmd_list(CMD_FUNC_PARAMS);
void cmd_print(CMD_FUNC_PARAMS);
void cmd_define(CMD_FUNC_PARAMS);
void cmd_delete(CMD_FUNC_PARAMS);
void cmd_transpose(CMD_FUNC_PARAMS);