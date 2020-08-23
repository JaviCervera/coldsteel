#include "core.h"
#include "graphics.h"
#include "gui.h"

extern "C" {


struct csEvent {
    int type;
    IGUIElement* control;
    csEvent(int type, IGUIElement* control) : type(type), control(control) {}
};


static array<csEvent> _events;
static csEvent _currentEvent(0, NULL);


IGUIEnvironment* _asGUI();
IGUIElement* _asResolveParent(IGUIElement* parent);


EXPORT IGUIElement* CALL csCreateButton(int x, int y, int width, int height, const char* text, IGUIElement* parent) {
    return _asGUI()->addButton(recti(x, y, x+width, y+height), _asResolveParent(parent), -1, _WSTR(text));
}


EXPORT IGUIElement* CALL csCreateCheckBox(int x, int y, int width, int height, const char* text, IGUIElement* parent) {
    return _asGUI()->addCheckBox(false, recti(x, y, x+width, y+height), _asResolveParent(parent), -1, _WSTR(text));
}


EXPORT IGUIElement* CALL csCreateComboBox(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addComboBox(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL csCreateContextMenu(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addContextMenu(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL csCreateEditBox(int x, int y, int width, int height, int type, IGUIElement* parent) {
    IGUIEditBox* box = _asGUI()->addEditBox(L"", recti(x, y, x+width, y+height), true, _asResolveParent(parent));
    if (type == CSEDITBOX_PASSWORD) box->setPasswordBox(true, L'*');
    else if (type == CSEDITBOX_MULTILINE) box->setMultiLine(true);
    return box;
}


EXPORT IGUIElement* CALL csCreateImage(int x, int y, int width, int height, ITexture* texture, IGUIElement* parent) {
    IGUIImage* image = _asGUI()->addImage(recti(x, y, x+width, y+height), _asResolveParent(parent));
    image->setImage(texture);
    image->setScaleImage(true);
    return image;
}


EXPORT IGUIElement* CALL csCreateListBox(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addListBox(recti(x, y, x+width, y+height), _asResolveParent(parent), -1, true);
}


EXPORT IGUIElement* CALL csCreateMenu(const char* text, IGUIElement* parent) {
    parent = _asResolveParent(parent);
    IGUIContextMenu* menu = (parent->getType() == EGUIET_CONTEXT_MENU || parent->getType() == EGUIET_MENU)
        ? ((IGUIContextMenu*)parent)->getSubMenu(((IGUIContextMenu*)parent)->addItem(_WSTR(text), -1, true, true))
        : _asGUI()->addMenu((parent->getType() == EGUIET_WINDOW) ? parent : NULL);
    menu->setText(_WSTR(text));
    return menu;
}


EXPORT IGUIElement* CALL csCreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement* parent) {
    IGUIScrollBar* scrollbar = _asGUI()->addScrollBar(horizontal, recti(x, y, x+width, y+height), _asResolveParent(parent));
    scrollbar->setMax(max);
    scrollbar->setSmallStep(step);
    return scrollbar;
}


EXPORT IGUIElement* CALL csCreateTabbar(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addTabControl(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL csCreateText(int x, int y, int width, int height, const char* text, bool_t border, IGUIElement* parent) {
    return _asGUI()->addStaticText(_WSTR(text), recti(x, y, x+width, y+height), border, true, _asResolveParent(parent));
}


EXPORT IGUIElement* CALL csCreateToolbar(IGUIElement* parent) {
    return _asGUI()->addToolBar(_asResolveParent(parent));
}


EXPORT IGUIElement* CALL csCreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement* parent) {
    return _asGUI()->addWindow(recti(x, y, x+width, y+height), modal, NULL, _asResolveParent(parent));
}


EXPORT void CALL csFreeControl(IGUIElement* control) {
    control->remove();
}


EXPORT int CALL csControlType(IGUIElement* control) {
    map<EGUI_ELEMENT_TYPE, int> types;
    types.set(EGUIET_BUTTON, CSCONTROL_BUTTON);
    types.set(EGUIET_CHECK_BOX, CSCONTROL_CHECKBOX);
    types.set(EGUIET_COMBO_BOX, CSCONTROL_COMBOBOX);
    types.set(EGUIET_EDIT_BOX, CSCONTROL_EDITBOX);
    types.set(EGUIET_IMAGE, CSCONTROL_IMAGE);
    types.set(EGUIET_LIST_BOX, CSCONTROL_LISTBOX);
    types.set(EGUIET_CONTEXT_MENU, CSCONTROL_MENU);
    types.set(EGUIET_MENU, CSCONTROL_MENU);
    types.set(EGUIET_SCROLL_BAR, CSCONTROL_SCROLLBAR);
    types.set(EGUIET_TAB_CONTROL, CSCONTROL_TABBAR);
    types.set(EGUIET_STATIC_TEXT, CSCONTROL_TEXT);
    types.set(EGUIET_TOOL_BAR, CSCONTROL_TOOLBAR);
    types.set(EGUIET_WINDOW, CSCONTROL_WINDOW);
    return types[control->getType()];
}


EXPORT void CALL csSetControlEnabled(IGUIElement* control, bool_t enabled) {
    control->setEnabled(enabled);
}


EXPORT bool_t CALL csControlEnabled(IGUIElement* control) {
    return control->isEnabled();
}


EXPORT void CALL csSetControlVisible(IGUIElement* control, bool_t visible) {
    control->setVisible(visible);
}


EXPORT bool_t CALL csControlVisible(IGUIElement* control) {
    return control->isVisible();
}


EXPORT IGUIElement* CALL csControlParent(IGUIElement* control) {
    return control->getParent();
}


EXPORT int CALL csControlNumChildren(IGUIElement* control) {
    return control->getChildren().size() - ((csControlType(control) == CSCONTROL_TABBAR) ? 2 : 0);
}


EXPORT IGUIElement* CALL csControlChild(IGUIElement* control, int index) {
    return *(control->getChildren().begin() + index);
}


EXPORT void CALL csSetControlShape(IGUIElement* control, int x, int y, int width, int height) {
    control->setRelativePosition(recti(x, y, x+width, y+height));
}


EXPORT int CALL csControlX(IGUIElement* control) {
    return control->getRelativePosition().UpperLeftCorner.X;
}


EXPORT int CALL csControlY(IGUIElement* control) {
    return control->getRelativePosition().UpperLeftCorner.Y;
}


EXPORT int CALL csControlWidth(IGUIElement* control) {
    return control->getRelativePosition().getWidth();
}


EXPORT int CALL csControlHeight(IGUIElement* control) {
    return control->getRelativePosition().getHeight();
}


EXPORT void CALL csSetControlText(IGUIElement* control, const char* text) {
    control->setText(_WSTR(text));
}


EXPORT const char* CALL csControlText(IGUIElement* control) {
    return _CSTR(control->getText());
}


EXPORT void CALL csSetControlTooltip(IGUIElement* control, const char* tooltip) {
    control->setToolTipText(_WSTR(tooltip));
}


EXPORT const char* CALL csControlTooltip(IGUIElement* control) {
    return _CSTR(control->getToolTipText().c_str());
}


EXPORT void CALL csSetControlTexture(IGUIElement* control, ITexture* texture, bool_t alpha) {
    switch (csControlType(control)) {
        case CSCONTROL_BUTTON:
            ((IGUIButton*)control)->setImage(texture);
            ((IGUIButton*)control)->setUseAlphaChannel(alpha);
            break;
        case CSCONTROL_IMAGE:
            ((IGUIImage*)control)->setImage(texture);
            ((IGUIImage*)control)->setUseAlphaChannel(alpha);
            break;
    }
}


EXPORT void CALL csSetControlFont(IGUIElement* control, IGUIFont* font) {
    if (control != NULL) {
        switch (csControlType(control)) {
            case CSCONTROL_BUTTON:
                ((IGUIButton*)control)->setOverrideFont(font);
                break;
            case CSCONTROL_EDITBOX:
                ((IGUIEditBox*)control)->setOverrideFont(font);
                break;
            case CSCONTROL_TEXT:
                ((IGUIStaticText*)control)->setOverrideFont(font);
                break;
        }
    } else {
        _asGUI()->getSkin()->setFont(font);
    }
}


EXPORT IGUIFont* CALL csControlFont(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_BUTTON:
            return ((IGUIButton*)control)->getOverrideFont();
        case CSCONTROL_EDITBOX:
            ((IGUIEditBox*)control)->getOverrideFont();
        case CSCONTROL_TEXT:
            ((IGUIStaticText*)control)->getOverrideFont();
        default:
            return NULL;
    }
}


EXPORT void CALL csSetControlColor(IGUIElement* control, int color) {
    switch (csControlType(control)) {
        case CSCONTROL_EDITBOX:
            ((IGUIEditBox*)control)->setOverrideColor(_csColor(color));
            break;
        case CSCONTROL_TEXT:
            ((IGUIStaticText*)control)->setOverrideColor(_csColor(color));
    }
}


EXPORT void CALL csClearControlColor(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_EDITBOX:
            ((IGUIEditBox*)control)->enableOverrideColor(false);
            break;
        case CSCONTROL_TEXT:
            ((IGUIStaticText*)control)->enableOverrideColor(false);
    }
}


EXPORT int CALL csControlColor(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_EDITBOX:
            return _csIntColor(((IGUIEditBox*)control)->getOverrideColor());
        case CSCONTROL_TEXT:
            return _csIntColor(((IGUIStaticText*)control)->getOverrideColor());
        default:
            return csRGB(0, 0, 0);
    }
}


EXPORT void CALL csSetControlChecked(IGUIElement* control, bool_t checked) {
    switch (csControlType(control)) {
        case CSCONTROL_BUTTON:
            ((IGUIButton*)control)->setPressed(checked);
            break;
        case CSCONTROL_CHECKBOX:
            ((IGUICheckBox*)control)->setChecked(checked);
            break;
    }
}


EXPORT bool_t CALL csControlChecked(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_BUTTON:
            return ((IGUIButton*)control)->isPressed();
        case CSCONTROL_CHECKBOX:
            return ((IGUICheckBox*)control)->isChecked();
    }
    return false;
}


EXPORT void CALL csSetControlValue(IGUIElement* control, int value) {
    if (csControlType(control) == CSCONTROL_SCROLLBAR) ((IGUIScrollBar*)control)->setPos(value);
}


EXPORT int CALL csControlValue(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_SCROLLBAR:
            return ((IGUIScrollBar*)control)->getPos();
        case CSCONTROL_TABBAR:
            return ((IGUITabControl*)control)->getActiveTab();
    }
    return 0;
}


