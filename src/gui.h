#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IGUIElement* CALL csCreateButton(int x, int y, int width, int height, const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateCheckBox(int x, int y, int width, int height, const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateComboBox(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateContextMenu(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateEditBox(int x, int y, int width, int height, int type, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateImage(int x, int y, int width, int height, ITexture* texture, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateListBox(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateMenu(const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateTabbar(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateText(int x, int y, int width, int height, const char* text, bool_t border, IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateToolbar(IGUIElement* parent);
EXPORT IGUIElement* CALL csCreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement* parent);
EXPORT void CALL csFreeControl(IGUIElement* control);
EXPORT int CALL csControlType(IGUIElement* control);
EXPORT void CALL csSetControlEnabled(IGUIElement* control, bool_t enabled);
EXPORT bool_t CALL csControlEnabled(IGUIElement* control);
EXPORT void CALL csSetControlVisible(IGUIElement* control, bool_t visible);
EXPORT bool_t CALL csControlVisible(IGUIElement* control);
EXPORT IGUIElement* CALL csControlParent(IGUIElement* control);
EXPORT int CALL csControlNumChildren(IGUIElement* control);
EXPORT IGUIElement* CALL csControlChild(IGUIElement* control, int index);
EXPORT void CALL csSetControlShape(IGUIElement* control, int x, int y, int width, int height);
EXPORT int CALL csControlX(IGUIElement* control);
EXPORT int CALL csControlY(IGUIElement* control);
EXPORT int CALL csControlWidth(IGUIElement* control);
EXPORT int CALL csControlHeight(IGUIElement* control);
EXPORT void CALL csSetControlText(IGUIElement* control, const char* text);
EXPORT const char* CALL csControlText(IGUIElement* control);
EXPORT void CALL csSetControlTooltip(IGUIElement* control, const char* tooltip);
EXPORT const char* CALL csControlTooltip(IGUIElement* control);
EXPORT void CALL csSetControlTexture(IGUIElement* control, ITexture* texture, bool_t alpha);
EXPORT void CALL csSetControlFont(IGUIElement* control, IGUIFont* font);
EXPORT IGUIFont* CALL csControlFont(IGUIElement* control);
EXPORT void CALL csSetControlColor(IGUIElement* control, int color);
EXPORT void CALL csClearControlColor(IGUIElement* control);
EXPORT int CALL csControlColor(IGUIElement* control);
EXPORT void CALL csSetControlChecked(IGUIElement* control, bool_t checked);
EXPORT bool_t CALL csControlChecked(IGUIElement* control);
EXPORT void CALL csSetControlValue(IGUIElement* control, int value);
EXPORT int CALL csControlValue(IGUIElement* control);
EXPORT int CALL csAddControlItem(IGUIElement* control, const char* text, int id);
EXPORT int CALL csControlNumItems(IGUIElement* control);
EXPORT void CALL csSetControlItemEnabled(IGUIElement* control, int index, bool_t enable);
EXPORT bool_t CALL csControlItemEnabled(IGUIElement* control, int index);
EXPORT void CALL csSetControlItemChecked(IGUIElement* control, int index, bool_t check);
EXPORT bool_t CALL csControlItemChecked(IGUIElement* control, int index);
EXPORT void CALL csSelectControlItem(IGUIElement* control, int index);
EXPORT int CALL csSelectedControlItem(IGUIElement* control);
EXPORT void CALL csSetControlItemText(IGUIElement* control, int index, const char* text);
EXPORT const char* CALL csControlItemText(IGUIElement* control, int index);
EXPORT void CALL csRemoveControlItem(IGUIElement* control, int index);
EXPORT void CALL csRemoveControlItems(IGUIElement* control);
EXPORT void CALL csAddMenuSeparator(IGUIElement* menu);
EXPORT void CALL csDrawGUI();
EXPORT bool_t CALL csPrepareNextGUIEvent();
EXPORT int CALL csGUIEventType();
EXPORT IGUIElement* CALL csGUIEventControl();


void _csPostEvent(int type, IGUIElement* control);


#ifdef __cplusplus
} /* extern "C" */
#endif
