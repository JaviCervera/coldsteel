OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local font = LoadFont("greenscr_12.xml")

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
	ClearScreen(COLOR_BLACK)
	if NumJoysticks() > 0 then
		DrawText(font, "Name: " .. JoystickName(1), 4, 4, COLOR_WHITE)
		DrawText(font, "Num Joystick Axes: " .. Str(NumJoystickAxes(1)), 4, 16, COLOR_WHITE)
		DrawText(font, "Num Joystick Buttons: " .. Str(NumJoystickButtons(1)), 4, 28, COLOR_WHITE)
		for i = 0, NumJoystickAxes(1) - 1 do
			DrawText(font, "Axis " .. Str(i) .. ": " .. StrF(JoystickAxis(1, i)), 4, 40 + i*12, COLOR_WHITE)
		end
		for i = 0, NumJoystickButtons(1) - 1 do
			local pressed = " not pressed."
			if JoystickButtonDown(1, i) then pressed = " pressed." end
			DrawText(font, "Button " .. Str(i) .. pressed, 4, 40 + (NumJoystickAxes(1) + i)*12, COLOR_WHITE)
		end
	else
		DrawText(font, "No joysticks connected", 4, 4, COLOR_RED)
	end
	RefreshScreen()
end


