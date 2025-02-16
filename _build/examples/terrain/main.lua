-- Camera
local root = CreateEntity()
local cam = CreateCamera()
SetEntityParent(cam, root)
SetEntityRotation(cam, 30, 0, 0)
SetEntityPosition(root, 500, 70, 200)

-- Load terrain
local terrain = LoadTerrain("terrain-heightmap.bmp", nil, 1000, 50, 1000)
local mat = EntityMaterial(terrain, 0)
SetMaterialType(mat, MATERIAL_DETAIL)
SetMaterialTexture(mat, 0, LoadTexture("terrain-texture.jpg"))
SetMaterialTexture(mat, 1, LoadTexture("detailmap3.jpg"))

function Loop()
    if KeyDown(KEY_ESC) then Exit(0) end
    if KeyDown(KEY_UP) then MoveEntity(root, 0, 0, 100 * DeltaTime()) end
    if KeyDown(KEY_DOWN) then MoveEntity(root, 0, 0, -100 * DeltaTime()) end
    if KeyDown(KEY_LEFT) then TurnEntity(root, 0, -90 * DeltaTime(), 0) end
    if KeyDown(KEY_RIGHT) then TurnEntity(root, 0, 90 * DeltaTime(), 0) end
	BeginDrawing(true, true, RGB(55, 155, 255))
	DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(255, 255, 255))
	EndDrawing()
end
