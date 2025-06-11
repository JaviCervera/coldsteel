NUM_CUBES = 16 * 500

OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_DARKGRAY)
SetFog(COLOR_DARKGRAY, 0, 20)

local cam = CreateCamera()
SetEntityPosition(cam, 0, 0, -7)
SetCameraRange(cam, 1, 5000)
SetCameraClearColor(cam, COLOR_DARKGRAY)

local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 45, -45, 0)

local mesh = CreateCubeMesh()
local mat = SurfaceMaterial(MeshSurface(mesh, 1))
SetMaterialFlag(mat, FLAG_VERTEXCOLORS, false)
SetMaterialDiffuse(mat, COLOR_BROWN)
SetMaterialAmbient(mat, COLOR_BROWN)

local cubes = {}
local x = -7
local z = NUM_CUBES / 16 * 2 - 2
for i = 1, NUM_CUBES, 2 do
    cubes[i] = CreateModel(mesh)
    SetMaterialFlag(EntityMaterial(cubes[i], 1), FLAG_FOG, true)
    SetEntityPosition(cubes[i], x, -1.5, z)
    cubes[i+1] = CreateModel(mesh)
    SetMaterialFlag(EntityMaterial(cubes[i+1], 1), FLAG_FOG, true)
    SetEntityPosition(cubes[i+1], x, 1.5, z)
    x = x + 2
    if x > 7 then
        x = -7
        z = z - 2
    end
end

local spacePressed = false

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    if KeyDown(KEY_SPACE) then
        if not spacePressed then
            for _, cube in ipairs(cubes) do
                local mat = EntityMaterial(cube, 1)
                SetMaterialFlag(mat, FLAG_FOG, not MaterialFlag(mat, FLAG_FOG))
            end
        end
        spacePressed = true
    else
        spacePressed = false
    end

    for _, cube in ipairs(cubes) do
        TurnEntity(cube, 0, 32 * DeltaTime(), 0)
    end
    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    DrawText(nil, "Press SPACE to toggle fog", 2, 14, COLOR_WHITE)
    RefreshScreen()
end
