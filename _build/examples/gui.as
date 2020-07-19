void Main() {
	OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
	SetScreenTitle("Astro - <untitled>");
	if (!AddZip("assets.dat")) ChangeDir("assets");
	
	Font@ liberationSans = LoadFont("segoe_ui.xml");
	Font@ fixedsys = LoadFont("fixedsys.xml");
	SetControlFont(null, liberationSans);
	
	const array<Texture@> icons = {
		LoadTexture("toolbar/new.png"),
		LoadTexture("toolbar/open.png"),
		LoadTexture("toolbar/save.png"),
		LoadTexture("toolbar/close.png")
	};

	// Create menu
	Control@ mainMenu = CreateMenu("", null);
	Control@ fileMenu = CreateMenu("File", mainMenu);
	AddControlItem(fileMenu, "New", 101);
	AddControlItem(fileMenu, "Open", 102);
	AddControlItem(fileMenu, "Save", 103);
	AddControlItem(fileMenu, "Save As...", 104);
	AddControlItem(fileMenu, "Save All", 105);
	AddMenuSeparator(fileMenu);
	AddControlItem(fileMenu, "Close", 106);
	AddControlItem(fileMenu, "Close All", 107);
	AddMenuSeparator(fileMenu);
	AddControlItem(fileMenu, "Next File", 108);
	AddControlItem(fileMenu, "Previous File", 109);
	AddMenuSeparator(fileMenu);
	AddControlItem(fileMenu, "Exit", 110);
	Control@ editMenu = CreateMenu("Edit", mainMenu);
	Control@ programMenu = CreateMenu("Program", mainMenu);
	Control@ helpMenu = CreateMenu("Help", mainMenu);
	
	// Create toolbar
	Control@ toolbar = CreateToolbar(null);
	for (int i = 0; i < icons.Len(); ++i) {
		AddControlItem(toolbar, "   ", 101 + i);
		SetControlTexture(ControlChild(toolbar, i), icons[i], true);
	}
	
	const int headerHeight = ControlHeight(mainMenu) + ControlHeight(toolbar);
	
	// Create tabbar
	Control@ tabbar = CreateTabbar(0, headerHeight, ScreenWidth(), ScreenHeight() - headerHeight - 20, null);
	AddControlItem(tabbar, "<untitled>", 0);
	SetControlFont(CreateEditBox(0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20, EDITBOX_MULTILINE, ControlChild(tabbar, 0)), fixedsys);
	
	while (Run() && !KeyDown(KEY_ESC)) {
		SetControlShape(tabbar, 0, headerHeight, ScreenWidth(), ScreenHeight() - headerHeight - 20);
		for (int i = 0; i < ControlNumChildren(tabbar); ++i) {
			SetControlShape(ControlChild(ControlChild(tabbar, i), 0), 0, 0, ScreenWidth(), ScreenHeight() - headerHeight - 20);
		}
		BeginDrawing(true, true, RGB(210, 210, 210));
		DrawGUI();
		EndDrawing();
	}
}
