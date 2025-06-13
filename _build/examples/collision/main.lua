OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 45, -45, 0)

-- Create cube
local cube = CreateCubeModel()
SetEntityPosition(cube, 0, 0, 2)
SetEntityCollision(cube, COLLISION_BOX, 1)

-- Create cone
local cone = CreateConeModel(8)
RotateMesh(ModelMesh(cone), 90, 0, 0)
SetEntityPosition(cone, 0, 0, -1)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    if KeyDown(KEY_LEFT) then TurnEntity(cone, 0, -32 * DeltaTime(), 0) end
    if KeyDown(KEY_RIGHT) then TurnEntity(cone, 0, 32 * DeltaTime(), 0) end
    if KeyDown(KEY_UP) then SlideEntity(cone, 0, 0, DeltaTime(), 1, 1, 1, 1) end
    if KeyDown(KEY_DOWN) then SlideEntity(cone, 0, 0, -DeltaTime(), 1, 1, 1, 1) end

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
