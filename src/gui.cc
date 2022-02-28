#include "core.h"
#include "graphics.h"
#include "gui.h"

extern "C" {


struct Event {
    int type;
    IGUIElement* control;
    int menuId;
    Event(int type, IGUIElement* control, int menuId) : type(type), control(control), menuId(menuId) {}
};


static array<Event> _events;
static Event _currentEvent(0, NULL, -1);


IGUIEnvironment* _asGUI();
IGUIElement* _asResolveParent(IGUIElement* parent);


EXPORT IGUIElement* CALL CreateButton(int x, int y, int width, int height, const char* text, IGUIElement* parent) {
    return _asGUI()->addButton(recti(x, y, x+width, y+height), _asResolveParent(parent), -1, _WSTR(text));
}


EXPORT IGUIElement* CALL CreateCheckBox(int x, int y, int width, int height, const char* text, IGUIElement* parent) {
    return _asGUI()->addCheckBox(false, recti(x, y, x+width, y+height), _asResolveParent(parent), -1, _WSTR(text));
}


EXPORT IGUIElement* CALL CreateComboBox(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addComboBox(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL CreateContextMenu(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addContextMenu(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL CreateEditBox(int x, int y, int width, int height, int type, IGUIElement* parent) {
    IGUIEditBox* box = _asGUI()->addEditBox(L"", recti(x, y, x+width, y+height), true, _asResolveParent(parent));
    if (type == EDITBOX_PASSWORD) box->setPasswordBox(true, L'*');
    else if (type == EDITBOX_MULTILINE) box->setMultiLine(true);
    return box;
}


EXPORT IGUIElement* CALL CreateImage(int x, int y, int width, int height, ITexture* texture, IGUIElement* parent) {
    IGUIImage* image = _asGUI()->addImage(recti(x, y, x+width, y+height), _asResolveParent(parent));
    image->setImage(texture);
    image->setScaleImage(true);
    return image;
}


EXPORT IGUIElement* CALL CreateListBox(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addListBox(recti(x, y, x+width, y+height), _asResolveParent(parent), -1, true);
}


EXPORT IGUIElement* CALL CreateMenu(const char* text, IGUIElement* parent) {
    parent = _asResolveParent(parent);
    IGUIContextMenu* menu = (parent->getType() == EGUIET_CONTEXT_MENU || parent->getType() == EGUIET_MENU)
        ? ((IGUIContextMenu*)parent)->getSubMenu(((IGUIContextMenu*)parent)->addItem(_WSTR(text), -1, true, true))
        : _asGUI()->addMenu((parent->getType() == EGUIET_WINDOW) ? parent : NULL);
    menu->setText(_WSTR(text));
    return menu;
}


EXPORT IGUIElement* CALL CreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement* parent) {
    IGUIScrollBar* scrollbar = _asGUI()->addScrollBar(horizontal, recti(x, y, x+width, y+height), _asResolveParent(parent));
    scrollbar->setMax(max);
    scrollbar->setSmallStep(step);
    return scrollbar;
}


EXPORT IGUIElement* CALL CreateTabbar(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addTabControl(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL CreateText(int x, int y, int width, int height, const char* text, bool_t border, IGUIElement* parent) {
    return _asGUI()->addStaticText(_WSTR(text), recti(x, y, x+width, y+height), border, true, _asResolveParent(parent));
}


EXPORT IGUIElement* CALL CreateToolbar(IGUIElement* parent) {
    return _asGUI()->addToolBar(_asResolveParent(parent));
}


EXPORT IGUIElement* CALL CreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement* parent) {
    return _asGUI()->addWindow(recti(x, y, x+width, y+height), modal, NULL, _asResolveParent(parent));
}


EXPORT void CALL FreeControl(IGUIElement* control) {
    control->remove();
}


EXPORT int CALL ControlType(IGUIElement* control) {
    map<EGUI_ELEMENT_TYPE, int> types;
    types.set(EGUIET_BUTTON, CONTROL_BUTTON);
    types.set(EGUIET_CHECK_BOX, CONTROL_CHECKBOX);
    types.set(EGUIET_COMBO_BOX, CONTROL_COMBOBOX);
    types.set(EGUIET_EDIT_BOX, CONTROL_EDITBOX);
    types.set(EGUIET_IMAGE, CONTROL_IMAGE);
    types.set(EGUIET_LIST_BOX, CONTROL_LISTBOX);
    types.set(EGUIET_CONTEXT_MENU, CONTROL_MENU);
    types.set(EGUIET_MENU, CONTROL_MENU);
    types.set(EGUIET_SCROLL_BAR, CONTROL_SCROLLBAR);
    types.set(EGUIET_TAB_CONTROL, CONTROL_TABBAR);
    types.set(EGUIET_STATIC_TEXT, CONTROL_TEXT);
    types.set(EGUIET_TOOL_BAR, CONTROL_TOOLBAR);
    types.set(EGUIET_WINDOW, CONTROL_WINDOW);
    return types[control->getType()];
}


EXPORT void CALL SetControlEnabled(IGUIElement* control, bool_t enabled) {
    control->setEnabled(enabled);
}


EXPORT bool_t CALL ControlEnabled(IGUIElement* control) {
    return control->isEnabled();
}


EXPORT void CALL SetControlVisible(IGUIElement* control, bool_t visible) {
    control->setVisible(visible);
}


EXPORT bool_t CALL ControlVisible(IGUIElement* control) {
    return control->isVisible();
}


EXPORT IGUIElement* CALL ControlParent(IGUIElement* control) {
    return control->getParent();
}


EXPORT int CALL ControlNumChildren(IGUIElement* control) {
    return control->getChildren().size() - ((ControlType(control) == CONTROL_TABBAR) ? 2 : 0);
}


EXPORT IGUIElement* CALL ControlChild(IGUIElement* control, int index) {
    return *(control->getChildren().begin() + index);
}


EXPORT void CALL SetControlShape(IGUIElement* control, int x, int y, int width, int height) {
    control->setRelativePosition(recti(x, y, x+width, y+height));
}


EXPORT int CALL ControlX(IGUIElement* control) {
    return control->getRelativePosition().UpperLeftCorner.X;
}


EXPORT int CALL ControlY(IGUIElement* control) {
    return control->getRelativePosition().UpperLeftCorner.Y;
}


EXPORT int CALL ControlWidth(IGUIElement* control) {
    return control->getRelativePosition().getWidth();
}


EXPORT int CALL ControlHeight(IGUIElement* control) {
    return control->getRelativePosition().getHeight();
}


EXPORT void CALL SetControlText(IGUIElement* control, const char* text) {
    control->setText(_WSTR(text));
}


EXPORT const char* CALL ControlText(IGUIElement* control) {
    return _CSTR(control->getText());
}


EXPORT void CALL SetControlTooltip(IGUIElement* control, const char* tooltip) {
    control->setToolTipText(_WSTR(tooltip));
}


EXPORT const char* CALL ControlTooltip(IGUIElement* control) {
    return _CSTR(control->getToolTipText().c_str());
}


EXPORT void CALL SetControlTexture(IGUIElement* control, ITexture* texture, bool_t alpha) {
    switch (ControlType(control)) {
        case CONTROL_BUTTON:
            ((IGUIButton*)control)->setImage(texture);
            ((IGUIButton*)control)->setUseAlphaChannel(alpha);
            break;
        case CONTROL_IMAGE:
            ((IGUIImage*)control)->setImage(texture);
            ((IGUIImage*)control)->setUseAlphaChannel(alpha);
            break;
    }
}


EXPORT void CALL SetControlFont(IGUIElement* control, IGUIFont* font) {
    if (control != NULL) {
        switch (ControlType(control)) {
            case CONTROL_BUTTON:
                ((IGUIButton*)control)->setOverrideFont(font);
                break;
            case CONTROL_EDITBOX:
                ((IGUIEditBox*)control)->setOverrideFont(font);
                break;
            case CONTROL_TEXT:
                ((IGUIStaticText*)control)->setOverrideFont(font);
                break;
        }
    } else {
        _asGUI()->getSkin()->setFont(font);
    }
}


EXPORT IGUIFont* CALL ControlFont(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_BUTTON:
            return ((IGUIButton*)control)->getOverrideFont();
        case CONTROL_EDITBOX:
            ((IGUIEditBox*)control)->getOverrideFont();
        case CONTROL_TEXT:
            ((IGUIStaticText*)control)->getOverrideFont();
        default:
            return NULL;
    }
}


EXPORT void CALL SetControlColor(IGUIElement* control, int color) {
    switch (ControlType(control)) {
        case CONTROL_EDITBOX:
            ((IGUIEditBox*)control)->setOverrideColor(_Color(color));
            break;
        case CONTROL_TEXT:
            ((IGUIStaticText*)control)->setOverrideColor(_Color(color));
    }
}


EXPORT void CALL ClearControlColor(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_EDITBOX:
            ((IGUIEditBox*)control)->enableOverrideColor(false);
            break;
        case CONTROL_TEXT:
            ((IGUIStaticText*)control)->enableOverrideColor(false);
    }
}


EXPORT int CALL ControlColor(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_EDITBOX:
            return _IntColor(((IGUIEditBox*)control)->getOverrideColor());
        case CONTROL_TEXT:
            return _IntColor(((IGUIStaticText*)control)->getOverrideColor());
        default:
            return RGB(0, 0, 0);
    }
}


EXPORT void CALL SetControlChecked(IGUIElement* control, bool_t checked) {
    switch (ControlType(control)) {
        case CONTROL_BUTTON:
            ((IGUIButton*)control)->setPressed(checked);
            break;
        case CONTROL_CHECKBOX:
            ((IGUICheckBox*)control)->setChecked(checked);
            break;
    }
}


EXPORT bool_t CALL ControlChecked(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_BUTTON:
            return ((IGUIButton*)control)->isPressed();
        case CONTROL_CHECKBOX:
            return ((IGUICheckBox*)control)->isChecked();
    }
    return false;
}


EXPORT void CALL SetControlValue(IGUIElement* control, int value) {
    if (ControlType(control) == CONTROL_SCROLLBAR) ((IGUIScrollBar*)control)->setPos(value);
}


EXPORT int CALL ControlValue(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_SCROLLBAR:
            return ((IGUIScrollBar*)control)->getPos();
        case CONTROL_TABBAR:
            return ((IGUITabControl*)control)->getActiveTab();
    }
    return 0;
}


