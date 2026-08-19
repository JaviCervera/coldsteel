OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_BLACK)

local tex1 = LoadTexture("tex.png")
local tex2 = LoadTexture("logo.png")

-- Build the CSG solid: a room carved out of solid space, then a column added inside it.
-- The column rotates its texture 45 degrees and scales it to double density to show the
-- UV modifiers; the room uses the default UV mapping (1 texel per world unit).
-- A second small box reuses tex2; both tex2 primitives merge into a single surface.
local csg = CreateCsg()
AddCsgBox(csg, CSG_SUBTRACT, tex1, 0, 0, 0, 0, 0, 0, 20, 10, 16, 0, 0, 0, 1, 1)
AddCsgBox(csg, CSG_ADD, tex2, 0, 0, 0, 0, 0, 0, 2, 10, 2, 0, 0, 45, 0.5, 0.5)
AddCsgBox(csg, CSG_ADD, tex2, 6, 0, 4, 0, 0, 0, 2, 2, 2, 0, 0, 0, 1, 1)
local mesh = CsgMesh(csg)
local room = CreateModel(mesh)
FreeMesh(mesh)
FreeCsg(csg)

-- Disable lighting on all surfaces so the scene is visible purely through the texturing.
local mats = EntityNumMaterials(room)
for i = 1, mats do
    SetMaterialFlag(EntityMaterial(room, i), FLAG_LIGHTING, false)
end

-- Camera outside the room, looking in through the culled front wall.
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityPosition(cam, 0, 1.5, -10)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(room, 0, 30 * DeltaTime(), 0)
    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    DrawText(nil, "Surfaces: " .. Str(mats), 2, 22, COLOR_WHITE)
    RefreshScreen()
end
