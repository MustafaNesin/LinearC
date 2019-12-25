#pragma once

#pragma region Include
#include "Memory.h"
#pragma endregion

void      clear          (void);
int       readl          (char buf[], int n);
void      scanl          (char const* const format, ...);
int       get_char       (void);
float     froundf        (float value);
float     deg2rad        (float degree);
float     rad2deg        (float radian);
bool      iszero         (float value);