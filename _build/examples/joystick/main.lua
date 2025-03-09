OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

function Loop()
	BeginDrawing(true, false, COLOR_BLACK)
	if NumJoysticks() > 0 then
		DrawText(nil, "Name: " .. JoystickName(0), 4, 4, COLOR_WHITE)
		DrawText(nil, "Num Joystick Axes: " .. Str(NumJoystickAxes(0)), 4, 16, COLOR_WHITE)
		DrawText(nil, "Num Joystick Buttons: " .. Str(NumJoystickButtons(0)), 4, 28, COLOR_WHITE)
		for i = 0, NumJoystickAxes(0) - 1 do
			DrawText(nil, "Axis " .. Str(i) .. ": " .. StrF(JoystickAxis(0, i)), 4, 40 + i*12, COLOR_WHITE)
		end
		for i = 0, NumJoystickButtons(0) - 1 do
			local pressed = " not pressed."
			if JoystickButtonDown(0, i) then pressed = " pressed." end
			DrawText(nil, "Button " .. Str(i) .. pressed, 4, 40 + (NumJoystickAxes(0) + i)*12, COLOR_WHITE)
		end
	else
		DrawText(nil, "No joysticks connected", 4, 4, COLOR_RED)
	end
	EndDrawing()
end
