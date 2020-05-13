void Main() {
	const int COLOR_WHITE = RGB(255, 255, 255);
	
	OpenScreen(800, 600, 32, SCREEN_WINDOWED);
	
	while (Run() && !KeyDown(KEY_ESC)) {
		BeginDrawing(true, false, RGB(0, 0, 0));
		if (NumJoysticks() > 0) {
			DrawText(null, "Name: " + JoystickName(0), 4, 4, COLOR_WHITE);
			DrawText(null, "Num Joystick Axes: " + Str(NumJoystickAxes(0)), 4, 16, COLOR_WHITE);
			DrawText(null, "Num Joystick Buttons: " + Str(NumJoystickButtons(0)), 4, 28, COLOR_WHITE);
			for (int i = 0; i < NumJoystickAxes(0); ++i) {
				DrawText(null, "Axis " + Str(i) + ": " + StrF(JoystickAxis(0, i)), 4, 40 + i*12, COLOR_WHITE);
			}
			for (int i = 0; i < NumJoystickButtons(0); ++i) {
				DrawText(null, "Button " + Str(i) + (JoystickButtonDown(0, i) ? "" : " not") + " pressed.", 4, 40 + (NumJoystickAxes(0) + i)*12, COLOR_WHITE);
			}
		} else {
			DrawText(null, "No joysticks connected", 4, 4, RGB(255, 0, 0));
		}
		EndDrawing();
	}
}
