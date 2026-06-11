OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local font = LoadFont("greenscr_12.xml")

-- Create a pixmap
local pixmap = CreatePixmap(256, 256)

-- Fill pixmap with a gradient
for y = 0, PixmapHeight(pixmap) - 1 do
    for x = 0, PixmapWidth(pixmap) - 1 do
        local r = math.floor((x / (PixmapWidth(pixmap) - 1)) * 255)
        local g = math.floor((y / (PixmapHeight(pixmap) - 1)) * 255)
        local b = 128
        WritePixel(pixmap, x, y, RGB(r, g, b))
    end
end

-- Create texture from pixmap
local tex = CreateTextureFromPixmap(pixmap)

-- Free pixmap as texture already contains the data
FreePixmap(pixmap)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    ClearScreen(RGB(0, 0, 0))

    -- Draw texture covering the full screen
    DrawTextureEx(tex, 0, 0, ScreenWidth(), ScreenHeight(), COLOR_WHITE)

    -- Draw fps
    DrawText(font, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)

    RefreshScreen()
end
