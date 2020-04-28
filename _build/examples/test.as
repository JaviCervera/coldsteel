void Main() {
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    if (!AddZip("assets.dat")) ChangeDir("assets");

    Texture@ tex = LoadTexture("space_station.jpg");
    SetSkydome(tex, 1, true);
    Camera@ cam = CreateCamera();

    while (Run() && !KeyDown(KEY_ESC)) {
        TurnEntity(cam, 0, 8 * DeltaTime(), 0);

        BeginDrawing(false, true, 0);
        DrawWorld(cam);
        DrawText(null, CurrentDir(), 4, 4, RGB(255, 255, 0));
        EndDrawing();
    }
}
