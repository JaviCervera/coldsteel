#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IGUIFont* CALL csLoadFont(const char* filename);
EXPORT void CALL csFreeFont(IGUIFont* font);
EXPORT void CALL csDrawText(IGUIFont* font, const char* text, int x, int y, int color);
EXPORT int CALL csTextWidth(IGUIFont* font, const char* text);
EXPORT int CALL csTextHeight(IGUIFont* font, const char* text);


#ifdef __cplusplus
} /* extern "C" */
#endif
