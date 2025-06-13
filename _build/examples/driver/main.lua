DRIVERS = {"SoftwareFast", "Software", "OpenGL", "Direct3D"}
FULLSCREEN = {"No", "Yes"}
local driver = DRIVER_OPENGL    -- Current driver
local fullscreen = 0            -- 0=No, 1=Yes
local mode = 0                  -- Currently selected video mode (starts at zero)
local row = 0                   -- Currently selected row (starts at zero)

function RPad(str, len)
    local result = str
    if Len(str) < len then
        local diff = len - Len(str)
        for i = 1, diff do result = result .. " " end
    end
    return result
end

function Pad(str, len)
    local result = str
    if Len(str) < len then
        local half_diff = (len - Len(str)) * 0.5
        local left = Floor(half_diff)
        local right = Ceil(half_diff)
        for i = 1, left do result = " " .. result end
        for i = 1, right do result = result .. " " end
    end
    return result
end

function FormatMsg(a, b)
    return RPad(a, 22) .. "< " .. Pad(b, 22) .. " >"
end

function MsgColor(msg_row, sel_row)
    if msg_row == sel_row then
        return COLOR_BLACK
    else
        return COLOR_GRAY
    end
end

SetDriver(DRIVER_SOFTWAREFAST)
OpenScreen(400, 60, DesktopDepth(), 0)
SetScreenTitle("Up and down select, left and right change, Enter to confirm")
while not KeyHit(KEY_ENTER) do
    if KeyHit(KEY_ESC) or ScreenShouldClose() then Exit(0) end
    if row == 0 then
        if KeyHit(KEY_LEFT) then driver = (driver - 1) % #DRIVERS end
        if KeyHit(KEY_RIGHT) then driver = (driver + 1) % #DRIVERS end
    elseif row == 1 then
        if KeyHit(KEY_LEFT) then mode = (mode - 1) % ScreenNumModes() end
        if KeyHit(KEY_RIGHT) then mode = (mode + 1) % ScreenNumModes() end
    elseif row == 2 then
        if KeyHit(KEY_LEFT) then fullscreen = (fullscreen - 1) % #FULLSCREEN end
        if KeyHit(KEY_RIGHT) then fullscreen = (fullscreen + 1) % #FULLSCREEN end
    end
    if KeyHit(KEY_UP) then row = (row - 1) % 3 end
    if KeyHit(KEY_DOWN) then row = (row + 1) % 3 end

    ClearScreen(COLOR_LIGHTGRAY)
    DrawText(nil, FormatMsg("Video Driver: ", DRIVERS[driver + 1]), 8, 4, MsgColor(0, row))
    DrawText(nil, FormatMsg("Mode: ", ScreenModeWidth(mode + 1) .. "x" .. ScreenModeHeight(mode + 1) .. "@" .. ScreenModeDepth(mode + 1)), 8, 20, MsgColor(1, row))
    DrawText(nil, FormatMsg("Fullscreen: ", FULLSCREEN[fullscreen + 1]), 8, 36, MsgColor(2, row))
    RefreshScreen()
end

local flags = SCREEN_RESIZABLE
if fullscreen == 1 then flags = SCREEN_FULLSCREEN end
SetDriver(driver)
OpenScreen(ScreenModeWidth(mode + 1), ScreenModeHeight(mode + 1), ScreenModeDepth(mode + 1), flags)

-- Create and setup camera
local cam = CreateCamera()
SetEntityPosition(cam, 0, 2, -2)
SetEntityRotation(cam, 45, 0, 0)

-- Setup lighting
SetAmbient(COLOR_BLACK)
local dirLight = CreateLight(LIGHT_DIRECTIONAL)
SetEntityRotation(dirLight, 45, -45, 0)

-- Create cube
local cube = CreateCubeModel()

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(cube, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    DrawText(nil, "Driver: " .. DriverName(), 2, 2, COLOR_WHITE)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 16, COLOR_WHITE)
    RefreshScreen()
end
