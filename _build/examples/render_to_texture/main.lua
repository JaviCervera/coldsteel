VERTEX_SIZE = 36
INDEX_SIZE = 2

function VerticesMemblock(vertices)
    local memblock = CreateMemblock(#vertices * VERTEX_SIZE)
    for v = 0, #vertices - 1 do
        local vertex = vertices[v + 1]
        PokeFloat(memblock, v * VERTEX_SIZE + 0, vertex[1])
        PokeFloat(memblock, v * VERTEX_SIZE + 4, vertex[2])
        PokeFloat(memblock, v * VERTEX_SIZE + 8, vertex[3])
        PokeFloat(memblock, v * VERTEX_SIZE + 12, vertex[4])
        PokeFloat(memblock, v * VERTEX_SIZE + 16, vertex[5])
        PokeFloat(memblock, v * VERTEX_SIZE + 20, vertex[6])
        PokeInt(memblock, v * VERTEX_SIZE + 24, vertex[7])
        PokeFloat(memblock, v * VERTEX_SIZE + 28, vertex[8])
        PokeFloat(memblock, v * VERTEX_SIZE + 32, vertex[9])
    end
    return memblock
end

function IndicesMemblock(indices)
    local memblock = CreateMemblock(#indices * INDEX_SIZE)
    for i = 0, #indices - 1 do
        local index = indices[i + 1]
        PokeShort(memblock, i * INDEX_SIZE, index)
    end
    return memblock
end

OpenScreen(400, 400, DesktopDepth(), SCREEN_RESIZABLE)
SetAmbient(COLOR_WHITE)

local tex = CreateRenderTexture(128, 128)

local cam = CreateCamera()
SetCameraRenderTarget(cam, tex)
SetEntityPosition(cam, 0, 0, -2)

local vertices = VerticesMemblock({
    -- x, y, z, nx, ny, nz, color, u, v
    {0, 0.5, 0, 0, 0, -1, COLOR_YELLOW, 0, 0},
    {0.5, -0.5, 0, 0, 0, -1, COLOR_CYAN, 0, 0},
    {-0.5, -0.5, 0, 0, 0, -1, COLOR_MAGENTA, 0, 0},
})
local indices = IndicesMemblock({0, 1, 2})
local mesh = CreateMesh()
local surf = AddSurface(mesh, vertices, 3, indices, 3, SURFACE_STANDARD)
FreeMemblock(vertices)
FreeMemblock(indices)

local triangle = CreateModel(mesh)
FreeMesh(mesh)
SetMaterialFlag(EntityMaterial(triangle, 1), FLAG_BACKFACECULLING, false)

while not ScreenShouldClose() and not KeyHit(KEY_ESC) do
    TurnEntity(triangle, 0, 64 * DeltaTime(), 0)

    DrawWorld()
    SetRenderTarget(tex, false, false, 0)
    DrawText(nil, Str(ScreenFPS()) .. " FPS", 2, 2, COLOR_WHITE)
    SetRenderTarget(nil, false, false, 0)
    DrawTextureEx(tex, 0, 0, ScreenWidth(), ScreenHeight(), COLOR_WHITE)
    RefreshScreen()
end
