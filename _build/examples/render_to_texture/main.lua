OpenScreen(400, 400, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_WHITE)

local tex = CreateRenderTexture(128, 128)

local cam = CreateCamera()
SetCameraRenderTarget(cam, tex)
SetEntityPosition(cam, 0, 0, -2)

local mesh = CreateMesh()
local surf = AddSurface(mesh)
AddVertex(surf, 0, 0.5, 0, 0, 0, -1, COLOR_CYAN, 0, 0)
AddVertex(surf, 0.5, -0.5, 0, 0, 0, -1, COLOR_YELLOW, 0, 0)
AddVertex(surf, -0.5, -0.5, 0, 0, 0, -1, COLOR_MAGENTA, 0, 0)
AddIndex(surf, 1)
AddIndex(surf, 2)
AddIndex(surf, 3)
UpdateMesh(mesh)

local triangle = CreateModel(mesh)
FreeMesh(mesh)
SetMaterialFlag(EntityMaterial(triangle, 1), FLAG_BACKFACECULLING, false)
SetMaterialFlag(EntityMaterial(triangle, 1), FLAG_VERTEXCOLORS, true)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(triangle, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    SetRenderTarget(tex, false, false, 0)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    SetRenderTarget(nil, false, false, 0)
    DrawTextureEx(tex, 0, 0, ScreenWidth(), ScreenHeight(), COLOR_WHITE)
    RefreshScreen()
end
