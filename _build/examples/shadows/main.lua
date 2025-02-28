SetAmbient(RGB(75, 75, 75))

local cam = CreateCamera()
SetEntityPosition(cam, -150, 150, -200)
SetEntityRotation(cam, 30, 30, 0)

local light = CreateLight(LIGHT_POINT)
SetLightDiffuse(light, RGB(255, 155, 180))
SetLightRadius(light, 800)

local texture = LoadTexture("wall.jpg")
local room = CreateMeshEntity(LoadMesh("room.3ds"))
for i = 0, EntityNumMaterials(room)-1 do
	local mat = EntityMaterial(room, i)
	SetMaterialSpecular(mat, COLOR_BLACK)
	SetMaterialTexture(mat, 0, texture)
end

local dwarf = CreateMeshEntity(LoadMesh("dwarf.x"))
SetEntityPosition(dwarf, -50, 20, -60)
SetEntityScale(dwarf, 2, 2, 2)
SetMeshEntityCastShadows(dwarf, true)

function Loop()
	TurnEntity(light, 0, 90 * DeltaTime(), 0)
	SetEntityPosition(light, 0, 150, 0)
	MoveEntity(light, 0, 0, -250)
	BeginDrawing(true, true, RGB(55, 155, 255))
	DrawWorld(cam)
	EndDrawing()
end
