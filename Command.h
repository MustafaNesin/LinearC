#pragma once
#include "Memory.h"
#define CMD_CLEAR 2
#define CMD_COUNT 5	// Dikkat! Her yeni komut eklenildi�inde bu say� art�r�lmal�d�r.

void cmd_help(Memory* memory, char* args);
void cmd_clear(Memory* memory, char* args);
void cmd_list(Memory* memory, char* args);
void cmd_print(Memory* memory, char* args);