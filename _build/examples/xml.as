void Main() {
	OpenScreen(800, 600, 32, SCREEN_WINDOWED);
	XMLNode@ xml = ParseXML("assets/example.xml");
	
	while (Run() && !KeyDown(KEY_ESC)) {
		BeginDrawing(true, false, RGB(0, 0, 0));
		for (int i = 0; i < XMLNumChildren(xml); ++i) {
			XMLNode@ child = XMLChild(xml, i);
			DrawText(null, XMLName(child) + " : " + XMLText(child), 4, 4 + i*16, RGB(255, 255, 255));
		}
		EndDrawing();
	}
	
	FreeXML(xml);
}
