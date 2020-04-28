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


EXPORT IGUIElement* CALL asCreateButton(int x, int y, int width, int height, const char* text, IGUIElement* parent) {
    return _asGUI()->addButton(recti(x, y, x+width, y+height), _asResolveParent(parent), -1, _WSTR(text));
}


EXPORT IGUIElement* CALL asCreateCheckBox(int x, int y, int width, int height, const char* text, IGUIElement* parent) {
    return _asGUI()->addCheckBox(false, recti(x, y, x+width, y+height), _asResolveParent(parent), -1, _WSTR(text));
}


EXPORT IGUIElement* CALL asCreateComboBox(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addComboBox(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL asCreateContextMenu(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addContextMenu(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL asCreateEditBox(int x, int y, int width, int height, int type, IGUIElement* parent) {
    IGUIEditBox* box = _asGUI()->addEditBox(L"", recti(x, y, x+width, y+height), true, _asResolveParent(parent));
    if (type == ASEDITBOX_PASSWORD) box->setPasswordBox(true, L'*');
    else if (type == ASEDITBOX_MULTILINE) box->setMultiLine(true);
    return box;
}


EXPORT IGUIElement* CALL asCreateImage(int x, int y, int width, int height, ITexture* texture, IGUIElement* parent) {
    IGUIImage* image = _asGUI()->addImage(recti(x, y, x+width, y+height), _asResolveParent(parent));
    image->setImage(texture);
    image->setScaleImage(true);
    return image;
}


EXPORT IGUIElement* CALL asCreateListBox(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addListBox(recti(x, y, x+width, y+height), _asResolveParent(parent), -1, true);
}


EXPORT IGUIElement* CALL asCreateMenu(const char* text, IGUIElement* parent) {
    parent = _asResolveParent(parent);
    IGUIContextMenu* menu = (parent->getType() == EGUIET_CONTEXT_MENU || parent->getType() == EGUIET_MENU)
        ? ((IGUIContextMenu*)parent)->getSubMenu(((IGUIContextMenu*)parent)->addItem(_WSTR(text), -1, true, true))
        : _asGUI()->addMenu((parent->getType() == EGUIET_WINDOW) ? parent : NULL);
    menu->setText(_WSTR(text));
    return menu;
}


EXPORT IGUIElement* CALL asCreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement* parent) {
    IGUIScrollBar* scrollbar = _asGUI()->addScrollBar(horizontal, recti(x, y, x+width, y+height), _asResolveParent(parent));
    scrollbar->setMax(max);
    scrollbar->setSmallStep(step);
    return scrollbar;
}


EXPORT IGUIElement* CALL asCreateTabbar(int x, int y, int width, int height, IGUIElement* parent) {
    return _asGUI()->addTabControl(recti(x, y, x+width, y+height), _asResolveParent(parent));
}


EXPORT IGUIElement* CALL asCreateText(int x, int y, int width, int height, const char* text, bool_t border, IGUIElement* parent) {
    return _asGUI()->addStaticText(_WSTR(text), recti(x, y, x+width, y+height), border, true, _asResolveParent(parent));
}


EXPORT IGUIElement* CALL asCreateToolbar(IGUIElement* parent) {
    return _asGUI()->addToolBar(_asResolveParent(parent));
}


EXPORT IGUIElement* CALL asCreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement* parent) {
    return _asGUI()->addWindow(recti(x, y, x+width, y+height), modal, NULL, _asResolveParent(parent));
}


EXPORT void CALL asDeleteControl(IGUIElement* control) {
    control->remove();
}


EXPORT int CALL asControlType(IGUIElement* control) {
    map<EGUI_ELEMENT_TYPE, int> types;
    types.set(EGUIET_BUTTON, ASCONTROL_BUTTON);
    types.set(EGUIET_CHECK_BOX, ASCONTROL_CHECKBOX);
    types.set(EGUIET_COMBO_BOX, ASCONTROL_COMBOBOX);
    types.set(EGUIET_EDIT_BOX, ASCONTROL_EDITBOX);
    types.set(EGUIET_IMAGE, ASCONTROL_IMAGE);
    types.set(EGUIET_LIST_BOX, ASCONTROL_LISTBOX);
    types.set(EGUIET_CONTEXT_MENU, ASCONTROL_MENU);
    types.set(EGUIET_MENU, ASCONTROL_MENU);
    types.set(EGUIET_SCROLL_BAR, ASCONTROL_SCROLLBAR);
    types.set(EGUIET_TAB_CONTROL, ASCONTROL_TABBAR);
    types.set(EGUIET_STATIC_TEXT, ASCONTROL_TEXT);
    types.set(EGUIET_TOOL_BAR, ASCONTROL_TOOLBAR);
    types.set(EGUIET_WINDOW, ASCONTROL_WINDOW);
    return types[control->getType()];
}


EXPORT void CALL asSetControlEnabled(IGUIElement* control, bool_t enabled) {
    control->setEnabled(enabled);
}


EXPORT bool_t CALL asControlEnabled(IGUIElement* control) {
    return control->isEnabled();
}


EXPORT void CALL asSetControlVisible(IGUIElement* control, bool_t visible) {
    control->setVisible(visible);
}


EXPORT bool_t CALL asControlVisible(IGUIElement* control) {
    return control->isVisible();
}


EXPORT IGUIElement* CALL asControlParent(IGUIElement* control) {
    return control->getParent();
}


EXPORT int CALL asControlNumChildren(IGUIElement* control) {
    return control->getChildren().size() - ((asControlType(control) == ASCONTROL_TABBAR) ? 2 : 0);
}


EXPORT IGUIElement* CALL asControlChild(IGUIElement* control, int index) {
    return *(control->getChildren().begin() + index);
}


EXPORT void CALL asSetControlShape(IGUIElement* control, int x, int y, int width, int height) {
    control->setRelativePosition(recti(x, y, x+width, y+height));
}


EXPORT int CALL asControlX(IGUIElement* control) {
    return control->getRelativePosition().UpperLeftCorner.X;
}


EXPORT int CALL asControlY(IGUIElement* control) {
    return control->getRelativePosition().UpperLeftCorner.Y;
}


EXPORT int CALL asControlWidth(IGUIElement* control) {
    return control->getRelativePosition().getWidth();
}


EXPORT int CALL asControlHeight(IGUIElement* control) {
    return control->getRelativePosition().getHeight();
}


EXPORT void CALL asSetControlText(IGUIElement* control, const char* text) {
    control->setText(_WSTR(text));
}


EXPORT const char* CALL asControlText(IGUIElement* control) {
    return _CSTR(control->getText());
}


EXPORT void CALL asSetControlTooltip(IGUIElement* control, const char* tooltip) {
    control->setToolTipText(_WSTR(tooltip));
}


EXPORT const char* CALL asControlTooltip(IGUIElement* control) {
    return _CSTR(control->getToolTipText().c_str());
}


EXPORT void CALL asSetControlTexture(IGUIElement* control, ITexture* texture, bool_t alpha) {
    switch (asControlType(control)) {
        case ASCONTROL_BUTTON:
            ((IGUIButton*)control)->setImage(texture);
            ((IGUIButton*)control)->setUseAlphaChannel(alpha);
            break;
        case ASCONTROL_IMAGE:
            ((IGUIImage*)control)->setImage(texture);
            ((IGUIImage*)control)->setUseAlphaChannel(alpha);
            break;
    }
}


EXPORT void CALL asSetControlFont(IGUIElement* control, IGUIFont* font) {
    if (control != NULL) {
        switch (asControlType(control)) {
            case ASCONTROL_BUTTON:
                ((IGUIButton*)control)->setOverrideFont(font);
                break;
            case ASCONTROL_EDITBOX:
                ((IGUIEditBox*)control)->setOverrideFont(font);
                break;
            case ASCONTROL_TEXT:
                ((IGUIStaticText*)control)->setOverrideFont(font);
                break;
        }
    } else {
        _asGUI()->getSkin()->setFont(font);
    }
}


EXPORT IGUIFont* CALL asControlFont(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_BUTTON:
            return ((IGUIButton*)control)->getOverrideFont();
        case ASCONTROL_EDITBOX:
            ((IGUIEditBox*)control)->getOverrideFont();
        case ASCONTROL_TEXT:
            ((IGUIStaticText*)control)->getOverrideFont();
        default:
            return NULL;
    }
}


EXPORT void CALL asSetControlColor(IGUIElement* control, int color) {
    switch (asControlType(control)) {
        case ASCONTROL_EDITBOX:
            ((IGUIEditBox*)control)->setOverrideColor(_asColor(color));
            break;
        case ASCONTROL_TEXT:
            ((IGUIStaticText*)control)->setOverrideColor(_asColor(color));
    }
}


EXPORT void CALL asClearControlColor(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_EDITBOX:
            ((IGUIEditBox*)control)->enableOverrideColor(false);
            break;
        case ASCONTROL_TEXT:
            ((IGUIStaticText*)control)->enableOverrideColor(false);
    }
}


