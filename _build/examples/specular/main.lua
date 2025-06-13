OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

SetAmbient(COLOR_BLACK)
local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 30, 40, 0)

local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityPosition(cam, 0, 0, -2)

local model = CreateSphereModel(64)
SetMeshColor(ModelMesh(model), COLOR_ORANGE)
local mat = EntityMaterial(model, 1)
SetMaterialShininess(mat, 0.5)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    if KeyDown(KEY_LEFT) then SetMaterialShininess(mat, MaterialShininess(mat) - 0.2 * DeltaTime()) end
    if KeyDown(KEY_RIGHT) then SetMaterialShininess(mat, MaterialShininess(mat) + 0.2 * DeltaTime()) end
    SetMaterialSpecular(mat, MultiplyColor(COLOR_WHITE, MaterialShininess(mat) * 2))

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    DrawText(nil, "Shininess: " .. MaterialShininess(mat) .. " (LEFT and RIGHT to change)", 2, 16, COLOR_WHITE)
    RefreshScreen()
end
