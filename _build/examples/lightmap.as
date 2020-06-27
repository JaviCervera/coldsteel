void Main() {
    // Setup screen
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    if (!AddZip("assets.dat")) ChangeDir("assets");
    SetAmbient(RGB(255, 255, 255));

    // Create and setup camera
    Camera@ cam = CreateCamera();
    SetEntityPosition(cam, 0, 2, -2);
    SetEntityRotation(cam, 45, 0, 0);
    
    // Create cube
    Texture@ boxTex = LoadTexture("box.png");
    Texture@ lightmapTex = LoadTexture("lightmap.png");
    Mesh@ mesh = CreateCube();
    MeshEntity@ cube = CreateMeshEntity(mesh);
    Material@ mat = EntityMaterial(cube, 0);
    SetMaterialType(mat, MATERIAL_LIGHTMAP);
    SetMaterialTexture(mat, 0, boxTex);
    SetMaterialTexture(mat, 1, lightmapTex);
    FreeMesh(mesh);
    //FreeTexture(boxTex);
    //FreeTexture(lightmapTex);

    while (Run() && !KeyDown(KEY_ESC)) {
        TurnEntity(cube, 0, 64 * DeltaTime(), 0);

        BeginDrawing(true, true, RGB(15, 15, 15));
        DrawWorld(cam);
        DrawText(null, Str(ScreenFPS()) + " FPS", 2, 2, RGB(255, 255, 255));
        EndDrawing();
    }
}