EXPORT int CALL asControlColor(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_EDITBOX:
            return _asIntColor(((IGUIEditBox*)control)->getOverrideColor());
        case ASCONTROL_TEXT:
            return _asIntColor(((IGUIStaticText*)control)->getOverrideColor());
        default:
            return asRGB(0, 0, 0);
    }
}


EXPORT void CALL asSetControlChecked(IGUIElement* control, bool_t checked) {
    switch (asControlType(control)) {
        case ASCONTROL_BUTTON:
            ((IGUIButton*)control)->setPressed(checked);
            break;
        case ASCONTROL_CHECKBOX:
            ((IGUICheckBox*)control)->setChecked(checked);
            break;
    }
}


EXPORT bool_t CALL asControlChecked(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_BUTTON:
            return ((IGUIButton*)control)->isPressed();
        case ASCONTROL_CHECKBOX:
            return ((IGUICheckBox*)control)->isChecked();
    }
    return false;
}


EXPORT void CALL asSetControlValue(IGUIElement* control, int value) {
    if (asControlType(control) == ASCONTROL_SCROLLBAR) ((IGUIScrollBar*)control)->setPos(value);
}


EXPORT int CALL asControlValue(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_SCROLLBAR:
            return ((IGUIScrollBar*)control)->getPos();
        case ASCONTROL_TABBAR:
            return ((IGUITabControl*)control)->getActiveTab();
    }
    return 0;
}


