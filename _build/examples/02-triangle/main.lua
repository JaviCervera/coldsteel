SetAmbient(RGB(255, 255, 255))

local cam = CreateCamera()
SetEntityPosition(cam, 0, 0, -2)
    
local mesh = CreateMesh()
local surf = AddSurface(mesh)
AddVertex(surf, 0, 0.5, 0, 0, 0, -1, RGB(255, 0, 0), 0, 0)
AddVertex(surf, 0.5, -0.5, 0, 0, 0, -1, RGB(0, 255, 0), 0, 0)
AddVertex(surf, -0.5, -0.5, 0, 0, 0, -1, RGB(0, 0, 255), 0, 0)
AddIndex(surf, 0)
AddIndex(surf, 1)
AddIndex(surf, 2)
UpdateMesh(mesh)

local triangle = CreateMeshEntity(mesh)
SetMaterialCullingEnabled(EntityMaterial(triangle, 0), false)

function Loop()
    TurnEntity(triangle, 0, 64 * DeltaTime(), 0)

    BeginDrawing(true, true, RGB(255, 255, 255))
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(0, 0, 0))
    EndDrawing()
end
