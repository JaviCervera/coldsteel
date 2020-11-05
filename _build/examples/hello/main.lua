TEXT_SPEED = 200
TEXT = "Hello, world!"

local font = LoadFont("liberation_mono.xml")
local textX = 2
local textDir = 1

function Loop()
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

    BeginDrawing(true, false, RGB(48, 48, 48))
    DrawLine(0, textY - 3, ScreenWidth(), textY - 3, RGB(255, 255, 255))
    DrawLine(0, textY + textHeight + 2, ScreenWidth(), textY + textHeight + 2, RGB(255, 255, 255))
    DrawRect(0, textY - 2, ScreenWidth(), textHeight + 4, RGB(0, 0, 128))
    DrawText(font, TEXT, textX, textY, RGB(255, 255, 0))
    EndDrawing()
end
