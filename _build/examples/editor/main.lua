OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetScreenTitle("ColdSteel - <untitled>")

local liberationSans = LoadFont("segoe_ui.xml")
local fixedsys = LoadFont("fixedsys.xml")
SetControlFont(nil, liberationSans)

-- Dark theme skin colors
SetSkinColor(SKINCOLOR_WINDOW, RGB(60, 60, 60))
SetSkinColor(SKINCOLOR_3D_FACE, RGB(50, 50, 50))
SetSkinColor(SKINCOLOR_3D_DARK_SHADOW, RGB(30, 30, 30))
SetSkinColor(SKINCOLOR_3D_SHADOW, RGB(40, 40, 40))
SetSkinColor(SKINCOLOR_3D_HIGH_LIGHT, RGB(72, 72, 72))
SetSkinColor(SKINCOLOR_3D_LIGHT, RGB(80, 80, 80))
SetSkinColor(SKINCOLOR_BUTTON_TEXT, RGB(210, 210, 210))
SetSkinColor(SKINCOLOR_GRAY_TEXT, RGB(120, 120, 120))
SetSkinColor(SKINCOLOR_HIGH_LIGHT, RGB(80, 120, 180))
SetSkinColor(SKINCOLOR_HIGH_LIGHT_TEXT, RGB(255, 255, 255))
SetSkinColor(SKINCOLOR_WINDOW_SYMBOL, RGB(200, 200, 200))
SetSkinColor(SKINCOLOR_GRAY_WINDOW_SYMBOL, RGB(100, 100, 100))
SetSkinColor(SKINCOLOR_EDITABLE, RGB(0, 0, 0))
SetSkinColor(SKINCOLOR_FOCUSED_EDITABLE, RGB(10, 10, 10))
SetSkinColor(SKINCOLOR_GRAY_EDITABLE, RGB(20, 20, 20))
SetSkinColor(SKINCOLOR_SCROLLBAR, RGB(40, 40, 40))
SetSkinColor(SKINCOLOR_ACTIVE_CAPTION, RGB(210, 210, 210))
SetSkinColor(SKINCOLOR_INACTIVE_CAPTION, RGB(140, 140, 140))
SetSkinColor(SKINCOLOR_TOOLTIP_BACKGROUND, RGB(70, 70, 70))
SetSkinColor(SKINCOLOR_TOOLTIP, RGB(220, 220, 220))
SetSkinColor(SKINCOLOR_ACTIVE_BORDER, RGB(100, 100, 100))
SetSkinColor(SKINCOLOR_INACTIVE_BORDER, RGB(50, 50, 50))
SetSkinColor(SKINCOLOR_APP_WORKSPACE, RGB(40, 40, 40))
SetSkinColor(SKINCOLOR_ICON, RGB(200, 200, 200))
SetSkinColor(SKINCOLOR_ICON_HIGH_LIGHT, RGB(255, 255, 255))

local icons = {
    LoadTexture("toolbar/new.png"),
    LoadTexture("toolbar/open.png"),
    LoadTexture("toolbar/save.png"),
    LoadTexture("toolbar/close.png")
}

-- Create menu
local mainMenu = CreateMenu("", nil)
local fileMenu = CreateMenu("File", mainMenu)
AddControlItem(fileMenu, "New", 101)
AddControlItem(fileMenu, "Open", 102)
AddControlItem(fileMenu, "Save", 103)
AddControlItem(fileMenu, "Save As...", 104)
AddControlItem(fileMenu, "Save All", 105)
AddMenuSeparator(fileMenu)
AddControlItem(fileMenu, "Close", 106)
AddControlItem(fileMenu, "Close All", 107)
AddMenuSeparator(fileMenu)
AddControlItem(fileMenu, "Next File", 108)
AddControlItem(fileMenu, "Previous File", 109)
AddMenuSeparator(fileMenu)
AddControlItem(fileMenu, "Exit", 110)
local editMenu = CreateMenu("Edit", mainMenu)
local programMenu = CreateMenu("Program", mainMenu)
local helpMenu = CreateMenu("Help", mainMenu)

-- Create toolbar
local toolbar = CreateToolbar(nil)
for i = 1, #icons do
    AddControlItem(toolbar, "   ", 100 + i)
    SetControlTexture(ControlChild(toolbar, i), icons[i], true)
end

local headerHeight = ControlHeight(mainMenu) + ControlHeight(toolbar)

-- Create tabbar
local tabbar = CreateTabbar(0, headerHeight, ScreenWidth(), ScreenHeight() - headerHeight - 20, nil)
SetTabFocusNavigation(false)

-- Per-tab state
local tabFilenames = {}
local tabSavedContent = {}

function isEmpty(s)
    return s == nil or s == ""
end

function isDirty(idx)
    if not idx or idx <= 0 then return false end
    local editBox = ControlChild(ControlChild(tabbar, idx), 1)
    if not editBox then return false end
    local saved = tabSavedContent[idx]
    if saved == nil then
        return ControlText(editBox) ~= ""
    end
    return ControlText(editBox) ~= saved
