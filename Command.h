#pragma once
#include "Memory.h"
#define CMD_COUNT 6	// Dikkat! Her yeni komut eklenildiðinde bu sayý artýrýlmalýdýr.

#define CMD_FUNC_PARAM_COUNT_FAIL \
{ \
	printf("Parametre sayisi uyusmuyor.\nYardim: help(%s)\n", parsed->name); \
	return; \
}

#define CMD_HELP_RETURN	"\
return      Ana menuye geri dondurur."

#define CMD_HELP_HELP	"\
help        Tum fonksiyonlarin kullanim bilgisini listeler.\n\
help(X)     X fonksiyonunun kullanimi hakkinda bilgi verir."

#define CMD_HELP_CLEAR	"\
clear       Konsolu temizler."

#define CMD_HELP_LIST	"\
list        Tanimlanmis tum matrislerin adlarini ve boyutlarini listeler."

#define CMD_HELP_PRINT	"\
print       Yazdirilarak bellege alinmis olan son matrisi yazdirir.\n\
print(X)    X matrisini yazdirir ve bellege alir."

#define CMD_HELP_ASSIGN	"\
assign(X)   Yazdirilarak bellege alinmis olan son matrisi yeni bir X degiskenine atar."


void parse_command(char* input, Parsed* parsed);

void cmd_help(CMD_FUNC_PARAMS);
void cmd_clear(CMD_FUNC_PARAMS);
void cmd_list(CMD_FUNC_PARAMS);
void cmd_print(CMD_FUNC_PARAMS);
void cmd_assign(CMD_FUNC_PARAMS);