OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_BLACK)

-- Build the CSG solid: carve a rectangular room out of solid space, carve a hallway at
-- the far end, then add a column in the middle of the room. Save the result as a model.
local csg = CreateCsg()
AddCsgBox(csg, CSG_SUBTRACT, 0, 0, 0, 0, 0, 0, 20, 10, 16)
AddCsgBox(csg, CSG_SUBTRACT, 0, 0, -12, 0, 0, 0, 6, 10, 8)
AddCsgBox(csg, CSG_ADD, 0, 0, 0, 0, 0, 0, 2, 10, 2)
local mesh = CsgMesh(csg)
local room = CreateModel(mesh)
--SetMaterialFlag(EntityMaterial(room, 1), FLAG_LIGHTING, false)
FreeMesh(mesh)
FreeCsg(csg)

-- Lights: a white light on the ceiling, offset towards the far end so the column casts its
-- shadow towards the camera, and a blue light inside the hallway.
local white = CreateLight(LIGHT_POINT)
SetEntityPosition(white, 0, 4.8, -3.5)
SetLightDiffuse(white, COLOR_WHITE)
SetLightRadius(white, 30)
SetLightCastShadows(white, true)

local blue = CreateLight(LIGHT_POINT)
SetEntityPosition(blue, 0, 0, -12)
SetLightDiffuse(blue, COLOR_BLUE)
SetLightRadius(blue, 14)

-- Bake the lightmap: every static mesh under the root (NULL = whole scene) receives lightmap
-- UVs and a lightmapped material. The returned atlas is a pixmap which must be freed.
local atlas = BakeLightmap(nil, 8, 1024, 0, false)
if atlas ~= nil then
    LogInfo("Baked CSG lightmap atlas " .. PixmapWidth(atlas) .. "x" .. PixmapHeight(atlas))
    FreePixmap(atlas)
else
    LogInfo("CSG lightmap bake failed!")
end
SetAmbient(COLOR_WHITE)

-- Camera outside the room, looking in through the culled front wall. The white light sits
-- between the column and the far end, so the column's shadow stretches towards the camera.
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityPosition(cam, 0, 1.5, 20)
SetEntityRotation(cam, 0, 180, 0)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(room, 0, 30 * DeltaTime(), 0)
    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS - CSG room", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