EXPORT int CALL asAddControlItem(IGUIElement* control, const char* text, int id) {
    switch (asControlType(control)) {
        case ASCONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->addItem(_WSTR(text));
        case ASCONTROL_MENU:
            return ((IGUIContextMenu*)control)->addItem(_WSTR(text), id);
        case ASCONTROL_LISTBOX:
            return ((IGUIListBox*)control)->addItem(_WSTR(text));
        case ASCONTROL_TABBAR:
            ((IGUITabControl*)control)->addTab(_WSTR(text), id);
            return ((IGUITabControl*)control)->getTabCount() - 1;
        case ASCONTROL_TOOLBAR: {
            IGUIButton* button = ((IGUIToolBar*)control)->addButton(id, _WSTR(text));
            button->setDrawBorder(false);
            return asControlNumChildren(control) - 1;
        }
    }
    return -1;
}


EXPORT int CALL asControlNumItems(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->getItemCount();
        case ASCONTROL_MENU:
            return ((IGUIContextMenu*)control)->getItemCount();
        case ASCONTROL_LISTBOX:
            return ((IGUIListBox*)control)->getItemCount();
        case ASCONTROL_TABBAR:
            return ((IGUITabControl*)control)->getTabCount();
        case ASCONTROL_TOOLBAR:
            return asControlNumChildren(control);
    }
    return 0;
}


EXPORT void CALL asSetControlItemEnabled(IGUIElement* control, int index, bool_t enable) {
    switch (asControlType(control)) {
        case ASCONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemEnabled(index, enable);
            break;
    }
}


EXPORT bool_t CALL asControlItemEnabled(IGUIElement* control, int index) {
    switch (asControlType(control)) {
        case ASCONTROL_MENU:
            return ((IGUIContextMenu*)control)->isItemEnabled(index);
    }
    return false;
}


