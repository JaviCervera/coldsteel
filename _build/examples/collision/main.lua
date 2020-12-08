-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
SetAmbient(RGB(40, 40, 40))
local light = CreateLight(LIGHT_POINT)
SetEntityPosition(light, 2, 2, -2)

-- Create cube
local mesh = CreateCube()
local cube = CreateMeshEntity(mesh)
SetEntityPosition(cube, 0, 0, 2)
SetEntityCollision(cube, COLLISION_BOX, 1)
FreeMesh(mesh)

-- Create cone
mesh = CreateCone(8)
RotateMesh(mesh, 90, 0, 0)
local cone = CreateMeshEntity(mesh)
SetEntityPosition(cone, 0, 0, -1)
FreeMesh(mesh)

function Loop()
    if KeyDown(KEY_LEFT) then TurnEntity(cone, 0, -32 * DeltaTime(), 0) end
    if KeyDown(KEY_RIGHT) then TurnEntity(cone, 0, 32 * DeltaTime(), 0) end
    if KeyDown(KEY_UP) then SlideEntity(cone, 0, 0, DeltaTime(), 1, 1, 1, 1) end
    if KeyDown(KEY_DOWN) then SlideEntity(cone, 0, 0, -DeltaTime(), 1, 1, 1, 1) end

    BeginDrawing(true, true, RGB(15, 15, 15))
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(255, 255, 255))
    EndDrawing()
end
