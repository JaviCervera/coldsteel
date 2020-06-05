void Main() {
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    SetAmbient(RGB(75, 75, 75));
    SetFog(RGB(64, 64, 64), 0, 20);

    Camera@ cam = CreateCamera();
    SetEntityPosition(cam, 0, 0, -7);
    SetCameraRange(cam, 1, 5000);
    
    Light@ light = CreateLight(LIGHT_DIRECTIONAL);
    SetEntityRotation(light, 45, -45, 0);
    
    Mesh@ mesh = CreateCube();
    SetMeshColor(mesh, RGB(140, 70, 20));
    
    array<MeshEntity@> cubes(16 * 500);
    float x = -7;
    float z = cubes.length() / 16 * 2 - 2;
    for (uint i = 0; i < cubes.length(); i += 2) {
        @cubes[i] = CreateMeshEntity(mesh);
        SetMaterialFogEnabled(EntityMaterial(cubes[i], 0), true);
        SetEntityPosition(cubes[i], x, -1.5, z);
        @cubes[i+1] = CreateMeshEntity(mesh);
        SetMaterialFogEnabled(EntityMaterial(cubes[i+1], 0), true);
        SetEntityPosition(cubes[i+1], x, 1.5, z);
        x += 2;
        if (x > 7) {
            x = -7;
            z -= 2;
        }
    }


    bool spacePressed = false;
    while (Run() && !KeyDown(KEY_ESC)) {
        if (KeyDown(KEY_SPACE)) {
            if (!spacePressed) {
                for (uint i = 0; i < cubes.length(); ++i) {
                    Material@ mat = EntityMaterial(cubes[i], 0);
                    SetMaterialFogEnabled(mat, !MaterialFogEnabled(mat));
                }
            }
            spacePressed = true;
        } else {
            spacePressed = false;
        }
        
        for (uint i = 0; i < cubes.length(); ++i) {
            TurnEntity(cubes[i], 0, 32 * DeltaTime(), 0);
        }
        BeginDrawing(true, true, RGB(64, 64, 64));
        DrawWorld(cam);
        DrawText(null, Str(ScreenFPS()) + " FPS", 2, 2, RGB(255, 255, 255));
        DrawText(null, "Press SPACE to toggle fog", 2, 14, RGB(255, 255, 255));
        EndDrawing();
    }
}
