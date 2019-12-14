#pragma once
#include "Memory.h"
#define CMD_COUNT 6	// Dikkat! Her yeni komut eklenildiðinde bu sayý artýrýlmalýdýr.
#define CMD_FUNC_PARAM_COUNT_FAIL { printf("Parametre sayisi uyusmuyor.\nYardim: help(%s)\n", parsed->name); return; }

void parse_command(char* input, Parsed* parsed);

void cmd_help(CMD_FUNC_PARAMS);
void cmd_clear(CMD_FUNC_PARAMS);
void cmd_list(CMD_FUNC_PARAMS);
void cmd_print(CMD_FUNC_PARAMS);
void cmd_assign(CMD_FUNC_PARAMS);