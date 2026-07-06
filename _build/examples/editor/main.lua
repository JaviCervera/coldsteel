OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetScreenTitle("ColdSteel - <untitled>")

local liberationSans = LoadFont("segoe_ui.xml")
local fixedsys = LoadFont("fixedsys.xml")
SetControlFont(nil, liberationSans)

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

function newTab()
    local idx = AddControlItem(tabbar, "<untitled>", 0)
    local tab = ControlChild(tabbar, idx)
    local editBox = CreateEditBox(0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20, EDITBOX_MULTILINE, tab)
    SetControlFont(editBox, fixedsys)
    SelectControlItem(tabbar, idx)
    SetFocusedControl(editBox)
    return idx
end

function closeTab(index)
    if index <= 0 or index > ControlNumItems(tabbar) then return end
    RemoveControlItem(tabbar, index)
    if ControlNumItems(tabbar) == 0 then
        newTab()
    else
        local active = SelectedControlItem(tabbar)
        if active == 0 then
            active = 1
            SelectControlItem(tabbar, 1)
        end
        local editBox = ControlChild(ControlChild(tabbar, active), 1)
        SetFocusedControl(editBox)
    end
end

function closeAllTabs()
    RemoveControlItems(tabbar)
    newTab()
end

function focusActiveTab()
    local active = SelectedControlItem(tabbar)
    if active > 0 then
        local tab = ControlChild(tabbar, active)
        local editBox = ControlChild(tab, 1)
        SetFocusedControl(editBox)
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
    end
end

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
                elseif menuId == 106 then
                    closeTab(SelectedControlItem(tabbar))
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
                    elseif btnId == 104 then
                        closeTab(SelectedControlItem(tabbar))
                    end
                end
            end
        end
    end

    if KeyHit(KEY_TAB) then
        local active = SelectedControlItem(tabbar)
        if active > 0 then
            local editBox = ControlChild(ControlChild(tabbar, active), 1)
            SetFocusedControl(editBox)
            local text = ControlText(editBox)
            local pos = EditBoxCursorPos(editBox)
            local newText = text:sub(1, pos) .. "    " .. text:sub(pos + 1)
            SetControlText(editBox, newText)
            SetEditBoxCursorPos(editBox, pos + 4)
        end
    end

    resizeTabs()
    ClearScreen(RGB(210, 210, 210))
    DrawGUI()
    RefreshScreen()
end
