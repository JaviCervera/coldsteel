TEXT_SPEED = 200
TEXT = "Hello, world!"

OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)

local font = LoadFont("liberation_mono.xml")
local textX = 2
local textDir = 1

while not ScreenShouldClose() and not KeyDown(KEY_ESC) do
    local textWidth = TextWidth(font, TEXT)
    local textHeight = TextHeight(font, TEXT)
    local textY = (ScreenHeight() - textHeight) / 2
    local leftLimit = 2
    local rightLimit = ScreenWidth() - textWidth - 2

    textX = textX + textDir * TEXT_SPEED * DeltaTime()
    if textX <= leftLimit then
        textX = leftLimit
        textDir = textDir * -1
    end
    if textX >= rightLimit then
        textX = rightLimit;
        textDir = textDir * -1;
    end

    ClearScreen(COLOR_DARKGRAY)
    DrawLine(0, textY - 3, ScreenWidth(), textY - 3, COLOR_WHITE)
    DrawLine(0, textY + textHeight + 2, ScreenWidth(), textY + textHeight + 2, COLOR_WHITE)
    DrawRect(0, textY - 2, ScreenWidth(), textHeight + 4, COLOR_BLUE)
    DrawText(font, TEXT, textX, textY, COLOR_YELLOW)
    RefreshScreen()
end
