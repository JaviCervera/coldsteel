void Main() {
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    if (!AddZip("assets.dat")) ChangeDir("assets");
    SetAmbient(RGB(255, 255, 255));

    Texture@ tex = LoadTexture("tree.png");
    Camera@ cam = CreateCamera();
    SetEntityRotation(cam, 30, 0, 0);
    
    for (int z = -8; z <= 8; z += 2) {
			for (int x = -8; x <= 8; x += 2) {
				Sprite@ tree = CreateSprite(tex, BLEND_ALPHA);
				SetEntityPosition(tree, x, 0, z);
				SetSpriteSize(tree, 1, 1);
			}
    }

    while (Run() && !KeyDown(KEY_ESC)) {
        TurnEntity(cam, 0, 15 * DeltaTime(), 0);
        SetEntityPosition(cam, 0, 0, 0);
        MoveEntity(cam, 0, 0, -8);

        BeginDrawing(true, true, RGB(0, 0, 128));
        DrawWorld(cam);
        EndDrawing();
    }
}
