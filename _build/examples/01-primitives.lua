-- Max number of primitives to draw
MAX_PRIMITIVES = 1000

-- Types of primitives supported
PRIM_LINE = 0
PRIM_RECT = 1

-- This table represents a primitive on the screen
Primitive = {
    function New(self, type, color, x, y, z, w)
        self.type = type
        self.color = color
        self.x = x
        self.y = y
        self.z = z
        self.w = w
    end
    
    function Draw() {
        if (type == PRIM_LINE) {
            DrawLine(x, y, w, z, color);
        } else {
            DrawRect(x, y, w, z, color);
        }
    }
    
    private int type;
    private int color;
    private int x, y, z, w;
}

void Main() {
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    
    array<Primitive> primitives;
    while (Run() && !KeyDown(KEY_ESC)) {
        // If we are below the limit, add a new primitive with random values
        if (primitives.Len() < MAX_PRIMITIVES) {
            primitives.Add(Primitive(
                Rand(0, 2), // Types
                RGB(Rand(0, 256), Rand(0, 256), Rand(0, 256)), // Color
                Rand(0, ScreenWidth()), // Coordinates
                Rand(0, ScreenHeight()),
                Rand(0, ScreenWidth()),
                Rand(0, ScreenHeight())));
        }

        // Begin drawing on screen, clearing the screen to black
        BeginDrawing(true, false, 0);
        
        // Draw all primitives
        for (int i = 0; i < primitives.Len(); ++i) {
            primitives[i].Draw();
        }
        
        // Draw statistics texts
        DrawText(null, Str(ScreenFPS()) + " FPS", 2, 2, RGB(255, 255, 255));
        DrawText(null, Str(primitives.Len()) + " primitives", 2, 14, RGB(255, 255, 255));
        
        // Finish drawing on screen
        EndDrawing();
    }
}
