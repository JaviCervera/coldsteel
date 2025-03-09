OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
SetAmbient(COLOR_BLACK)
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Create cube
local boxTex = LoadTexture("box.png")
local mesh = CreateCube()
local cube = CreateMeshEntity(mesh)
local mat = EntityMaterial(cube, 1)
SetMaterialTexture(mat, 1, boxTex)
FreeMesh(mesh)

function Loop()
    TurnEntity(cube, 0, 64 * DeltaTime(), 0)

    BeginDrawing(true, true, COLOR_DARKGRAY)
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    EndDrawing()
end
