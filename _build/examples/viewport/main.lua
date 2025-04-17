OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Setup lighting
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Create first camera
local cam = CreateCamera()
SetCameraClearColor(cam, RGB(128, 0, 0))
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Create second camera
local cam2 = CreateCamera()
SetCameraClearColor(cam2, RGB(0, 128, 0))
SetEntityPosition(cam2, 0, 2, -2)
SetEntityRotation(cam2, 45, 0, 0)

-- Create cube
local mesh = CreateCubeMesh()
local cube = CreateModel(mesh)
FreeMesh(mesh)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cube, 32 * DeltaTime(), 64 * DeltaTime(), 0)

    SetCameraViewport(cam, ScreenWidth() / 2, 0, ScreenWidth() / 2, ScreenHeight() / 2)
    SetCameraViewport(cam2, 0, ScreenHeight() / 2, ScreenWidth() / 2, ScreenHeight() / 2)
    ClearScreen(RGB(0, 0, 128))
    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
