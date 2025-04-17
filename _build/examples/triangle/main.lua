OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_WHITE)

local cam = CreateCamera()
SetEntityPosition(cam, 0, 0, -2)

local mesh = CreateMesh()
local surf = AddSurface(mesh)
AddVertex(surf, 0, 0.5, 0, 0, 0, -1, COLOR_RED, 0, 0)
AddVertex(surf, 0.5, -0.5, 0, 0, 0, -1, COLOR_GREEN, 0, 0)
AddVertex(surf, -0.5, -0.5, 0, 0, 0, -1, COLOR_BLUE, 0, 0)
AddIndex(surf, 1)
AddIndex(surf, 2)
AddIndex(surf, 3)
UpdateMesh(mesh)

local triangle = CreateModel(mesh)
FreeMesh(mesh)
SetMaterialCullingEnabled(EntityMaterial(triangle, 1), false)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(triangle, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
