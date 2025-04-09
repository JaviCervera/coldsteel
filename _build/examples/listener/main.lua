MAX_PITCH = 80
WORLD_GROUP = 1
ROT_SPEED = 15
MOVE_SPEED = 16
SOUND_RADIUS = 25

OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
SetCursorVisible(false)

-- Create player entity
local player = CreateEntity()
SetEntityPosition(player, 0, 2, 0)

-- Create and setup camera
local cam = CreateCamera()
SetEntityParent(cam, player)

-- Setup environment
local top = LoadTexture("top.jpg")
local bottom = LoadTexture("bottom.jpg")
local left = LoadTexture("left.jpg")
local right = LoadTexture("right.jpg")
local front = LoadTexture("front.jpg")
local back = LoadTexture("back.jpg")
SetSkybox(top, bottom, left, right, front, back)
SetAmbient(RGB(155, 155, 255))
local light = CreateLight(LIGHT_POINT)
SetEntityRotation(light, 45, 215, 0)

-- Create floor
local mesh = CreateCubeMesh()
ScaleMesh(mesh, 50, 1, 50)
local floor = CreateModel(mesh)
SetEntityCollision(floor, COLLISION_BOX, WORLD_GROUP)
SetEntityPosition(floor, 0, -0.5, 0)
local mat = EntityMaterial(floor, 1)
SetMaterialDiffuse(mat, COLOR_BLACK)
SetMaterialAmbient(mat, RGB(55, 55, 55))
SetMaterialVertexColorsEnabled(mat, false)
FreeMesh(mesh)

-- Create pillars
mesh = CreateCubeMesh()
ScaleMesh(mesh, 4, 8, 4)
local xpos = {-10, -10, 10, 10}
local zpos = {-10, 10, -10, 10}
for i = 1, #xpos do
    local pillar = CreateModel(mesh)
    SetModelCastShadows(pillar, true)
    SetEntityCollision(pillar, COLLISION_BOX, WORLD_GROUP)
    SetEntityPosition(pillar, xpos[i], 4, zpos[i])
    mat = EntityMaterial(pillar, 1)
    SetMaterialDiffuse(mat, RGB(120, 0, 0))
    SetMaterialAmbient(mat, RGB(120, 0, 0))
    SetMaterialVertexColorsEnabled(mat, false)
end
FreeMesh(mesh)

-- Create floating sphere
mesh = CreateSphereMesh(32)
local sphere = CreateModel(mesh)
mat = EntityMaterial(sphere, 1)
SetMaterialDiffuse(mat, RGB(155, 155, 255))
SetMaterialEmissive(mat, RGB(155, 155, 255))
SetMaterialAmbient(mat, RGB(155, 155, 255))
SetMaterialVertexColorsEnabled(mat, false)
FreeMesh(mesh)

-- Play sound
local sound = LoadSound("heli.ogg")
local channel = PlaySound3D(sound, EntityX(sphere), EntityY(sphere), EntityZ(sphere), SOUND_RADIUS, true)

local mxSpeed = 0
local mySpeed = 0
SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2)

function Loop()
    -- Player yaw
    TurnEntity(player, 0, mxSpeed * ROT_SPEED * DeltaTime(), 0)

    -- Camera pitch
    TurnEntity(cam, mySpeed * ROT_SPEED * DeltaTime(), 0, 0)
    if (EntityPitch(cam) > MAX_PITCH) then SetEntityRotation(cam, MAX_PITCH, EntityYaw(cam), 0) end
    if (EntityPitch(cam) < -MAX_PITCH) then SetEntityRotation(cam, -MAX_PITCH, EntityYaw(cam), 0) end

    -- Move player
    local movX = 0
    local movZ = 0
    if (KeyDown(KEY_W)) then movZ = MOVE_SPEED * DeltaTime() end
    if (KeyDown(KEY_S)) then movZ = -MOVE_SPEED * DeltaTime() end
    if (KeyDown(KEY_A)) then movX = -MOVE_SPEED * DeltaTime() end
    if (KeyDown(KEY_D)) then movX = MOVE_SPEED * DeltaTime() end
    SlideEntity(player, movX, 0, movZ, 1, 1, 1, WORLD_GROUP)

    -- Move sphere
    SetEntityPosition(sphere, 0, 3, 0)
    TurnEntity(sphere, 0, 8 * DeltaTime(), 0)
    MoveEntity(sphere, 0, 0, -20)
    SetEntityPosition(light, EntityX(sphere), EntityY(sphere), EntityZ(sphere))
    SetChannelPosition(channel, EntityX(sphere), EntityY(sphere), EntityZ(sphere), SOUND_RADIUS)
    SetListener(EntityX(cam), EntityY(cam), EntityZ(cam), EntityYaw(cam))

    BeginDrawing(true, true, RGB(15, 15, 15))
    DrawWorld(cam)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    EndDrawing()

    -- Update mouse speed
    mxSpeed = CursorX() - ScreenWidth()/2
    mySpeed = CursorY() - ScreenHeight()/2
    SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2)
end
