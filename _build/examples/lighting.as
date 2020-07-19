void Main() {
    // Setup screen
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    if (!AddZip("assets.dat")) ChangeDir("assets");

    // Create and setup camera
    Camera@ cam = CreateCamera();
    SetEntityPosition(cam, 0, 32, -90);
    SetEntityRotation(cam, 16, 0, 0);
    
    // Setup lighting
    array<Light@> lights;
    SetAmbient(RGB(0, 0, 0));
    lights.Add(CreateLight(LIGHT_POINT));
    SetLightAttenuation(lights[lights.Len()-1], 1, 0.05, 0);
    SetLightDiffuse(lights[lights.Len()-1], RGB(255, 0, 0));
    SetEntityRotation(lights[lights.Len()-1], 0, 0, 0);
    lights.Add(CreateLight(LIGHT_POINT));
    SetLightAttenuation(lights[lights.Len()-1], 1, 0.05, 0);
    SetLightDiffuse(lights[lights.Len()-1], RGB(0, 255, 0));
    SetEntityRotation(lights[lights.Len()-1], 0, 120, 0);
    lights.Add(CreateLight(LIGHT_POINT));
    SetLightAttenuation(lights[lights.Len()-1], 1, 0.05, 0);
    SetLightDiffuse(lights[lights.Len()-1], RGB(0, 0, 255));
    SetEntityRotation(lights[lights.Len()-1], 0, 240, 0);
    
    // Create spheres
    array<MeshEntity@> spheres;
    Mesh@ mesh = CreateSphere(32);
    ScaleMesh(mesh, 5, 5, 5);
    float x = -32, z = -32;
    for (int i = 0; i < 81; ++i) {
        MeshEntity@ sphere = CreateMeshEntity(mesh);
        SetEntityPosition(sphere, x, 0, z);
        Material@ mat = EntityMaterial(sphere, 0);
        SetMaterialShininess(mat, 0.15);
        spheres.Add(sphere);
        x += 8;
        if (x > 32) {
            x = -32;
            z += 8;
        }
    }
    FreeMesh(mesh);

    while (Run() && !KeyDown(KEY_ESC)) {
        // Update lights
        for (int i = 0; i < lights.Len(); ++i) {
            TurnEntity(lights[i], 0, -32 * DeltaTime(), 0);
            SetEntityPosition(lights[i], 0, 0, 0);
            MoveEntity(lights[i], 0, 0, -48);
        }
        
        BeginDrawing(true, true, RGB(0, 0, 0));
        DrawWorld(cam);
        DrawText(null, Str(ScreenFPS()) + " FPS", 2, 2, RGB(255, 255, 255));
        EndDrawing();
    }
}
