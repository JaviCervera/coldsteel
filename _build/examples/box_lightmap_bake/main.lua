OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_DARKGRAY)

local tex = LoadTexture("tex.png")

-- Camera is inside the room
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityPosition(cam, 0, 2, 0)

-- Floor
local floor = CreateCubeModel()
SetEntityScale(floor, 20, 0.2, 20)
SetEntityPosition(floor, 0, 0, 0)
SetMaterialTexture(EntityMaterial(floor, 1), 1, tex)

-- Walls
for i, pos in ipairs({
    { 0, 5, -10, 20, 10, 0.2 },
    { 0, 5, 10, 20, 10, 0.2 },
    { -10, 5, 0, 0.2, 10, 20 },
    { 10, 5, 0, 0.2, 10, 20 }
}) do
    local wall = CreateCubeModel()
    SetEntityScale(wall, pos[4], pos[5], pos[6])
    SetEntityPosition(wall, pos[1], pos[2], pos[3])
    SetMaterialTexture(EntityMaterial(wall, 1), 1, tex)
end

-- Occluder casting a shadow on the floor
local box = CreateCubeModel()
SetEntityScale(box, 2, 2, 2)
SetEntityPosition(box, 2, 1.2, 2)
SetMaterialTexture(EntityMaterial(box, 1), 1, tex)

-- Lights
local sun = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(sun, 35, -35, 0)
SetLightDiffuse(sun, COLOR_WHITE)

local lamp = CreateLight(LIGHT_POINT)
SetEntityPosition(lamp, -3, 4, -2)
SetLightDiffuse(lamp, COLOR_YELLOW)
SetLightRadius(lamp, 8)
SetLightAttenuation(lamp, 1, 0.05, 0.02)

-- Bake direct lighting into a single lightmap atlas
local atlas = BakeLightmaps(nil, 8, 2048)
if atlas ~= nil then
    --SavePixmap(atlas, "lightmap_bake.png")
    LogInfo("Baked lightmap atlas " .. PixmapWidth(atlas) .. "x" .. PixmapHeight(atlas))
    FreePixmap(atlas)
else
    LogInfo("Lightmap bake failed!")
end

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cam, 0, 45 * DeltaTime(), 0)
    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_BLACK)
    RefreshScreen()
end
