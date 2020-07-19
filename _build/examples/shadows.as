void Main() {
	OpenScreen(800, 600, 32, SCREEN_WINDOWED | SCREEN_RESIZABLE);
	if (!AddZip("assets.dat")) ChangeDir("assets");
	SetAmbient(RGB(75, 75, 75));
	
	Camera@ cam = CreateCamera();
	SetEntityPosition(cam, -150, 150, -200);
	SetEntityRotation(cam, 30, 30, 0);
	
	Light@ light = CreateLight(LIGHT_POINT);
	SetLightDiffuse(light, RGB(255, 155, 180));
	SetLightRadius(light, 800);
	
	Texture@ texture = LoadTexture("wall.jpg");
	MeshEntity@ room = CreateMeshEntity(LoadMesh("room.3ds"));
	for (int i = 0; i < EntityNumMaterials(room); ++i) {
		Material@ mat = EntityMaterial(room, i);
		SetMaterialSpecular(mat, RGB(0, 0, 0));
		SetMaterialTexture(mat, 0, texture);
	}
	
	MeshEntity@ dwarf = CreateMeshEntity(LoadMesh("dwarf.x"));
	SetEntityPosition(dwarf, -50, 20, -60);
	SetEntityScale(dwarf, 2, 2, 2);
	SetMeshEntityCastShadows(dwarf, true);
	
	while (Run() && !KeyDown(KEY_ESC)) {
		TurnEntity(light, 0, 90 * DeltaTime(), 0);
		SetEntityPosition(light, 0, 150, 0);
		MoveEntity(light, 0, 0, -250);
		BeginDrawing(true, true, RGB(55, 155, 255));
		DrawWorld(cam);
		EndDrawing();
	}
}
