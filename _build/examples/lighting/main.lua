OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 32, -90)
SetEntityRotation(cam, 16, 0, 0)

-- Setup lighting
SetAmbient(COLOR_BLACK)
local lights = {
    CreateLight(LIGHT_POINT),
    CreateLight(LIGHT_POINT),
    CreateLight(LIGHT_POINT)
}
SetLightAttenuation(lights[1], 1, 0.05, 0)
SetLightDiffuse(lights[1], COLOR_RED)
SetEntityRotation(lights[1], 0, 0, 0)
SetLightAttenuation(lights[2], 1, 0.05, 0)
SetLightDiffuse(lights[2], COLOR_GREEN)
SetEntityRotation(lights[2], 0, 120, 0)
SetLightAttenuation(lights[3], 1, 0.05, 0)
SetLightDiffuse(lights[3], COLOR_BLUE)
SetEntityRotation(lights[3], 0, 240, 0)

-- Create spheres
local spheres = {}
local mesh = CreateSphere(32)
ScaleMesh(mesh, 5, 5, 5)
local x = -32
local z = -32
for i = 1, 81 do
    local sphere = CreateModel(mesh)
    SetEntityPosition(sphere, x, 0, z)
    local mat = EntityMaterial(sphere, 1)
    SetMaterialShininess(mat, 0.15)
    SetMaterialSpecular(mat, COLOR_WHITE)
    spheres[i] = sphere
    x = x + 8
    if x > 32 then
        x = -32
        z = z + 8
    end
end
FreeMesh(mesh)

function Loop()
    -- Update lights
    for _, light in ipairs(lights) do
        TurnEntity(light, 0, -32 * DeltaTime(), 0)
        SetEntityPosition(light, 0, 0, 0)
        MoveEntity(light, 0, 0, -48)
    end

    BeginDrawing(true, true, COLOR_BLACK)
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    EndDrawing()
end