EXPORT int CALL csAddControlItem(IGUIElement* control, const char* text, int id) {
    switch (csControlType(control)) {
        case CSCONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->addItem(_WSTR(text));
        case CSCONTROL_MENU:
            return ((IGUIContextMenu*)control)->addItem(_WSTR(text), id);
        case CSCONTROL_LISTBOX:
            return ((IGUIListBox*)control)->addItem(_WSTR(text));
        case CSCONTROL_TABBAR:
            ((IGUITabControl*)control)->addTab(_WSTR(text), id);
            return ((IGUITabControl*)control)->getTabCount() - 1;
        case CSCONTROL_TOOLBAR: {
            IGUIButton* button = ((IGUIToolBar*)control)->addButton(id, _WSTR(text));
            button->setDrawBorder(false);
            return csControlNumChildren(control) - 1;
        }
    }
    return -1;
}


EXPORT int CALL csControlNumItems(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->getItemCount();
        case CSCONTROL_MENU:
            return ((IGUIContextMenu*)control)->getItemCount();
        case CSCONTROL_LISTBOX:
            return ((IGUIListBox*)control)->getItemCount();
        case CSCONTROL_TABBAR:
            return ((IGUITabControl*)control)->getTabCount();
        case CSCONTROL_TOOLBAR:
            return csControlNumChildren(control);
    }
    return 0;
}