EXPORT int CALL AddControlItem(IGUIElement* control, const char* text, int id) {
    switch (ControlType(control)) {
        case CONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->addItem(_WSTR(text));
        case CONTROL_MENU:
            return ((IGUIContextMenu*)control)->addItem(_WSTR(text), id);
        case CONTROL_LISTBOX:
            return ((IGUIListBox*)control)->addItem(_WSTR(text));
        case CONTROL_TABBAR:
            ((IGUITabControl*)control)->addTab(_WSTR(text), id);
            return ((IGUITabControl*)control)->getTabCount() - 1;
        case CONTROL_TOOLBAR: {
            IGUIButton* button = ((IGUIToolBar*)control)->addButton(id, _WSTR(text));
            button->setDrawBorder(false);
            return ControlNumChildren(control) - 1;
        }
    }
    return -1;
}


EXPORT int CALL ControlNumItems(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->getItemCount();
        case CONTROL_MENU:
            return ((IGUIContextMenu*)control)->getItemCount();
        case CONTROL_LISTBOX:
            return ((IGUIListBox*)control)->getItemCount();
        case CONTROL_TABBAR:
            return ((IGUITabControl*)control)->getTabCount();
        case CONTROL_TOOLBAR:
            return ControlNumChildren(control);
    }
    return 0;
}


