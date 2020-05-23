#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IGUIFont* CALL asLoadFont(const char* filename);
EXPORT void CALL asFreeFont(IGUIFont* font);
EXPORT void CALL asDrawText(IGUIFont* font, const char* text, int x, int y, int color);
EXPORT int CALL asTextWidth(IGUIFont* font, const char* text);
EXPORT int CALL asTextHeight(IGUIFont* font, const char* text);


#ifdef __cplusplus
} /* extern "C" */
#endif
