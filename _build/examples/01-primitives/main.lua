-- Max number of primitives to draw
MAX_PRIMITIVES = 1000

-- Types of primitives supported
PRIM_LINE = 0
PRIM_RECT = 1

function CreatePrimitive(type, color, x, y, z, w)
    return {
        type = type,
        color = color,
        x = x,
        y = y,
        z = z,
        w = w
    }
end

function DrawPrimitive(prim)
    if prim.type == PRIM_LINE then
        DrawLine(prim.x, prim.y, prim.w, prim.z, prim.color)
    else
        DrawRect(prim.x, prim.y, prim.w, prim.z, prim.color)
    end
end

local primitives = {}

function Loop()
    -- If we are below the limit, add a new primitive with random values
    if #primitives < MAX_PRIMITIVES then
        primitives[#primitives+1] = CreatePrimitive(
            Rand(0, 2), -- Type
            RGB(Rand(0, 256), Rand(0, 256), Rand(0, 256)), -- Color
            Rand(0, ScreenWidth()), -- Coords
            Rand(0, ScreenHeight()),
            Rand(0, ScreenWidth()),
            Rand(0, ScreenHeight())
        )
    end

    -- Begin drawing on screen, clearing the screen to black
    BeginDrawing(true, false, 0)
    
    -- Draw all primitives
    for _, primitive in ipairs(primitives) do
        DrawPrimitive(primitive)
    end
    
    -- Draw statistics texts
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(255, 255, 255))
    DrawText(nil, Str(#primitives) .. " primitives", 2, 14, RGB(255, 255, 255))
    
    -- Finish drawing on screen
    EndDrawing()
end
