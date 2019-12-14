#pragma once
#include "Memory.h"
#define CMD_CLEAR 2
#define CMD_COUNT 5	// Dikkat! Her yeni komut eklenildiğinde bu sayı artırılmalıdır.

void parse_command(char* input, ParsedCommand* parsed);

void cmd_help(Memory* memory, ParsedCommand* parsed);
void cmd_clear(Memory* memory, ParsedCommand* parsed);
void cmd_list(Memory* memory, ParsedCommand* parsed);
void cmd_print(Memory* memory, ParsedCommand* parsed);