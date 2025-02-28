local tex = LoadTexture("space_station.jpg")
SetSkydome(tex, 1, true)
local cam = CreateCamera()

function Loop()
    TurnEntity(cam, 0, 8 * DeltaTime(), 0)

    BeginDrawing(false, true, COLOR_BLACK)
    DrawWorld(cam)
    DrawText(nil, CurrentDir(), 4, 4, COLOR_YELLOW)
    EndDrawing()
end
