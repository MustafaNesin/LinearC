#pragma once
#include "Memory.h"
#define CMD_CLEAR 2
#define CMD_COUNT 5	// Dikkat! Her yeni komut eklenildiğinde bu sayı artırılmalıdır.

void cmd_help(Memory* memory, char* args);
void cmd_clear(Memory* memory, char* args);
void cmd_list(Memory* memory, char* args);
void cmd_print(Memory* memory, char* args);