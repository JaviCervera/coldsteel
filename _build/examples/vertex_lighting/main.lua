MAX_PITCH = 80
ROT_SPEED = 15
MOVE_SPEED = 32
GRAVITY = -9.81 * 4
WORLD_GROUP = 1

function DrawBanner()
    local text = ":: Ruin gile[s] sample scene - by Mikkel Fredborg ::"
    DrawRect(0, ScreenHeight() - 22, ScreenWidth(), 22, COLOR_BLACK)
    DrawText(nil, text, (ScreenWidth() - TextWidth(nil, text)) / 2, ScreenHeight() - 19, COLOR_WHITE)
end

OpenScreen(640, 480, DesktopDepth(), 0) -- SCREEN_FULLSCREEN)
SetCursorVisible(false)

-- Create player entity
local player = CreateEntity()
SetEntityPosition(player, 0, 16, 0)

-- Create and setup camera
local cam = CreateCamera()
SetCameraClearColor(cam, COLOR_BLACK)
SetEntityParent(cam, player)

-- Load scene
local scene = LoadModel("ruin.b3d")
SetEntityCollision(scene, COLLISION_MESH, WORLD_GROUP)
for i = 6, EntityNumMaterials(scene) do
    -- The last materials should use texture masking
    local mat = EntityMaterial(scene, i)
    SetMaterialType(mat, MATERIAL_MASKED)
    SetTextureColorKey(MaterialTexture(mat, 1), COLOR_BLACK)
end

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
    DrawBanner()
    RefreshScreen()

    -- Update mouse speed
    mxSpeed = CursorX() - ScreenWidth()/2
    mySpeed = CursorY() - ScreenHeight()/2
    SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2)
end
