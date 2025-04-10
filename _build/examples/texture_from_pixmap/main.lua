OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local pixmap = CreatePixmap(128, 128)
for y = 0, PixmapHeight(pixmap) - 1 do
    for x = 0, PixmapWidth(pixmap) - 1 do
        WritePixel(pixmap, x, y, RGB(Rand(0, 255), Rand(0, 255), Rand(0, 255)))
    end
end

local tex = CreateTextureFromPixmap(pixmap)
FreePixmap(pixmap)

local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 45, -45, 0)

local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

local mesh = CreateCubeMesh()
local cube = CreateModel(mesh)
SetMaterialTexture(EntityMaterial(cube, 1), 1, tex)
FreeMesh(mesh)

while not ScreenShouldClose() and not KeyDown(KEY_ESC) do
    TurnEntity(cube, 30 * DeltaTime(), 60 * DeltaTime(), 0)

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
