csOpenScreen(800, 600, 32, CSSCREEN_WINDOWED | CSSCREEN_RESIZABLE)
csSetAmbient(csRGB(255, 255, 255))

local cam = csCreateCamera()
csSetEntityPosition(cam, 0, 0, -2)
    
local mesh = csCreateMesh()
local surf = csAddSurface(mesh)
csAddVertex(surf, 0, 0.5, 0, 0, 0, -1, csRGB(255, 0, 0), 0, 0)
csAddVertex(surf, 0.5, -0.5, 0, 0, 0, -1, csRGB(0, 255, 0), 0, 0)
csAddVertex(surf, -0.5, -0.5, 0, 0, 0, -1, csRGB(0, 0, 255), 0, 0)
csAddIndex(surf, 0)
csAddIndex(surf, 1)
csAddIndex(surf, 2)
csUpdateMesh(mesh)

local triangle = csCreateMeshEntity(mesh)
csSetMaterialCullingEnabled(csEntityMaterial(triangle, 0), false)

while csRun() and not csKeyDown(CSKEY_ESC) do
    csTurnEntity(triangle, 0, 64 * csDeltaTime(), 0)

    csBeginDrawing(true, true, csRGB(255, 255, 255))
    csDrawWorld(cam)
    csDrawText(nil, csStr(csScreenFPS()) .. " FPS", 2, 2, csRGB(0, 0, 0))
    csEndDrawing()
end
