MAX_PITCH = 80
ROT_SPEED = 15
MOVE_SPEED = 32
GRAVITY = -9.81 * 4
WORLD_GROUP = 1

OpenScreen(640, 480, DesktopDepth(), SCREEN_FULLSCREEN)
SetCursorVisible(false)

-- Camera
local player = CreateEntity()
local cam = CreateCamera()
SetCameraClearMode(cam, false, true)
SetEntityParent(cam, player)
SetEntityPosition(player, 1024, 64, 1024)
SetSkydome(LoadTexture("skydome.jpg"), 0.75, true)

-- Load terrain
local terrain = LoadTerrain("terrain-heightmap.bmp", 2048, 128, 2048)
ScaleTerrainTexture(terrain, 1, 64)
local mat = EntityMaterial(terrain, 1)
SetMaterialType(mat, MATERIAL_DETAIL)
SetMaterialTexture(mat, 1, LoadTexture("terrain-texture.jpg"))
SetMaterialTexture(mat, 2, LoadTexture("detailmap3.jpg"))
SetEntityCollision(terrain, COLLISION_MESH, WORLD_GROUP)

local mxSpeed = 0
local mySpeed = 0
SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
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
    SlideEntity(player, movX, GRAVITY * DeltaTime(), movZ, 2, 16, 2, WORLD_GROUP)

    DrawWorld()
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    RefreshScreen()

    -- Update mouse speed
    mxSpeed = CursorX() - ScreenWidth()/2
    mySpeed = CursorY() - ScreenHeight()/2
    SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2)
end
