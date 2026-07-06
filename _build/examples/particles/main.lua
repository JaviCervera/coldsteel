OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_WHITE)

local tex = LoadTexture("particle.png")
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityRotation(cam, 30, 0, 0)

local ps = CreateParticleSystem()
SetEntityPosition(ps, 0, 2, 0)
local mat = EntityMaterial(ps, 1)
SetMaterialTexture(mat, 1, tex)
SetMaterialType(mat, MATERIAL_ALPHA)
SetMaterialFlag(mat, FLAG_LIGHTING, false)
SetParticleSize(ps, 3, 3)
SetBoxEmitter(ps, -2, 0, -2, 2, 4, 2)
SetEmitterDirection(ps, 0, 0.04, 0)
SetEmitterRate(ps, 30, 60)
SetEmitterColor(ps, COLOR_RED, COLOR_YELLOW)
SetEmitterLife(ps, 500, 1500)
SetEmitterSpread(ps, 15)
AddFadeOutAffector(ps, RGBA(255, 255, 0, 0), 1000)
AddGravityAffector(ps, 0, -0.002, 0, 400)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cam, 0, 15 * DeltaTime(), 0)
    SetEntityPosition(cam, 0, 0, 0)
    MoveEntity(cam, 0, 0, -20)

    DrawWorld()
    RefreshScreen()
end
