OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Create and setup camera
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Create cube
local boxTex = LoadTexture("box.png")
local lightmapTex = LoadTexture("lightmap.png")
local mesh = CreateCubeMesh()
local cube = CreateModel(mesh)
local mat = EntityMaterial(cube, 1)
SetMaterialType(mat, MATERIAL_LIGHTMAP)
SetMaterialTexture(mat, 1, boxTex)
SetMaterialTexture(mat, 2, lightmapTex)
FreeMesh(mesh)

-- Create light (it can darken the lightmap, but not brighten it)
local light = CreateLight(LIGHT_POINT)
SetEntityPosition(light, 2, 2, -2)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cube, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
