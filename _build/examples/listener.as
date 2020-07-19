const int MAX_PITCH = 80;
const int WORLD_GROUP = 1;
const int ROT_SPEED = 15;
const int MOVE_SPEED = 16;

void Main() {
    // Setup screen
    OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
    SetCursorVisible(false);
    if (!AddZip("assets.dat")) ChangeDir("assets");
    
    // Create player entity
    Entity@ player = CreateEntity();
    SetEntityPosition(player, 0, 2, 0);

    // Create and setup camera
    Camera@ cam = CreateCamera();
    SetEntityParent(cam, player);
    
    // Setup environment
    Texture@ top = LoadTexture("top.jpg");
    Texture@ bottom = LoadTexture("bottom.jpg");
    Texture@ left = LoadTexture("left.jpg");
    Texture@ right = LoadTexture("right.jpg");
    Texture@ front = LoadTexture("front.jpg");
    Texture@ back = LoadTexture("back.jpg");
    SetSkybox(top, bottom, left, right, front, back);
    SetAmbient(RGB(155, 155, 255));
    Light@ light = CreateLight(LIGHT_POINT);
    SetEntityRotation(light, 45, 215, 0);
    
    // Create floor
    Mesh@ mesh = CreateCube();
    ScaleMesh(mesh, 50, 1, 50);
    MeshEntity@ floor = CreateMeshEntity(mesh);
    SetEntityCollision(floor, COLLISION_BOX, WORLD_GROUP);
    SetEntityPosition(floor, 0, -0.5, 0);
    Material@ mat = EntityMaterial(floor, 0);
    SetMaterialDiffuse(mat, RGB(0, 0, 0));
    SetMaterialAmbient(mat, RGB(55, 55, 55));
    SetMaterialVertexColorsEnabled(mat, false);
    FreeMesh(mesh);
    
    // Create pillars
    @mesh = CreateCube();
    ScaleMesh(mesh, 4, 8, 4);
    array<int> xpos = {-10, -10, 10, 10};
    array<int> zpos = {-10, 10, -10, 10};
    for (int i = 0; i < xpos.Len(); ++i) {
        MeshEntity@ pillar = CreateMeshEntity(mesh);
        SetMeshEntityCastShadows(pillar, true);
        SetEntityCollision(pillar, COLLISION_BOX, WORLD_GROUP);
        SetEntityPosition(pillar, xpos[i], 4, zpos[i]);
        @mat = EntityMaterial(pillar, 0);
        SetMaterialDiffuse(mat, RGB(120, 0, 0));
        SetMaterialAmbient(mat, RGB(120, 0, 0));
        SetMaterialVertexColorsEnabled(mat, false);
    }
    FreeMesh(mesh);
    
    // Create floating sphere
    @mesh = CreateSphere(32);
    MeshEntity@ sphere = CreateMeshEntity(mesh);
    @mat = EntityMaterial(sphere, 0);
    SetMaterialDiffuse(mat, RGB(155, 155, 255));
    SetMaterialEmissive(mat, RGB(155, 155, 255));
    SetMaterialAmbient(mat, RGB(155, 155, 255));
    SetMaterialVertexColorsEnabled(mat, false);
    FreeMesh(mesh);
    
    // Play sound
    Sound@ sound = LoadSound("heli.wav");
    int channel = PlaySound(sound, true);

    float mxSpeed = 0, mySpeed = 0;
    SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2);
    while (Run() && !KeyDown(KEY_ESC)) {
        // Player yaw
        TurnEntity(player, 0, mxSpeed * ROT_SPEED * DeltaTime(), 0);
        
        // Camera pitch
        TurnEntity(cam, mySpeed * ROT_SPEED * DeltaTime(), 0, 0);
        if (EntityPitch(cam) > MAX_PITCH) SetEntityRotation(cam, MAX_PITCH, EntityYaw(cam), 0);
        if (EntityPitch(cam) < -MAX_PITCH) SetEntityRotation(cam, -MAX_PITCH, EntityYaw(cam), 0);
        
        // Move player
        float movX = 0, movZ = 0;
        if (KeyDown(KEY_W)) movZ = MOVE_SPEED * DeltaTime();
        if (KeyDown(KEY_S)) movZ = -MOVE_SPEED * DeltaTime();
        if (KeyDown(KEY_A)) movX = -MOVE_SPEED * DeltaTime();
        if (KeyDown(KEY_D)) movX = MOVE_SPEED * DeltaTime();
        SlideEntity(player, movX, 0, movZ, 1, 1, 1, WORLD_GROUP);
        
        // Move sphere
        SetEntityPosition(sphere, 0, 3, 0);
        TurnEntity(sphere, 0, 8 * DeltaTime(), 0);
        MoveEntity(sphere, 0, 0, -20);
        SetEntityPosition(light, EntityX(sphere), EntityY(sphere), EntityZ(sphere));
        SetChannelPosition(channel, EntityX(sphere), EntityY(sphere), EntityZ(sphere), 25);
        SetListener(EntityX(cam), EntityY(cam), EntityZ(cam), EntityYaw(cam));
        
        BeginDrawing(true, true, RGB(15, 15, 15));
        DrawWorld(cam);
        DrawText(null, Str(ScreenFPS()) + " FPS", 2, 2, RGB(255, 255, 255));
        EndDrawing();
        
        // Update mouse speed
        mxSpeed = CursorX() - ScreenWidth()/2;
        mySpeed = CursorY() - ScreenHeight()/2;
        SetCursorPosition(ScreenWidth()/2, ScreenHeight()/2);
    }
}
