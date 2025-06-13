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
SetEntityPosition(cam, 0, 0, -5)

local light = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(light, 45, -45, 0)

local model = CreateCubeModel()
SetEntityName(model, "This is a cube")

local space_pressed = false
while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    -- Update
    TurnEntity(model, 45 * DeltaTime(), 90 * DeltaTime(), 0)
    if KeyDown(KEY_LEFT) then TranslateEntity(model, -DeltaTime(), 0, 0) end
    if KeyDown(KEY_RIGHT) then TranslateEntity(model, DeltaTime(), 0, 0) end
    if KeyDown(KEY_UP) then TranslateEntity(model, 0, DeltaTime(), 0) end
    if KeyDown(KEY_DOWN) then TranslateEntity(model, 0, -DeltaTime(), 0) end
    if KeyDown(KEY_SPACE) then
        if not space_pressed then SetCameraOrtho(cam, not CameraOrtho(cam)) end
        space_pressed = true
    else
        space_pressed = false
    end
    WorldToScreen(cam, EntityX(model), EntityY(model), EntityZ(model))

    -- Draw
    SetCameraViewport(cam, ScreenWidth() / 2 - 200, ScreenHeight() / 2 - 200, 400, 400)
    ClearScreen(COLOR_DARKGRAY)
    DrawWorld()
    DrawText(nil, ScreenFPS() .. " FPS", 2, 2, COLOR_WHITE)
    DrawText(nil, Int(PointX()) .. ", " .. Int(PointY()), 2, 16, COLOR_WHITE)
    DrawRect(PointX() - 2, PointY() - 2, 4, 4, COLOR_RED)
    DrawText(
        nil,
        EntityName(model),
        PointX() - TextWidth(nil, EntityName(model)) / 2,
        PointY(),
        COLOR_RED)
    DrawBanner(cam)
    RefreshScreen()
end
