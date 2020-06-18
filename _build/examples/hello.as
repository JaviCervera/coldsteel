void Main() {
    const int TEXT_SPEED = 200;
    const string text = "Hello, world!";
    
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    if (!AddZip("assets.dat")) ChangeDir("assets");
    
    Font@ font = LoadFont("liberation_mono.xml");
    
    float textX = 2;
    int textDir = 1;
    while (Run() && !KeyDown(KEY_ESC)) {
        const int textWidth = TextWidth(font, text);
        const int textHeight = TextHeight(font, text);
        const int textY = (ScreenHeight() - textHeight) / 2;
        const int leftLimit = 2;
        const int rightLimit = ScreenWidth() - textWidth - 2;
        
        textX += textDir * TEXT_SPEED * DeltaTime();
        if (textX <= leftLimit) {
            textX = leftLimit;
            textDir *= -1;
        }
        if (textX >= rightLimit) {
            textX = rightLimit;
            textDir *= -1;
        }
    
        BeginDrawing(true, false, RGB(48, 48, 48));
        DrawLine(0, textY - 3, ScreenWidth(), textY - 3, RGB(255, 255, 255));
        DrawLine(0, textY + textHeight + 2, ScreenWidth(), textY + textHeight + 2, RGB(255, 255, 255));
        DrawRect(0, textY - 2, ScreenWidth(), textHeight + 4, RGB(0, 0, 128));
        DrawText(font, text, int(textX), textY, RGB(255, 255, 0));
        EndDrawing();
    }
}
