#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IGUIElement* CALL asCreateButton(int x, int y, int width, int height, const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateCheckBox(int x, int y, int width, int height, const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateComboBox(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateContextMenu(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateEditBox(int x, int y, int width, int height, int type, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateImage(int x, int y, int width, int height, ITexture* texture, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateListBox(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateMenu(const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateTabbar(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateText(int x, int y, int width, int height, const char* text, bool_t border, IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateToolbar(IGUIElement* parent);
EXPORT IGUIElement* CALL asCreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement* parent);
EXPORT void CALL asFreeControl(IGUIElement* control);
EXPORT int CALL asControlType(IGUIElement* control);
EXPORT void CALL asSetControlEnabled(IGUIElement* control, bool_t enabled);
EXPORT bool_t CALL asControlEnabled(IGUIElement* control);
EXPORT void CALL asSetControlVisible(IGUIElement* control, bool_t visible);
EXPORT bool_t CALL asControlVisible(IGUIElement* control);
EXPORT IGUIElement* CALL asControlParent(IGUIElement* control);
EXPORT int CALL asControlNumChildren(IGUIElement* control);
EXPORT IGUIElement* CALL asControlChild(IGUIElement* control, int index);
EXPORT void CALL asSetControlShape(IGUIElement* control, int x, int y, int width, int height);
EXPORT int CALL asControlX(IGUIElement* control);
EXPORT int CALL asControlY(IGUIElement* control);
EXPORT int CALL asControlWidth(IGUIElement* control);
EXPORT int CALL asControlHeight(IGUIElement* control);
EXPORT void CALL asSetControlText(IGUIElement* control, const char* text);
EXPORT const char* CALL asControlText(IGUIElement* control);
EXPORT void CALL asSetControlTooltip(IGUIElement* control, const char* tooltip);
EXPORT const char* CALL asControlTooltip(IGUIElement* control);
EXPORT void CALL asSetControlTexture(IGUIElement* control, ITexture* texture, bool_t alpha);
EXPORT void CALL asSetControlFont(IGUIElement* control, IGUIFont* font);
EXPORT IGUIFont* CALL asControlFont(IGUIElement* control);
EXPORT void CALL asSetControlColor(IGUIElement* control, int color);
EXPORT void CALL asClearControlColor(IGUIElement* control);
EXPORT int CALL asControlColor(IGUIElement* control);
EXPORT void CALL asSetControlChecked(IGUIElement* control, bool_t checked);
EXPORT bool_t CALL asControlChecked(IGUIElement* control);
EXPORT void CALL asSetControlValue(IGUIElement* control, int value);
EXPORT int CALL asControlValue(IGUIElement* control);
EXPORT int CALL asAddControlItem(IGUIElement* control, const char* text, int id);
EXPORT int CALL asControlNumItems(IGUIElement* control);
EXPORT void CALL asSetControlItemEnabled(IGUIElement* control, int index, bool_t enable);
EXPORT bool_t CALL asControlItemEnabled(IGUIElement* control, int index);
EXPORT void CALL asSetControlItemChecked(IGUIElement* control, int index, bool_t check);
EXPORT bool_t CALL asControlItemChecked(IGUIElement* control, int index);
EXPORT void CALL asSelectControlItem(IGUIElement* control, int index);
EXPORT int CALL asSelectedControlItem(IGUIElement* control);
EXPORT void CALL asSetControlItemText(IGUIElement* control, int index, const char* text);
EXPORT const char* CALL asControlItemText(IGUIElement* control, int index);
EXPORT void CALL asRemoveControlItem(IGUIElement* control, int index);
EXPORT void CALL asRemoveControlItems(IGUIElement* control);
EXPORT void CALL asAddMenuSeparator(IGUIElement* menu);
EXPORT void CALL asDrawGUI();
EXPORT bool_t CALL asPrepareNextGUIEvent();
EXPORT int CALL asGUIEventType();
EXPORT IGUIElement* CALL asGUIEventControl();


void _asPostEvent(int type, IGUIElement* control);


#ifdef __cplusplus
} /* extern "C" */
#endif
