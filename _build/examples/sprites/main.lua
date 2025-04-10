OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_WHITE)

local tex = LoadTexture("logo.png")
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityRotation(cam, 30, 0, 0)

for z = -8, 8, 2 do
    for x = -8, 8, 2 do
        local tree = CreateSprite(tex, MATERIAL_ALPHA)
        SetEntityPosition(tree, x, 0, z)
        SetSpriteSize(tree, 2, 2)
    end
end

while not ScreenShouldClose() and not KeyDown(KEY_ESC) do
    TurnEntity(cam, 0, 15 * DeltaTime(), 0)
    SetEntityPosition(cam, 0, 0, 0)
    MoveEntity(cam, 0, 0, -8)

    DrawWorld()
    RefreshScreen()
end
