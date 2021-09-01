-- Max number of primitives to draw
MAX_PRIMITIVES = 1000

-- Types of primitives supported
PRIM_LINE = 0
PRIM_RECT = 1

-- This table represents a primitive on the screen
Primitive = {}
Primitive.__index = Primitive

function Primitive:New(type, color, x, y, z, w)
    local self = {}
    setmetatable(self, Primitive)
    self.type = type
    self.color = color
    self.x = x
    self.y = y
    self.z = z
    self.w = w
    return self
end

function Primitive:Draw()
    if self.type == PRIM_LINE then
        DrawLine(self.x, self.y, self.w, self.z, self.color)
    else
        DrawRect(self.x, self.y, self.w, self.z, self.color)
    end
end

local primitives = {}

function Loop()
    -- If we are below the limit, add a new primitive with random values
    if #primitives < MAX_PRIMITIVES then
        primitives[#primitives+1] = Primitive:New(
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
        primitive:Draw()
    end
    
    -- Draw statistics texts
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, RGB(255, 255, 255))
    DrawText(nil, Str(#primitives) .. " primitives", 2, 14, RGB(255, 255, 255))
    
    -- Finish drawing on screen
    EndDrawing()
end
