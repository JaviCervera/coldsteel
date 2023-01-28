#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IGUIElement* CALL CreateButton(int x, int y, int width, int height, const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateCheckBox(int x, int y, int width, int height, const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateComboBox(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateContextMenu(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateEditBox(int x, int y, int width, int height, int type, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateImage(int x, int y, int width, int height, ITexture* texture, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateListBox(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateMenu(const char* text, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateTabbar(int x, int y, int width, int height, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateText(int x, int y, int width, int height, const char* text, bool_t border, IGUIElement* parent);
EXPORT IGUIElement* CALL CreateToolbar(IGUIElement* parent);
EXPORT IGUIElement* CALL CreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement* parent);
EXPORT void CALL FreeControl(IGUIElement* control);
EXPORT int CALL ControlType(IGUIElement* control);
EXPORT void CALL SetControlEnabled(IGUIElement* control, bool_t enabled);
EXPORT bool_t CALL ControlEnabled(IGUIElement* control);
EXPORT void CALL SetControlVisible(IGUIElement* control, bool_t visible);
EXPORT bool_t CALL ControlVisible(IGUIElement* control);
EXPORT IGUIElement* CALL ControlParent(IGUIElement* control);
EXPORT int CALL ControlNumChildren(IGUIElement* control);
EXPORT IGUIElement* CALL ControlChild(IGUIElement* control, int index);
EXPORT void CALL SetControlShape(IGUIElement* control, int x, int y, int width, int height);
EXPORT int CALL ControlX(IGUIElement* control);
EXPORT int CALL ControlY(IGUIElement* control);
EXPORT int CALL ControlWidth(IGUIElement* control);
EXPORT int CALL ControlHeight(IGUIElement* control);
EXPORT void CALL SetControlText(IGUIElement* control, const char* text);
EXPORT const char* CALL ControlText(IGUIElement* control);
EXPORT void CALL SetControlTooltip(IGUIElement* control, const char* tooltip);
EXPORT const char* CALL ControlTooltip(IGUIElement* control);
EXPORT void CALL SetControlTexture(IGUIElement* control, ITexture* texture, bool_t alpha);
EXPORT void CALL SetControlFont(IGUIElement* control, IGUIFont* font);
EXPORT IGUIFont* CALL ControlFont(IGUIElement* control);
EXPORT void CALL SetControlColor(IGUIElement* control, int color);
EXPORT void CALL ClearControlColor(IGUIElement* control);
EXPORT int CALL ControlColor(IGUIElement* control);
EXPORT void CALL SetControlChecked(IGUIElement* control, bool_t checked);
EXPORT bool_t CALL ControlChecked(IGUIElement* control);
EXPORT void CALL SetControlValue(IGUIElement* control, int value);
EXPORT int CALL ControlValue(IGUIElement* control);
EXPORT int CALL ControlId(IGUIElement* control);
EXPORT int CALL AddControlItem(IGUIElement* control, const char* text, int id);
EXPORT int CALL ControlNumItems(IGUIElement* control);
EXPORT void CALL SetControlItemEnabled(IGUIElement* control, int index, bool_t enable);
EXPORT bool_t CALL ControlItemEnabled(IGUIElement* control, int index);
EXPORT void CALL SetControlItemChecked(IGUIElement* control, int index, bool_t check);
EXPORT bool_t CALL ControlItemChecked(IGUIElement* control, int index);
EXPORT void CALL SelectControlItem(IGUIElement* control, int index);
EXPORT int CALL SelectedControlItem(IGUIElement* control);
EXPORT void CALL SetControlItemText(IGUIElement* control, int index, const char* text);
EXPORT const char* CALL ControlItemText(IGUIElement* control, int index);
EXPORT void CALL RemoveControlItem(IGUIElement* control, int index);
EXPORT void CALL RemoveControlItems(IGUIElement* control);
EXPORT void CALL AddMenuSeparator(IGUIElement* menu);
EXPORT void CALL DrawGUI();
EXPORT bool_t CALL PrepareNextGUIEvent();
EXPORT int CALL GUIEventType();
EXPORT IGUIElement* CALL GUIEventControl();
EXPORT int CALL GUIEventMenuId();


void _PostEvent(int type, IGUIElement* control, int menuId);


#ifdef __cplusplus
} /* extern "C" */
#endif
