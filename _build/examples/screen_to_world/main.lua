function CameraMode(cam)
    if CameraOrtho(cam) then return "ORTHOGRAPHIC" else return "PERSPECTIVE" end
end

function DrawBanner(cam)
    local text = ":: Press SPACE to change camera mode - Current mode: " .. CameraMode(cam) .. " ::"
    DrawRect(0, ScreenHeight() - 18, ScreenWidth(), 18, COLOR_BLACK)
    DrawText(nil, text, (ScreenWidth() - TextWidth(nil, text)) / 2, ScreenHeight() - 18, COLOR_WHITE)
end

OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local cam = CreateCamera()
SetCameraRange(cam, 1, 10)
SetEntityPosition(cam, 0, 0, -5)

local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 45, -45, 0)

local model = CreateCubeModel()

local depth = 0
local depth_dir = 1
local space_pressed = false
while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    -- Update
    depth = depth + depth_dir * DeltaTime()
    if depth < 0 or depth > 1 then depth = Clamp(depth, 0, 1); depth_dir = depth_dir * -1 end
    TurnEntity(model, 45 * DeltaTime(), 90 * DeltaTime(), 0)
    if KeyDown(KEY_SPACE) then
        if not space_pressed then SetCameraOrtho(cam, not CameraOrtho(cam)) end
        space_pressed = true
    else
        space_pressed = false
    end
    ScreenToWorld(cam, CursorX(), CursorY(), depth)
    SetEntityPosition(model, PointX(), PointY(), PointZ())

    -- Draw
    SetCameraViewport(cam, ScreenWidth() / 2 - 200, ScreenHeight() / 2 - 200, 400, 400)
    ClearScreen(COLOR_DARKGRAY)
    DrawWorld()
    DrawText(nil, ScreenFPS() .. " FPS", 2, 2, COLOR_WHITE)
    DrawText(nil, Int(CursorX()) .. ", " .. Int(CursorY()), 2, 18, COLOR_WHITE)
    DrawText(nil, Int(PointX()) .. ", " .. Int(PointY()) .. ", " .. Int(PointZ()), 2, 34, COLOR_WHITE)
    DrawBanner(cam)
    RefreshScreen()
end
