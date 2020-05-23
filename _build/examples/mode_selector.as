class ScreenSettings {
	int Width;
	int Height;
	int Depth;
	int Flags;
	
	bool Valid() const {
		return Width != 0 && Height != 0 && Depth != 0;
	}
}


void Main() {
	const ScreenSettings settings = ShowScreenSettingsDialog();
	if (settings.Valid()) {
		OpenScreen(settings.Width, settings.Height, settings.Depth, settings.Flags);
		while (Run() && !KeyDown(KEY_ESC)) {
			BeginDrawing(true, false, RGB(210, 210, 210));
			EndDrawing();
		}
	}
}


ScreenSettings ShowScreenSettingsDialog() {
	OpenScreen(400, 190, DesktopDepth(), SCREEN_WINDOWED);
	SetScreenTitle("Select video settings");
	if (!AddZip("assets.dat")) ChangeDir("assets");
	
	Font@ font = LoadFont("tahoma.xml");
	SetControlFont(null, font);
	
	const int margin = 4;
	const int width = ScreenWidth() - margin*2;
	const int textHeight = 16;
	const int comboHeight = 24;
	
	Control@ driverText = CreateText(margin, margin, width, textHeight, "Driver:", false, null);
	Control@ driverBox = CreateComboBox(margin, margin + ControlY(driverText) + ControlHeight(driverText), width, comboHeight, null);
	AddControlItem(driverBox, "OpenGL", 0);
	AddControlItem(driverBox, "Software", 0);
	
	Control@ resolutionText = CreateText(margin, margin + ControlY(driverBox) + ControlHeight(driverBox), width, textHeight, "Resolution:", false, null);
	Control@ resolutionBox = CreateComboBox(margin, margin + ControlY(resolutionText) + ControlHeight(resolutionText), width, comboHeight, null);
	for (int i = 0; i < ScreenNumModes(); ++i) {
		AddControlItem(resolutionBox, Str(ScreenModeWidth(i)) + "x" + Str(ScreenModeHeight(i)) + "x" + Str(ScreenModeDepth(i)), 0);
	}
	SelectControlItem(resolutionBox, ControlNumItems(resolutionBox) - 1);
	
	Control@ windowedCheck = CreateCheckBox(margin, margin + ControlY(resolutionBox) + ControlHeight(resolutionBox), width, comboHeight, "Windowed", null);
	Control@ vsyncCheck = CreateCheckBox(margin, margin + ControlY(windowedCheck) + ControlHeight(windowedCheck), width, comboHeight, "Vertical Sync", null);
	
	Control@ acceptButton = CreateButton(ScreenWidth() - margin - 100, margin + ControlY(vsyncCheck) + ControlHeight(vsyncCheck), 100, comboHeight * 1.25, "Accept", null);
	
	bool accepted = false;
	while (Run() && !accepted) {
		while (PrepareNextGUIEvent())
			if (GUIEventType() == CONTROL_ACTION && @GUIEventControl() == @acceptButton)
				accepted = true;
		BeginDrawing(true, false, RGB(210, 210, 210));
		DrawGUI();
		EndDrawing();
	}
	
	FreeFont(font);
	
	ScreenSettings settings;
	if (accepted) {
		settings.Width = Val(Field(ControlText(resolutionBox), "x", 0));
		settings.Height = Val(Field(ControlText(resolutionBox), "x", 1));
		settings.Depth = Val(Field(ControlText(resolutionBox), "x", 2));
		if (ControlChecked(windowedCheck)) settings.Flags |= SCREEN_WINDOWED;
		if (ControlChecked(vsyncCheck)) settings.Flags |= SCREEN_VSYNC;
		//if (SelectedControlItem(driverBox) == 1) settings.Flags |= SCREEN_SOFTWARE;
	}
	return settings;
}
