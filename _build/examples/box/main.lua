OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local font = LoadFont("greenscr_12.xml")

-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
SetAmbient(COLOR_BLACK)
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Create cube
local cube = CreateCubeModel()
local mat = EntityMaterial(cube, 1)
SetMaterialTexture(mat, 1, LoadTexture("tex.png"))

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cube, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    DrawText(font, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end

