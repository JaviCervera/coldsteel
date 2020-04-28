void Main() {
	OpenScreen(800, 600, 32, SCREEN_WINDOWED);
	XML@ xml = LoadXML("assets/example.xml");
	array<string> nodes;
	while (PrepareNextXMLNode(xml)) {
		nodes.insertLast(XMLNodeText(xml));
	}
	CloseXML(xml);
	
	while (Run() && !KeyDown(KEY_ESC)) {
		BeginDrawing(true, false, RGB(0, 0, 0));
		for (uint i = 0; i < nodes.length(); ++i) {
			DrawText(null, nodes[i], 4, 4 + i*16, RGB(255, 255, 255));
		}
		EndDrawing();
	}
}