EXPORT void CALL csSetControlItemEnabled(IGUIElement* control, int index, bool_t enable) {
    switch (csControlType(control)) {
        case CSCONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemEnabled(index, enable);
            break;
    }
}


EXPORT bool_t CALL csControlItemEnabled(IGUIElement* control, int index) {
    switch (csControlType(control)) {
        case CSCONTROL_MENU:
            return ((IGUIContextMenu*)control)->isItemEnabled(index);
    }
    return false;
}


EXPORT void CALL csSetControlItemChecked(IGUIElement* control, int index, bool_t check) {
    switch (csControlType(control)) {
        case CSCONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemChecked(index, check);
            break;
        case CSCONTROL_TOOLBAR:
            ((IGUIButton*)csControlChild(control, index))->setPressed(check);
            break;
    }
}


EXPORT bool_t CALL csControlItemChecked(IGUIElement* control, int index) {
    switch (csControlType(control)) {
        case CSCONTROL_MENU:
            return ((IGUIContextMenu*)control)->isItemChecked(index);
        case CSCONTROL_TOOLBAR:
            return ((IGUIButton*)csControlChild(control, index))->isPressed();
    }
    return false;
}


EXPORT void CALL csSelectControlItem(IGUIElement* control, int index) {
    switch (csControlType(control)) {
        case CSCONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->setSelected(index);
            break;
        case CSCONTROL_LISTBOX:
            ((IGUIListBox*)control)->setSelected(index);
            break;
        case CSCONTROL_TABBAR:
            ((IGUITabControl*)control)->setActiveTab(index);
            break;
    }
}


