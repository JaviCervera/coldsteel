local tex = LoadTexture("space_station.jpg")
SetSkydome(tex, 1, true)
local cam = CreateCamera()

function Loop()
    TurnEntity(cam, 0, 8 * DeltaTime(), 0)

    BeginDrawing(false, true, 0)
    DrawWorld(cam)
    DrawText(nil, CurrentDir(), 4, 4, RGB(255, 255, 0))
    EndDrawing()
end
