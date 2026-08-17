OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local font = LoadFont("greenscr_12.xml")
local tex = LoadTexture("tex.png")

-- Load a shader shared by both cubes
local shader = LoadShader("shader.vert", "shader.frag", MATERIAL_SOLID)
if shader == nil then
    LogInfo("Failed to load shader. Make sure the OpenGL driver is active.")
    Exit()
end

-- Setup camera and lighting
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -3)
SetEntityRotation(cam, 25, 0, 0)
SetAmbient(COLOR_BLACK)
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Memblock holding the animated 4x4 UV matrix (16 floats)
local uMat = CreateMemblock(64)

-- Two cubes sharing the same shader, differing in diffuse color and mParam1
local cube1 = CreateCubeModel()
local mat1 = EntityMaterial(cube1, 1)
SetMaterialTexture(mat1, 1, tex)
SetMaterialShader(mat1, shader)
SetMaterialDiffuse(mat1, RGB(255, 60, 60))
SetMaterialParam1(mat1, 2.0)
SetEntityPosition(cube1, -1.5, 1, 0)

local cube2 = CreateCubeModel()
local mat2 = EntityMaterial(cube2, 1)
SetMaterialTexture(mat2, 1, tex)
SetMaterialShader(mat2, shader)
SetMaterialDiffuse(mat2, RGB(60, 60, 255))
SetMaterialParam1(mat2, 5.0)
SetEntityPosition(cube2, 1.5, 1, 0)

local angle = 0

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cube1, 0, 64 * DeltaTime(), 0)
    TurnEntity(cube2, 0, -64 * DeltaTime(), 0)

    -- Animate the UV matrix inside the memblock
    angle = angle + 90 * DeltaTime()
    local c = Cos(angle)
    local s = Sin(angle)
    PokeFloat(uMat, 0, c)
    PokeFloat(uMat, 4, 0)
    PokeFloat(uMat, 8, s)
    PokeFloat(uMat, 12, 0)
    PokeFloat(uMat, 16, 0)
    PokeFloat(uMat, 20, 1)
    PokeFloat(uMat, 24, 0)
    PokeFloat(uMat, 28, 0)
    PokeFloat(uMat, 32, -s)
    PokeFloat(uMat, 36, 0)
    PokeFloat(uMat, 40, c)
    PokeFloat(uMat, 44, 0)
    PokeFloat(uMat, 48, 0)
    PokeFloat(uMat, 52, 0)
    PokeFloat(uMat, 56, 0)
    PokeFloat(uMat, 60, 1)
    SetShaderMatrix(shader, "uMat", uMat)

    -- uTime drives the pulse speed; mDiffuse and mParam1 are read from each material
    SetShaderFloat(shader, "uTime", Millisecs() / 1000.0)

    DrawWorld()
    DrawText(font, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()
end
