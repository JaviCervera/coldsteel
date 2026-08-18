OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_BLACK)

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
local walls = {}
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
    walls[#walls + 1] = wall
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
SetLightDiffuse(lamp, COLOR_RED)
SetLightRadius(lamp, 48)

-- Bake vertex lighting first (fast), then the texture lightmap. Baking the vertex lightmap before
-- the texture lightmap keeps both the vertex colors and the lightmap UVs on the meshes.
BakeVertexLightmap(nil, 3, true)
local atlas = BakeLightmap(nil, 8, 1024, 3, true)
if atlas ~= nil then
    --SavePixmap(atlas, "lightmap.png")
    LogInfo("Baked lightmap atlas " .. PixmapWidth(atlas) .. "x" .. PixmapHeight(atlas))
    FreePixmap(atlas)
else
    LogInfo("Lightmap bake failed!")
end

-- Toggle between the vertex-colored lightmap and the texture lightmap. The texture lightmap
-- requires FLAG_VERTEXCOLORS to be disabled on the materials so the baked vertex colors are ignored.
local entities = { floor, box }
for i, w in ipairs(walls) do entities[#entities + 1] = w end
local showVertex = false

local function SetLightmapMode(vertexMode, entity)
    for m = 1, EntityNumMaterials(entity) do
        local mat = EntityMaterial(entity, m)
        if vertexMode then
            SetMaterialType(mat, MATERIAL_SOLID)
            SetMaterialFlag(mat, FLAG_VERTEXCOLORS, true)
        else
            SetMaterialType(mat, MATERIAL_LIGHTMAP)
            SetMaterialFlag(mat, FLAG_VERTEXCOLORS, false)
        end
    end
end

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    if KeyHit(KEY_SPACE) then
        showVertex = not showVertex
        for i, e in ipairs(entities) do
            SetLightmapMode(showVertex, e)
        end
    end
    TurnEntity(cam, 0, 45 * DeltaTime(), 0)
    DrawWorld()
    local mode = showVertex and "Vertex lightmap [SPACE]" or "Texture lightmap [SPACE]"
    DrawText(nil, Str(ScreenFPS()) .. " FPS - " .. mode, 2, 2, COLOR_WHITE)
    RefreshScreen()
end
