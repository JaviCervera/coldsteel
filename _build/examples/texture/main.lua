OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local font = LoadFont("greenscr_12.xml")
local tex = LoadTexture("logo_small.png")

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    ClearScreen(RGB(0, 0, 0))

    -- Draw texture scaled to fill screen as a dimmed background
    DrawTextureEx(tex, 0, 0, ScreenWidth(), ScreenHeight(), RGB(64, 64, 64))

    -- Draw texture at original size centered
    local cx = (ScreenWidth() - TextureWidth(tex)) / 2
    local cy = (ScreenHeight() - TextureHeight(tex)) / 2
    DrawTexture(tex, cx, cy)

    -- Draw fps
    DrawText(font, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)

    RefreshScreen()
end
