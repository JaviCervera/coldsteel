OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Camera
local root = CreateEntity()
local cam = CreateCamera()
SetEntityParent(cam, root)
SetEntityRotation(cam, 30, 0, 0)
SetEntityPosition(root, 500, 70, 200)

-- Load terrain
--[[
local heightmap = LoadPixmap("terrain-heightmap.bmp")
local pixmap = LoadPixmap("terrain-texture.jpg")
local mesh = LoadTerrainMesh(heightmap, pixmap, 1000, 50, 1000)
FreePixmap(heightmap)
FreePixmap(pixmap)
local terrain = CreateMeshEntity(mesh)
]]--
local terrain = LoadTerrain("terrain-heightmap.bmp", 1000, 50, 1000)
ScaleTerrainTexture(terrain, 1, 16)
local mat = EntityMaterial(terrain, 1)
SetMaterialType(mat, MATERIAL_DETAIL)
SetMaterialTexture(mat, 1, LoadTexture("terrain-texture.jpg"))
SetMaterialTexture(mat, 2, LoadTexture("detailmap3.jpg"))

while ScreenActive() and not KeyDown(KEY_ESC) do
    if KeyDown(KEY_ESC) then Exit(0) end
    if KeyDown(KEY_UP) then MoveEntity(root, 0, 0, 100 * DeltaTime()) end
    if KeyDown(KEY_DOWN) then MoveEntity(root, 0, 0, -100 * DeltaTime()) end
    if KeyDown(KEY_LEFT) then TurnEntity(root, 0, -90 * DeltaTime(), 0) end
    if KeyDown(KEY_RIGHT) then TurnEntity(root, 0, 90 * DeltaTime(), 0) end
    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
