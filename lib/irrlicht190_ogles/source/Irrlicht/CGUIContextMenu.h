// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef IRR_C_GUI_CONTEXT_MENU_H_INCLUDED
#define IRR_C_GUI_CONTEXT_MENU_H_INCLUDED

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIContextMenu.h"
#include "irrString.h"
#include "irrArray.h"
#include "IGUIFont.h"

namespace irr
{
namespace gui
{

	//! GUI Context menu interface.
	class CGUIContextMenu : public IGUIContextMenu
	{
	public:

		//! constructor
		CGUIContextMenu(IGUIEnvironment* environment,
			IGUIElement* parent, s32 id, core::rect<s32> rectangle,
			bool getFocus = true, bool allowFocus = true);

		//! destructor
		virtual ~CGUIContextMenu();

		//! set behavior when menus are closed
		virtual void setCloseHandling(ECONTEXT_MENU_CLOSE onClose) IRR_OVERRIDE;

		//! get current behavior when the menu will be closed
		virtual ECONTEXT_MENU_CLOSE getCloseHandling() const IRR_OVERRIDE;

		//! When true menu is closed when the item check changes
		virtual void setCloseOnCheck(bool doCloseOnCheck) IRR_OVERRIDE
		{
			CloseOnCheck = doCloseOnCheck;
		}

		//! Get current setting for behaviour when checking items
		virtual bool getCloseOnCheck() const IRR_OVERRIDE
		{
			return CloseOnCheck;
		}

		//! Returns amount of menu items
		virtual u32 getItemCount() const IRR_OVERRIDE;

		//! Adds a menu item.
		virtual u32 addItem(const wchar_t* text, s32 commandid,
				bool enabled, bool hasSubMenu, bool checked, bool autoChecking) IRR_OVERRIDE;

		//! Insert a menu item at specified position.
		virtual u32 insertItem(u32 idx, const wchar_t* text, s32 commandId, bool enabled,
			bool hasSubMenu, bool checked, bool autoChecking) IRR_OVERRIDE;

		//! Find a item which has the given CommandId starting from given index
		virtual s32 findItemWithCommandId(s32 commandId, u32 idxStartSearch) const IRR_OVERRIDE;

		//! Adds a separator item to the menu
		virtual void addSeparator() IRR_OVERRIDE;

		//! Returns text of the menu item.
		virtual const wchar_t* getItemText(u32 idx) const IRR_OVERRIDE;

		//! Sets text of the menu item.
		virtual void setItemText(u32 idx, const wchar_t* text) IRR_OVERRIDE;

		//! Returns if a menu item is enabled
		virtual bool isItemEnabled(u32 idx) const IRR_OVERRIDE;

		//! Sets if the menu item should be enabled.
		virtual void setItemEnabled(u32 idx, bool enabled) IRR_OVERRIDE;

		//! Returns if a menu item is checked
		virtual bool isItemChecked(u32 idx) const IRR_OVERRIDE;

		//! Sets if the menu item should be checked.
		virtual void setItemChecked(u32 idx, bool enabled) IRR_OVERRIDE;

		//! Removes a menu item
		virtual void removeItem(u32 idx) IRR_OVERRIDE;

		//! Removes all menu items
		virtual void removeAllItems() IRR_OVERRIDE;

		//! called if an event happened.
		virtual bool OnEvent(const SEvent& event) IRR_OVERRIDE;

		//! draws the element and its children
		virtual void draw() IRR_OVERRIDE;

		//! Returns the selected item in the menu
		virtual s32 getSelectedItem() const IRR_OVERRIDE;

		//! Returns a pointer to the submenu of an item.
		//! \return Pointer to the submenu of an item.
		virtual IGUIContextMenu* getSubMenu(u32 idx) const IRR_OVERRIDE;

		//! Sets the visible state of this element.
		virtual void setVisible(bool visible) IRR_OVERRIDE;

		//! should the element change the checked status on clicking
		virtual void setItemAutoChecking(u32 idx, bool autoChecking) IRR_OVERRIDE;

		//! does the element change the checked status on clicking
		virtual bool getItemAutoChecking(u32 idx) const IRR_OVERRIDE;

		//! Returns command id of a menu item
		virtual s32 getItemCommandId(u32 idx) const IRR_OVERRIDE;

		//! Sets the command id of a menu item
		virtual void setItemCommandId(u32 idx, s32 id) IRR_OVERRIDE;

		//! Adds a sub menu from an element that already exists.
		virtual void setSubMenu(u32 index, CGUIContextMenu* menu);

		//! When an event parent is set it receives events instead of the usual parent element
		virtual void setEventParent(IGUIElement *parent) IRR_OVERRIDE;

		//! Writes attributes of the element.
		virtual void serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const IRR_OVERRIDE;

		//! Reads attributes of the element
		virtual void deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options) IRR_OVERRIDE;

	protected:

		void closeAllSubMenus();
		bool hasOpenSubMenu(irr::u32 *indexResult=0) const;

		struct SItem
		{
			core::stringw Text;
			bool IsSeparator;
			bool Enabled;
			bool Checked;
			bool AutoChecking;
			core::dimension2d<u32> Dim;
			s32 PosY;
			CGUIContextMenu* SubMenu;
			s32 CommandId;
		};

		virtual void recalculateSize();

		//! returns true, if an element was highlighted
		virtual bool highlight(const core::position2d<s32>& p);

		//! Decide if sub menus should be open or closed
		virtual void updateOpenSubMenus(irr::u32 menuDelayMs);

		//! sends a click Returns:
		//! 0 if click went outside of the element
		//! 1 if a valid, uncheckable item was clicked
		//! 2 if a valid, checkable item was clicked
		//! 3 if a non-clickable element was clicked
		virtual u32 sendClick(const core::position2d<s32>& p);

		//! returns the item highlight-area
		virtual core::rect<s32> getHRect(const SItem& i, const core::rect<s32>& absolute) const;

		//! Gets drawing rect of Item
		virtual core::rect<s32> getRect(const SItem& i, const core::rect<s32>& absolute) const;


		core::array<SItem> Items;
		core::position2d<s32> Pos;
		IGUIElement* EventParent;
		IGUIFont *LastFont;
		ECONTEXT_MENU_CLOSE CloseHandling;
		bool CloseOnCheck;
		s32 HighLighted;
		u32 ChangeTime;
		bool AllowFocus;
	};


} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif // IRR_C_GUI_CONTEXT_MENU_H_INCLUDED
