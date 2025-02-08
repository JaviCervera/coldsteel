#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C"
{
#endif

    EXPORT IGUIFont *CALL LoadFont(const char *filename);
    EXPORT void CALL FreeFont(IGUIFont *font);
    EXPORT void CALL DrawText(IGUIFont *font, const char *text, int x, int y, int color);
    EXPORT int CALL TextWidth(IGUIFont *font, const char *text);
    EXPORT int CALL TextHeight(IGUIFont *font, const char *text);

#ifdef __cplusplus
} /* extern "C" */
#endif