EXPORT void CALL SetControlItemEnabled(IGUIElement* control, int index, bool_t enable) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemEnabled(index, enable);
            break;
    }
}


EXPORT bool_t CALL ControlItemEnabled(IGUIElement* control, int index) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            return ((IGUIContextMenu*)control)->isItemEnabled(index);
    }
    return false;
}


EXPORT void CALL SetControlItemChecked(IGUIElement* control, int index, bool_t check) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemChecked(index, check);
            break;
        case CONTROL_TOOLBAR:
            ((IGUIButton*)ControlChild(control, index))->setPressed(check);
            break;
    }
}


EXPORT bool_t CALL ControlItemChecked(IGUIElement* control, int index) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            return ((IGUIContextMenu*)control)->isItemChecked(index);
        case CONTROL_TOOLBAR:
            return ((IGUIButton*)ControlChild(control, index))->isPressed();
    }
    return false;
}


EXPORT void CALL SelectControlItem(IGUIElement* control, int index) {
    switch (ControlType(control)) {
        case CONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->setSelected(index);
            break;
        case CONTROL_LISTBOX:
            ((IGUIListBox*)control)->setSelected(index);
            break;
        case CONTROL_TABBAR:
            ((IGUITabControl*)control)->setActiveTab(index);
            break;
    }
}