EXPORT int CALL csSelectedControlItem(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->getSelected();
        case CSCONTROL_LISTBOX:
            return ((IGUIListBox*)control)->getSelected();
        case CSCONTROL_TABBAR:
            return ((IGUITabControl*)control)->getActiveTab();
    }
    return -1;
}


EXPORT void CALL csSetControlItemText(IGUIElement* control, int index, const char* text) {
    switch (csControlType(control)) {
        case CSCONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemText(index, _WSTR(text));
            break;
    }
}


EXPORT const char* CALL csControlItemText(IGUIElement* control, int index) {
    switch (csControlType(control)) {
        case CSCONTROL_MENU:
            return _CSTR(((IGUIContextMenu*)control)->getItemText(index));
    }
    return _CSTR(L"");
}


EXPORT void CALL csRemoveControlItem(IGUIElement* control, int index) {
    switch (csControlType(control)) {
        case CSCONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->removeItem(index);
        case CSCONTROL_MENU:
            ((IGUIContextMenu*)control)->removeItem(index);
            break;
        case CSCONTROL_LISTBOX:
            ((IGUIListBox*)control)->removeItem(index);
            break;
        case CSCONTROL_TABBAR:
            ((IGUITabControl*)control)->removeTab(index);
            break;
    }
}


EXPORT void CALL csRemoveControlItems(IGUIElement* control) {
    switch (csControlType(control)) {
        case CSCONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->clear();
        case CSCONTROL_MENU:
            ((IGUIContextMenu*)control)->removeAllItems();
            break;
        case CSCONTROL_LISTBOX:
            ((IGUIListBox*)control)->clear();
            break;
        case CSCONTROL_TABBAR:
            ((IGUITabControl*)control)->clear();
            break;
    }
}


EXPORT void CALL csAddMenuSeparator(IGUIElement* menu) {
    if (csControlType(menu) == CSCONTROL_MENU) {
        ((IGUIContextMenu*)menu)->addSeparator();
    }
}


EXPORT void CALL csDrawGUI() {
    _asGUI()->drawAll();
}


EXPORT bool_t CALL csPrepareNextGUIEvent() {
    if (_events.size() > 0) {
        _currentEvent = _events[0];
        _events.erase(0);
        return true;
    } else {
        return false;
    }
}


EXPORT int CALL csGUIEventType() {
    return _currentEvent.type;
}


EXPORT IGUIElement* CALL csGUIEventControl() {
    return _currentEvent.control;
}


IGUIEnvironment* _asGUI() {
    return _csDevice()->getGUIEnvironment();
}


IGUIElement* _asResolveParent(IGUIElement* parent) {
    return (parent != NULL) ? parent : _csDevice()->getGUIEnvironment()->getRootGUIElement();
}


void _csPostEvent(int type, IGUIElement* control) {
    _events.push_back(csEvent(type, control));
}


} // extern "C"
