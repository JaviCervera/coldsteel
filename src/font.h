#pragma once

#include "common.h"

IGUIFont *LoadFont(const char *filename);
void FreeFont(IGUIFont *font);
void DrawText(IGUIFont *font, const char *text, int x, int y, int color);
int TextWidth(IGUIFont *font, const char *text);
int TextHeight(IGUIFont *font, const char *text);
