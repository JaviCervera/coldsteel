-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
SetAmbient(RGB(0, 0, 0))
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Create cube
local boxTex = LoadTexture("box.png")
local mesh = CreateCube()
local cube = CreateMeshEntity(mesh)
local mat = EntityMaterial(cube, 0)
SetMaterialTexture(mat, 0, boxTex)
FreeMesh(mesh)
--FreeTexture(boxTex)

function Loop()
    TurnEntity(cube, 0, 64 * DeltaTime(), 0)

    BeginDrawing(true, true, RGB(15, 15, 15))
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(255, 255, 255))
    EndDrawing()
end
