OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Create and setup camera
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_DARKGRAY)
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
SetAmbient(COLOR_BLACK)
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Create cube
local boxTex = LoadTexture("box.png")
local mesh = CreateCubeMesh()
local cube = CreateModel(mesh)
local mat = EntityMaterial(cube, 1)
SetMaterialTexture(mat, 1, boxTex)
FreeMesh(mesh)

while ScreenActive() and not KeyDown(KEY_ESC) do
    TurnEntity(cube, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