end

function updateTabLabel(idx)
    if not idx or idx <= 0 then return end
    local tab = ControlChild(tabbar, idx)
    if not tab then return end
    local name = tabFilenames[idx]
    if not name then name = "<untitled>" end
    if isDirty(idx) then name = "* " .. name end
    SetControlText(tab, name)
end

function newTab(filename, content)
    local idx = AddControlItem(tabbar, "<untitled>", 0)
    local tab = ControlChild(tabbar, idx)
    local editBox = CreateEditBox(0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20, EDITBOX_MULTILINE, tab)
    SetControlFont(editBox, fixedsys)
    SetControlColor(editBox, MultiplyColor(COLOR_GREEN, 0.5))
    tabFilenames[idx] = filename
    if content then
        SetControlText(editBox, content)
        tabSavedContent[idx] = content
    else
        tabSavedContent[idx] = nil
    end
    updateTabLabel(idx)
    SelectControlItem(tabbar, idx)
    SetFocusedControl(editBox)
    if filename then
        SetScreenTitle("ColdSteel - " .. filename)
    end
    return idx
end

function saveTab(idx, path)
    local editBox = ControlChild(ControlChild(tabbar, idx), 1)
    if not editBox then return end
    local file, err = io.open(path, "w")
    if file then
        local text = ControlText(editBox)
        file:write(text)
        file:close()
        tabFilenames[idx] = path
        tabSavedContent[idx] = text
        updateTabLabel(idx)
        SetScreenTitle("ColdSteel - " .. path)
    end
end

function performClose(idx)
    if idx <= 0 or idx > ControlNumItems(tabbar) then return end
    RemoveControlItem(tabbar, idx)
    table.remove(tabFilenames, idx)
    table.remove(tabSavedContent, idx)
    if ControlNumItems(tabbar) == 0 then
        newTab()
    else
        local active = SelectedControlItem(tabbar)
        if active == 0 then
            active = 1
            SelectControlItem(tabbar, 1)
        end
        local editBox = ControlChild(ControlChild(tabbar, active), 1)
        if editBox then
            SetFocusedControl(editBox)
        end
    end
end

function closeAllTabs()
    RemoveControlItems(tabbar)
    tabFilenames = {}
    tabSavedContent = {}
    newTab()
end

function focusActiveTab()
    local active = SelectedControlItem(tabbar)
    if active > 0 then
        local editBox = ControlChild(ControlChild(tabbar, active), 1)
        if editBox then
            SetFocusedControl(editBox)
        end
    end
end

function resizeTabs()
    SetControlShape(tabbar, 0, headerHeight, ScreenWidth(), ScreenHeight() - headerHeight - 20)
    for i = 1, ControlNumItems(tabbar) do
        local tab = ControlChild(tabbar, i)
        local editBox = ControlChild(tab, 1)
        if editBox then
            SetControlShape(editBox, 0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20)
        end
        updateTabLabel(i)
    end
end

function saveCurrentTab()
    local active = SelectedControlItem(tabbar)
    if active <= 0 then return end
    local filename = tabFilenames[active]
    if filename then
        saveTab(active, filename)
    else
        saveAsCurrentTab()
    end
end

function saveAsCurrentTab()
    local active = SelectedControlItem(tabbar)
    if active <= 0 then return end
    dialogState = "SAVE_AS"
    dialogContext = { tabIdx = active, closeAfter = false }
    CreateFileOpenDialog("Save As...", nil)
end

function saveAllTabs()
    for i = 1, ControlNumItems(tabbar) do
        if isDirty(i) and tabFilenames[i] then
            saveTab(i, tabFilenames[i])
        end
    end
end

-- Dialog state machine
local dialogState = nil
local dialogContext = {}

-- Create initial tab
newTab()

local shouldExit = false

