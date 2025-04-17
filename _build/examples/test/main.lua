OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local tex = LoadTexture("space_station.jpg")
SetSkydome(tex, 1, true)
local cam = CreateCamera()
SetCameraClearMode(cam, false, true)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cam, 0, 8 * DeltaTime(), 0)

    DrawWorld()
    DrawText(nil, CurrentDir(), 4, 4, COLOR_YELLOW)
    RefreshScreen()
end
