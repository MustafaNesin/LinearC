#pragma once
#include "Memory.h"
#define CMD_COUNT 5	// Dikkat! Her yeni komut eklenildi�inde bu say� art�r�lmal�d�r.

void parse_command(char* input, ParsedCommand* parsed);

void cmd_help(Memory* memory, ParsedCommand* parsed);
void cmd_clear(Memory* memory, ParsedCommand* parsed);
void cmd_list(Memory* memory, ParsedCommand* parsed);
void cmd_print(Memory* memory, ParsedCommand* parsed);