EXPORT int CALL SelectedControlItem(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            return ((IGUIContextMenu*)control)->getSelectedItem();
        case CONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->getSelected();
        case CONTROL_LISTBOX:
            return ((IGUIListBox*)control)->getSelected();
        case CONTROL_TABBAR:
            return ((IGUITabControl*)control)->getActiveTab();
    }
    return -1;
}


EXPORT void CALL SetControlItemText(IGUIElement* control, int index, const char* text) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemText(index, _WSTR(text));
            break;
    }
}


EXPORT const char* CALL ControlItemText(IGUIElement* control, int index) {
    switch (ControlType(control)) {
        case CONTROL_MENU:
            return _CSTR(((IGUIContextMenu*)control)->getItemText(index));
    }
    return _CSTR(L"");
}


EXPORT void CALL RemoveControlItem(IGUIElement* control, int index) {
    switch (ControlType(control)) {
        case CONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->removeItem(index);
        case CONTROL_MENU:
            ((IGUIContextMenu*)control)->removeItem(index);
            break;
        case CONTROL_LISTBOX:
            ((IGUIListBox*)control)->removeItem(index);
            break;
        case CONTROL_TABBAR:
            ((IGUITabControl*)control)->removeTab(index);
            break;
    }
}


EXPORT void CALL RemoveControlItems(IGUIElement* control) {
    switch (ControlType(control)) {
        case CONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->clear();
        case CONTROL_MENU:
            ((IGUIContextMenu*)control)->removeAllItems();
            break;
        case CONTROL_LISTBOX:
            ((IGUIListBox*)control)->clear();
            break;
        case CONTROL_TABBAR:
            ((IGUITabControl*)control)->clear();
            break;
    }
}


EXPORT void CALL AddMenuSeparator(IGUIElement* menu) {
    if (ControlType(menu) == CONTROL_MENU) {
        ((IGUIContextMenu*)menu)->addSeparator();
    }
}


EXPORT void CALL DrawGUI() {
    _asGUI()->drawAll();
}


EXPORT bool_t CALL PrepareNextGUIEvent() {
    if (_events.size() > 0) {
        _currentEvent = _events[0];
        _events.erase(0);
        return true;
    } else {
        return false;
    }
}


EXPORT int CALL GUIEventType() {
    return _currentEvent.type;
}


EXPORT IGUIElement* CALL GUIEventControl() {
    return _currentEvent.control;
}


EXPORT int CALL GUIEventMenuId() {
    return _currentEvent.menuId;
}


IGUIEnvironment* _asGUI() {
    return _Device()->getGUIEnvironment();
}


IGUIElement* _asResolveParent(IGUIElement* parent) {
    return (parent != NULL) ? parent : _Device()->getGUIEnvironment()->getRootGUIElement();
}


void _PostEvent(int type, IGUIElement* control, int menuId) {
    _events.push_back(Event(type, control, menuId));
}


} // extern "C"