EXPORT void CALL asSetControlItemChecked(IGUIElement* control, int index, bool_t check) {
    switch (asControlType(control)) {
        case ASCONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemChecked(index, check);
            break;
        case ASCONTROL_TOOLBAR:
            ((IGUIButton*)asControlChild(control, index))->setPressed(check);
            break;
    }
}


EXPORT bool_t CALL asControlItemChecked(IGUIElement* control, int index) {
    switch (asControlType(control)) {
        case ASCONTROL_MENU:
            return ((IGUIContextMenu*)control)->isItemChecked(index);
        case ASCONTROL_TOOLBAR:
            return ((IGUIButton*)asControlChild(control, index))->isPressed();
    }
    return false;
}


EXPORT void CALL asSelectControlItem(IGUIElement* control, int index) {
    switch (asControlType(control)) {
        case ASCONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->setSelected(index);
            break;
        case ASCONTROL_LISTBOX:
            ((IGUIListBox*)control)->setSelected(index);
            break;
        case ASCONTROL_TABBAR:
            ((IGUITabControl*)control)->setActiveTab(index);
            break;
    }
}


EXPORT int CALL asSelectedControlItem(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_COMBOBOX:
            return ((IGUIComboBox*)control)->getSelected();
        case ASCONTROL_LISTBOX:
            return ((IGUIListBox*)control)->getSelected();
        case ASCONTROL_TABBAR:
            return ((IGUITabControl*)control)->getActiveTab();
    }
    return -1;
}


EXPORT void CALL asSetControlItemText(IGUIElement* control, int index, const char* text) {
    switch (asControlType(control)) {
        case ASCONTROL_MENU:
            ((IGUIContextMenu*)control)->setItemText(index, _WSTR(text));
            break;
    }
}


EXPORT const char* CALL asControlItemText(IGUIElement* control, int index) {
    switch (asControlType(control)) {
        case ASCONTROL_MENU:
            return _CSTR(((IGUIContextMenu*)control)->getItemText(index));
    }
    return _CSTR(L"");
}


EXPORT void CALL asRemoveControlItem(IGUIElement* control, int index) {
    switch (asControlType(control)) {
        case ASCONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->removeItem(index);
        case ASCONTROL_MENU:
            ((IGUIContextMenu*)control)->removeItem(index);
            break;
        case ASCONTROL_LISTBOX:
            ((IGUIListBox*)control)->removeItem(index);
            break;
        case ASCONTROL_TABBAR:
            ((IGUITabControl*)control)->removeTab(index);
            break;
    }
}


EXPORT void CALL asRemoveControlItems(IGUIElement* control) {
    switch (asControlType(control)) {
        case ASCONTROL_COMBOBOX:
            ((IGUIComboBox*)control)->clear();
        case ASCONTROL_MENU:
            ((IGUIContextMenu*)control)->removeAllItems();
            break;
        case ASCONTROL_LISTBOX:
            ((IGUIListBox*)control)->clear();
            break;
        case ASCONTROL_TABBAR:
            ((IGUITabControl*)control)->clear();
            break;
    }
}


EXPORT void CALL asAddMenuSeparator(IGUIElement* menu) {
    if (asControlType(menu) == ASCONTROL_MENU) {
        ((IGUIContextMenu*)menu)->addSeparator();
    }
}


EXPORT void CALL asDrawGUI() {
    _asGUI()->drawAll();
}


EXPORT bool_t CALL asPrepareNextGUIEvent() {
    if (_events.size() > 0) {
        _currentEvent = _events[0];
        _events.erase(0);
        return true;
    } else {
        return false;
    }
}


EXPORT int CALL asGUIEventType() {
    return _currentEvent.type;
}


EXPORT IGUIElement* CALL asGUIEventControl() {
    return _currentEvent.control;
}


IGUIEnvironment* _asGUI() {
    return _asDevice()->getGUIEnvironment();
}


IGUIElement* _asResolveParent(IGUIElement* parent) {
    return (parent != NULL) ? parent : _asDevice()->getGUIEnvironment()->getRootGUIElement();
}


void _asPostEvent(int type, IGUIElement* control) {
    _events.push_back(csEvent(type, control));
}


} // extern "C"
