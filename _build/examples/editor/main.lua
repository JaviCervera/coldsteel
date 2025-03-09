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
for i = 0, #icons - 1 do
	AddControlItem(toolbar, "   ", 101 + i)
	SetControlTexture(ControlChild(toolbar, i), icons[i+1], true)
end

local headerHeight = ControlHeight(mainMenu) + ControlHeight(toolbar)

-- Create tabbar
local tabbar = CreateTabbar(0, headerHeight, ScreenWidth(), ScreenHeight() - headerHeight - 20, nil)
AddControlItem(tabbar, "<untitled>", 0)
SetControlFont(CreateEditBox(0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20, EDITBOX_MULTILINE, ControlChild(tabbar, 0)), fixedsys)

function Loop()
	SetControlShape(tabbar, 0, headerHeight, ScreenWidth(), ScreenHeight() - headerHeight - 20)
	for i = 0, ControlNumChildren(tabbar) -1 do
		SetControlShape(ControlChild(ControlChild(tabbar, i), 0), 0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20)
	end
	BeginDrawing(true, true, RGB(210, 210, 210))
	DrawGUI()
	EndDrawing()
end
