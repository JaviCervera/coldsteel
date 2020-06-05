// Max number of primitives to draw
const int MAX_PRIMITIVES = 1000;

// Types of primitives supported
const int PRIM_LINE = 0;
const int PRIM_RECT = 1;

// This class represents a primitive on the screen
class Primitive {
    // This constructor is required to be able to declare and array of Primitive objects
    Primitive() {}
    
    Primitive(int type, int color, int x, int y, int z, int w) {
        this.type = type;
        this.color = color;
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }
    
    void Draw() {
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
        if (primitives.length() < MAX_PRIMITIVES) {
            primitives.insertLast(Primitive(
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
        for (uint i = 0; i < primitives.length(); ++i) {
            primitives[i].Draw();
        }
        
        // Draw statistics texts
        DrawText(null, Str(ScreenFPS()) + " FPS", 2, 2, RGB(255, 255, 255));
        DrawText(null, Str(primitives.length()) + " primitives", 2, 14, RGB(255, 255, 255));
        
        // Finish drawing on screen
        EndDrawing();
    }
}
