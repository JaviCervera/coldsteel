NUM_CUBES = 16 * 500

SetAmbient(RGB(75, 75, 75))
SetFog(RGB(64, 64, 64), 0, 20)

local cam = CreateCamera()
SetEntityPosition(cam, 0, 0, -7)
SetCameraRange(cam, 1, 5000)

local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 45, -45, 0)

local mesh = CreateCube()
SetMeshColor(mesh, RGB(140, 70, 20))

local cubes = {}
local x = -7
local z = NUM_CUBES / 16 * 2 - 2
for i = 1, NUM_CUBES, 2 do
    cubes[i] = CreateMeshEntity(mesh)
    SetMaterialFogEnabled(EntityMaterial(cubes[i], 0), true)
    SetEntityPosition(cubes[i], x, -1.5, z)
    cubes[i+1] = CreateMeshEntity(mesh)
    SetMaterialFogEnabled(EntityMaterial(cubes[i+1], 0), true)
    SetEntityPosition(cubes[i+1], x, 1.5, z)
    x = x + 2
    if x > 7 then
        x = -7
        z = z - 2
    end
end

local spacePressed = false

function Loop()
    if KeyDown(KEY_SPACE) then
        if not spacePressed then
            for _, cube in ipairs(cubes) do
                local mat = EntityMaterial(cube, 0)
                SetMaterialFogEnabled(mat, not MaterialFogEnabled(mat))
            end
        end
        spacePressed = true
    else
        spacePressed = false
    end

    for _, cube in ipairs(cubes) do
        TurnEntity(cube, 0, 32 * DeltaTime(), 0)
    end
    BeginDrawing(true, true, RGB(64, 64, 64))
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(255, 255, 255))
    DrawText(nil, "Press SPACE to toggle fog", 2, 14, RGB(255, 255, 255))
    EndDrawing()
end