while not ScreenShouldClose() and not KeyHit(KEY_ESC) and not shouldExit do
    while PrepareNextGUIEvent() do
        local eventType = GUIEventType()
        local eventCtrl = GUIEventControl()
        local menuId = GUIEventMenuId()

        if eventType == CONTROL_ACTION then
            if menuId ~= -1 then
                if menuId == 101 then
                    newTab()
                elseif menuId == 102 then
                    CreateFileOpenDialog("Open File", nil)
                elseif menuId == 103 then
                    saveCurrentTab()
                elseif menuId == 104 then
                    saveAsCurrentTab()
                elseif menuId == 105 then
                    saveAllTabs()
                elseif menuId == 106 then
                    local active = SelectedControlItem(tabbar)
                    if active > 0 and isDirty(active) then
                        dialogState = "CLOSE_PROMPT"
                        dialogContext = { tabIdx = active }
                        CreateMessageBox("ColdSteel", "Save changes?", EMBF_YES + EMBF_NO + EMBF_CANCEL)
                    elseif active > 0 then
                        performClose(active)
                    end
                elseif menuId == 107 then
                    closeAllTabs()
                elseif menuId == 110 then
                    shouldExit = true
                end
            else
                if ControlType(eventCtrl) == CONTROL_TABBAR then
                    focusActiveTab()
                elseif ControlType(eventCtrl) == CONTROL_BUTTON then
                    local btnId = ControlId(eventCtrl)
                    if btnId == 101 then
                        newTab()
                    elseif btnId == 102 then
                        CreateFileOpenDialog("Open File", nil)
                    elseif btnId == 103 then
                        saveCurrentTab()
                    elseif btnId == 104 then
                        local active = SelectedControlItem(tabbar)
                        if active > 0 and isDirty(active) then
                            dialogState = "CLOSE_PROMPT"
                            dialogContext = { tabIdx = active }
                            CreateMessageBox("ColdSteel", "Save changes?", EMBF_YES + EMBF_NO + EMBF_CANCEL)
                        elseif active > 0 then
                            performClose(active)
                        end
                    end
                    local parent = ControlParent(eventCtrl)
                    if parent then
                        local parentTitle = ControlText(parent)
                        if parentTitle == "Save As..." then
                            local editBox = ControlChild(parent, 8)
                            if editBox then
                                local editText = ControlText(editBox)
                                local cwd = CurrentDir()
                                if editText and editText ~= "" and editText ~= cwd then
                                    local fullPath = editText
                                    if editText:sub(1, 1) ~= "/" then
                                        fullPath = cwd .. "/" .. editText
                                    end
                                    local tabIdx = dialogContext.tabIdx
                                    if not tabIdx or tabIdx <= 0 then return end
                                    local f = io.open(fullPath, "r")
                                    if f then
                                        f:close()
                                        dialogState = "OVERWRITE"
                                        dialogContext.filename = fullPath
                                        CreateMessageBox("Save As", "File exists. Overwrite?", EMBF_YES + EMBF_NO)
                                    else
                                        saveTab(tabIdx, fullPath)
                                        if dialogContext.closeAfter then
                                            performClose(tabIdx)
                                        end
                                        dialogState = nil
                                    end
                                    FreeControl(parent)
                                end
                            end
                        end
                    end
                end
            end
        elseif eventType == CONTROL_FILESELECTED then
            local path = DialogResult()
            if path and path ~= "" then
                local dialogTitle = ControlText(eventCtrl)
                if dialogTitle == "Save As..." then
                    local tabIdx = dialogContext.tabIdx
                    local f = io.open(path, "r")
                    if f then
                        f:close()
                        dialogState = "OVERWRITE"
                        dialogContext.filename = path
                        CreateMessageBox("Save As", "File exists. Overwrite?", EMBF_YES + EMBF_NO)
                    else
                        saveTab(tabIdx, path)
                        if dialogContext.closeAfter then
                            performClose(tabIdx)
                        end
                        dialogState = nil
                    end
                else
                    local f = io.open(path, "r")
                    if f then
                        local content = f:read("*all")
                        f:close()
                        newTab(path, content)
                    end
                end
            end
        elseif eventType == CONTROL_FILECANCELLED then
            dialogState = nil
        elseif eventType == CONTROL_MESSAGEBOX_YES then
            if dialogState == "OVERWRITE" then
                local tabIdx = dialogContext.tabIdx
                local filename = dialogContext.filename
                saveTab(tabIdx, filename)
                if dialogContext.closeAfter then
                    performClose(tabIdx)
                end
                dialogState = nil
            elseif dialogState == "CLOSE_PROMPT" then
                local tabIdx = dialogContext.tabIdx
                local filename = tabFilenames[tabIdx]
                if filename then
                    saveTab(tabIdx, filename)
                    performClose(tabIdx)
                else
                    dialogState = "SAVE_AS"
                    dialogContext = { tabIdx = tabIdx, closeAfter = true }
                    CreateFileOpenDialog("Save As...", nil)
                end
                dialogState = nil
            end
        elseif eventType == CONTROL_MESSAGEBOX_NO then
            if dialogState == "OVERWRITE" then
                dialogState = "SAVE_AS"
                CreateFileOpenDialog("Save As...", nil)
            elseif dialogState == "CLOSE_PROMPT" then
                performClose(dialogContext.tabIdx)
                dialogState = nil
            end
        elseif eventType == CONTROL_MESSAGEBOX_CANCEL then
            if dialogState ~= nil then
                dialogState = nil
            end
        end
    end

    if KeyHit(KEY_TAB) then
        local active = SelectedControlItem(tabbar)
        if active > 0 then
            local editBox = ControlChild(ControlChild(tabbar, active), 1)
            if editBox then
                SetFocusedControl(editBox)
                local text = ControlText(editBox)
                local pos = EditBoxCursorPos(editBox)
                local newText = text:sub(1, pos) .. "    " .. text:sub(pos + 1)
                SetControlText(editBox, newText)
                SetEditBoxCursorPos(editBox, pos + 4)
            end
        end
    end

    resizeTabs()
    ClearScreen(RGB(40, 40, 40))
    DrawGUI()
    RefreshScreen()
end
