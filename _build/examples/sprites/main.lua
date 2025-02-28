SetAmbient(COLOR_WHITE)

local tex = LoadTexture("tree.png")
local cam = CreateCamera()
SetEntityRotation(cam, 30, 0, 0)

for z = -8, 8, 2 do
    for x = -8, 8, 2 do
        local tree = CreateSprite(tex, MATERIAL_ALPHA)
        SetEntityPosition(tree, x, 0, z)
        SetSpriteSize(tree, 1, 1)
    end
end

function Loop()
    TurnEntity(cam, 0, 15 * DeltaTime(), 0)
    SetEntityPosition(cam, 0, 0, 0)
    MoveEntity(cam, 0, 0, -8)

    BeginDrawing(true, true, RGB(0, 0, 128))
    DrawWorld(cam)
    EndDrawing()
end
