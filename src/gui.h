/** @file
 * Functions for creating and managing GUI controls.
 */
#pragma once

#include "common.h"

/** @name Gadget types */
/**@{*/
#define CONTROL_BUTTON 1
#define CONTROL_CHECKBOX 2
#define CONTROL_COMBOBOX 3
#define CONTROL_EDITBOX 4
#define CONTROL_IMAGE 5
#define CONTROL_LISTBOX 6
#define CONTROL_MENU 7
#define CONTROL_SCROLLBAR 8
#define CONTROL_TABBAR 9
#define CONTROL_TEXT 10
#define CONTROL_TOOLBAR 11
#define CONTROL_WINDOW 12
/**@}*/

/** @name Gadget events */
/**@{*/
#define CONTROL_LOSTFOCUS 0
#define CONTROL_ENTER 1
#define CONTROL_EXIT 2
#define CONTROL_ACTION 3
/**@}*/

/** @name EditBox types */
/**@{*/
#define EDITBOX_SIMPLE 0
#define EDITBOX_PASSWORD 1
#define EDITBOX_MULTILINE 2
/**@}*/

#ifdef __cplusplus
extern "C"
{
#endif

  struct Font;

  /**
   * Creates a button control.
   *
   * @param x The X position of the button.
   * @param y The Y position of the button.
   * @param width The width of the button.
   * @param height The height of the button.
   * @param text The button label.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new button control.
   */
  EXPORT IGUIElement *CALL CreateButton(int x, int y, int width, int height, const char *text, IGUIElement *parent);
  /**
   * Creates a checkbox control.
   *
   * @param x The X position of the checkbox.
   * @param y The Y position of the checkbox.
   * @param width The width of the checkbox.
   * @param height The height of the checkbox.
   * @param text The checkbox label.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new checkbox control.
   */
  EXPORT IGUIElement *CALL CreateCheckBox(int x, int y, int width, int height, const char *text, IGUIElement *parent);

  /**
   * Creates a combobox control.
   *
   * @param x The X position of the combobox.
   * @param y The Y position of the combobox.
   * @param width The width of the combobox.
   * @param height The height of the combobox.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new combobox control.
   */
  EXPORT IGUIElement *CALL CreateComboBox(int x, int y, int width, int height, IGUIElement *parent);

  /**
   * Creates a context menu control.
   *
   * @param x The X position of the context menu.
   * @param y The Y position of the context menu.
   * @param width The width of the context menu.
   * @param height The height of the context menu.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new context menu control.
   */
  EXPORT IGUIElement *CALL CreateContextMenu(int x, int y, int width, int height, IGUIElement *parent);

  /**
   * Creates an edit box control.
   *
   * @param x The X position of the edit box.
   * @param y The Y position of the edit box.
   * @param width The width of the edit box.
   * @param height The height of the edit box.
   * @param type The edit box type as one of the EDITBOX_* constants.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new edit box control.
   */
  EXPORT IGUIElement *CALL CreateEditBox(int x, int y, int width, int height, int type, IGUIElement *parent);

  /**
   * Creates an image control.
   *
   * @param x The X position of the image.
   * @param y The Y position of the image.
   * @param width The width of the image.
   * @param height The height of the image.
   * @param texture The texture to display.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new image control.
   */
  EXPORT IGUIElement *CALL CreateImage(int x, int y, int width, int height, ITexture *texture, IGUIElement *parent);

  /**
   * Creates a list box control.
   *
   * @param x The X position of the list box.
   * @param y The Y position of the list box.
   * @param width The width of the list box.
   * @param height The height of the list box.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new list box control.
   */
  EXPORT IGUIElement *CALL CreateListBox(int x, int y, int width, int height, IGUIElement *parent);

  /**
   * Creates a menu control. If parent is a menu or context menu, a submenu is added.
   *
   * @param text The menu text.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new menu control.
   */
  EXPORT IGUIElement *CALL CreateMenu(const char *text, IGUIElement *parent);

  /**
   * Creates a scrollbar control.
   *
   * @param x The X position of the scrollbar.
   * @param y The Y position of the scrollbar.
   * @param width The width of the scrollbar.
   * @param height The height of the scrollbar.
   * @param horizontal True for a horizontal scrollbar, false for vertical.
   * @param max The maximum scroll value.
   * @param step The small step value.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new scrollbar control.
   */
  EXPORT IGUIElement *CALL CreateScrollbar(int x, int y, int width, int height, bool_t horizontal, int max, int step, IGUIElement *parent);

  /**
   * Creates a tab bar control.
   *
   * @param x The X position of the tab bar.
   * @param y The Y position of the tab bar.
   * @param width The width of the tab bar.
   * @param height The height of the tab bar.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new tab bar control.
   */
  EXPORT IGUIElement *CALL CreateTabbar(int x, int y, int width, int height, IGUIElement *parent);

  /**
   * Creates a static text control.
   *
   * @param x The X position of the text.
   * @param y The Y position of the text.
   * @param width The width of the text area.
   * @param height The height of the text area.
   * @param text The text to display.
   * @param border True to draw a border around the text.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new text control.
   */
  EXPORT IGUIElement *CALL CreateText(int x, int y, int width, int height, const char *text, bool_t border, IGUIElement *parent);

  /**
   * Creates a toolbar control.
   *
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new toolbar control.
   */
  EXPORT IGUIElement *CALL CreateToolbar(IGUIElement *parent);

  /**
   * Creates a window control.
   *
   * @param x The X position of the window.
   * @param y The Y position of the window.
   * @param width The width of the window.
   * @param height The height of the window.
   * @param modal True for a modal window.
   * @param parent The parent control, or NULL for the root GUI element.
   * @return The new window control.
   */
  EXPORT IGUIElement *CALL CreateWindow(int x, int y, int width, int height, bool_t modal, IGUIElement *parent);

  /**
   * Frees a control and removes it from the GUI.
   *
   * @param control The control to free.
   */
  EXPORT void CALL FreeControl(IGUIElement *control);

  /**
   * Returns the type of a control.
   *
   * @param control The control to query.
   * @return The control type as one of the CONTROL_* constants.
   */
  EXPORT int CALL ControlType(IGUIElement *control);

  /**
   * Sets whether a control is enabled.
   *
   * @param control The control whose enabled state will be set.
   * @param enabled The new enabled state.
   */
  EXPORT void CALL SetControlEnabled(IGUIElement *control, bool_t enabled);

  /**
   * Returns whether a control is enabled.
   *
   * @param control The control to query.
   * @return True if the control is enabled.
   */
  EXPORT bool_t CALL ControlEnabled(IGUIElement *control);

  /**
   * Sets whether a control is visible.
   *
   * @param control The control whose visibility will be set.
   * @param visible The new visibility state.
   */
  EXPORT void CALL SetControlVisible(IGUIElement *control, bool_t visible);

  /**
   * Returns whether a control is visible.
   *
   * @param control The control to query.
   * @return True if the control is visible.
   */
  EXPORT bool_t CALL ControlVisible(IGUIElement *control);

  /**
   * Returns the parent of a control.
   *
   * @param control The control to query.
   * @return The parent control, or NULL if the control has no parent.
   */
  EXPORT IGUIElement *CALL ControlParent(IGUIElement *control);

  /**
   * Returns the number of child controls attached to a control.
   *
   * @param control The control to query.
   * @return The number of child controls.
   */
  EXPORT int CALL ControlNumChildren(IGUIElement *control);

  /**
   * Returns a child control by index.
   *
   * @param control The parent control.
   * @param index The child index.
   * @return The child control at the given index.
   */
  EXPORT IGUIElement *CALL ControlChild(IGUIElement *control, int index);

  /**
   * Sets the position and size of a control.
   *
   * @param control The control whose shape will be set.
   * @param x The new X position.
   * @param y The new Y position.
   * @param width The new width.
   * @param height The new height.
   */
  EXPORT void CALL SetControlShape(IGUIElement *control, int x, int y, int width, int height);

  /**
   * Returns the X position of a control.
   *
   * @param control The control to query.
   * @return The X position.
   */
  EXPORT int CALL ControlX(IGUIElement *control);

  /**
   * Returns the Y position of a control.
   *
   * @param control The control to query.
   * @return The Y position.
   */
  EXPORT int CALL ControlY(IGUIElement *control);

  /**
   * Returns the width of a control.
   *
   * @param control The control to query.
   * @return The width.
   */
  EXPORT int CALL ControlWidth(IGUIElement *control);

  /**
   * Returns the height of a control.
   *
   * @param control The control to query.
   * @return The height.
   */
  EXPORT int CALL ControlHeight(IGUIElement *control);

  /**
   * Sets the text of a control.
   *
   * @param control The control whose text will be set.
   * @param text The new text.
   */
  EXPORT void CALL SetControlText(IGUIElement *control, const char *text);

  /**
   * Returns the text of a control.
   *
   * @param control The control to query.
   * @return The control's text.
   */
  EXPORT const char *CALL ControlText(IGUIElement *control);

  /**
   * Sets the tooltip of a control.
   *
   * @param control The control whose tooltip will be set.
   * @param tooltip The new tooltip text.
   */
  EXPORT void CALL SetControlTooltip(IGUIElement *control, const char *tooltip);

  /**
   * Returns the tooltip of a control.
   *
   * @param control The control to query.
   * @return The control's tooltip text.
   */
  EXPORT const char *CALL ControlTooltip(IGUIElement *control);

  /**
   * Sets the texture of a button or image control.
   *
   * @param control The button or image control.
   * @param texture The texture to set.
   * @param alpha True to use the texture's alpha channel.
   */
  EXPORT void CALL SetControlTexture(IGUIElement *control, ITexture *texture, bool_t alpha);

  /**
   * Sets the font of a control. Pass NULL for control to set the global skin font.
   *
   * @param control The control whose font will be set, or NULL for the global font.
   * @param font The font to use.
   */
  EXPORT void CALL SetControlFont(IGUIElement *control, Font *font);

  /**
   * Returns the font of a control.
   *
   * @param control The control to query.
   * @return The control's font, or NULL if no override font is set.
   */
  EXPORT Font *CALL ControlFont(IGUIElement *control);

  /**
   * Sets the override color of an edit box or text control.
   *
   * @param control The edit box or text control.
   * @param color The new color.
   */
  EXPORT void CALL SetControlColor(IGUIElement *control, int color);

  /**
   * Clears the override color of an edit box or text control.
   *
   * @param control The edit box or text control.
   */
  EXPORT void CALL ClearControlColor(IGUIElement *control);

  /**
   * Returns the override color of an edit box or text control.
   *
   * @param control The edit box or text control.
   * @return The override color, or black if no override is set.
   */
  EXPORT int CALL ControlColor(IGUIElement *control);

  /**
   * Returns the cursor position of an edit box.
   *
   * @param control The edit box control.
   * @return The cursor position (character index).
   */
  EXPORT int CALL EditBoxCursorPos(IGUIElement *control);

  /**
   * Sets the cursor position of an edit box.
   *
   * @param control The edit box control.
   * @param pos The new cursor position (character index).
   */
  EXPORT void CALL SetEditBoxCursorPos(IGUIElement *control, int pos);

  /**
   * Sets the checked state of a button or checkbox control.
   *
   * @param control The button or checkbox control.
   * @param checked The new checked state.
   */
  EXPORT void CALL SetControlChecked(IGUIElement *control, bool_t checked);

  /**
   * Returns whether a button or checkbox control is checked.
   *
   * @param control The button or checkbox control.
   * @return True if the control is checked.
   */
  EXPORT bool_t CALL ControlChecked(IGUIElement *control);

  /**
   * Sets the value of a scrollbar or the active tab of a tab bar.
   *
   * @param control The scrollbar or tab bar control.
   * @param value The new value.
   */
  EXPORT void CALL SetControlValue(IGUIElement *control, int value);

  /**
   * Returns the value of a scrollbar or the active tab of a tab bar.
   *
   * @param control The scrollbar or tab bar control.
   * @return The current value.
   */
  EXPORT int CALL ControlValue(IGUIElement *control);

  /**
   * Returns the ID of a control.
   *
   * @param control The control to query.
   * @return The control's ID.
   */
  EXPORT int CALL ControlId(IGUIElement *control);

  /**
   * Adds an item to a combobox, menu, list box, tab bar, or toolbar control.
   *
   * @param control The control to add the item to.
   * @param text The item text.
   * @param id The item ID (used by menus).
   * @return The number of items in the control after adding.
   */
  EXPORT int CALL AddControlItem(IGUIElement *control, const char *text, int id);

  /**
   * Returns the number of items in a combobox, menu, list box, tab bar, or toolbar control.
   *
   * @param control The control to query.
   * @return The number of items.
   */
  EXPORT int CALL ControlNumItems(IGUIElement *control);

  /**
   * Sets whether a menu item is enabled.
   *
   * @param control The menu control.
   * @param index The item index.
   * @param enable The new enabled state.
   */
  EXPORT void CALL SetControlItemEnabled(IGUIElement *control, int index, bool_t enable);

  /**
   * Returns whether a menu item is enabled.
   *
   * @param control The menu control.
   * @param index The item index.
   * @return True if the item is enabled.
   */
  EXPORT bool_t CALL ControlItemEnabled(IGUIElement *control, int index);

  /**
   * Sets whether a menu or toolbar item is checked.
   *
   * @param control The menu or toolbar control.
   * @param index The item index.
   * @param check The new checked state.
   */
  EXPORT void CALL SetControlItemChecked(IGUIElement *control, int index, bool_t check);

  /**
   * Returns whether a menu or toolbar item is checked.
   *
   * @param control The menu or toolbar control.
   * @param index The item index.
   * @return True if the item is checked.
   */
  EXPORT bool_t CALL ControlItemChecked(IGUIElement *control, int index);

  /**
   * Selects an item in a combobox, list box, or tab bar control.
   *
   * @param control The combobox, list box, or tab bar control.
   * @param index The item index to select.
   */
  EXPORT void CALL SelectControlItem(IGUIElement *control, int index);

  /**
   * Returns the selected item index in a combobox, list box, or tab bar control.
   *
   * @param control The combobox, list box, or tab bar control.
   * @return The selected item index, or -1 if no item is selected.
   */
  EXPORT int CALL SelectedControlItem(IGUIElement *control);

  /**
   * Sets the text of a menu item.
   *
   * @param control The menu control.
   * @param index The item index.
   * @param text The new item text.
   */
  EXPORT void CALL SetControlItemText(IGUIElement *control, int index, const char *text);

  /**
   * Returns the text of a menu item.
   *
   * @param control The menu control.
   * @param index The item index.
   * @return The item text.
   */
  EXPORT const char *CALL ControlItemText(IGUIElement *control, int index);

  /**
   * Removes an item from a combobox, menu, list box, or tab bar control.
   *
   * @param control The control to remove the item from.
   * @param index The item index to remove.
   */
  EXPORT void CALL RemoveControlItem(IGUIElement *control, int index);

  /**
   * Removes all items from a combobox, menu, list box, or tab bar control.
   *
   * @param control The control to clear.
   */
  EXPORT void CALL RemoveControlItems(IGUIElement *control);

  /**
   * Adds a separator to a menu control.
   *
   * @param menu The menu control.
   */
  EXPORT void CALL AddMenuSeparator(IGUIElement *menu);

  /**
   * Draws all visible GUI controls.
   */
  EXPORT void CALL DrawGUI();

  /**
   * Sets the keyboard focus to a control.
   *
   * @param control The control to focus.
   */
  EXPORT void CALL SetFocusedControl(IGUIElement *control);

  /**
   * Returns the control that has keyboard focus.
   *
   * @return The focused control, or NULL if no control is focused.
   */
  EXPORT IGUIElement *CALL FocusedControl();

  /**
   * Sets whether pressing Tab moves keyboard focus to the next control.
   *
   * @param accept True to allow Tab focus navigation (default),
   *               false to let Tab be handled as a regular key.
   */
  EXPORT void CALL SetTabFocusNavigation(bool_t accept);

  /**
   * Returns whether pressing Tab moves keyboard focus to the next control.
   *
   * @return True if Tab changes focus, false if Tab is a regular key.
   */
  EXPORT bool_t CALL TabFocusNavigation();

  /**
   * Prepares the next queued GUI event for reading. Call this before GUIEventType, GUIEventControl, and GUIEventMenuId.
   *
   * @return True if an event was available, false if the queue was empty.
   */
  EXPORT bool_t CALL PrepareNextGUIEvent();

  /**
   * Returns the type of the current GUI event.
   *
   * @return The event type as one of the CONTROL_* event constants.
   */
  EXPORT int CALL GUIEventType();

  /**
   * Returns the control that triggered the current GUI event.
   *
   * @return The control that triggered the event.
   */
  EXPORT IGUIElement *CALL GUIEventControl();

  /**
   * Returns the menu ID of the current GUI event.
   *
   * @return The menu item ID, or -1 if not applicable.
   */
  EXPORT int CALL GUIEventMenuId();

#ifndef SWIG
  void _PostEvent(int type, IGUIElement *control, int menuId);
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
