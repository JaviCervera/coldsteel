OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(RGB(75, 75, 75))

local cam = CreateCamera()
SetCameraClearMode(cam, false, true)
SetEntityPosition(cam, -150, 150, -200)
SetEntityRotation(cam, 30, 30, 0)

local light = CreateLight(LIGHT_POINT)
SetLightDiffuse(light, RGB(255, 155, 180))
SetLightRadius(light, 800)

local texture = LoadTexture("wall.jpg")
local room = LoadModel("room.3ds")
for i = 1, EntityNumMaterials(room) do
	local mat = EntityMaterial(room, i)
	SetMaterialSpecular(mat, COLOR_BLACK)
	SetMaterialTexture(mat, 1, texture)
end

local dwarf = LoadModel("dwarf.x")
SetEntityPosition(dwarf, -50, 20, -60)
SetEntityScale(dwarf, 2, 2, 2)
SetModelCastShadows(dwarf, true)

while ScreenActive() and not KeyDown(KEY_ESC) do
	TurnEntity(light, 0, 90 * DeltaTime(), 0)
	SetEntityPosition(light, 0, 150, 0)
	MoveEntity(light, 0, 0, -250)
	DrawWorld()
	RefreshScreen()
end
