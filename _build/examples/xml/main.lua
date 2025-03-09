OpenScreen(640, 480, DesktopDepth(), SCREEN_RESIZABLE)
local xml = ParseXML("example.xml")

function Loop()
	BeginDrawing(true, false, COLOR_BLACK)
	for i = 0, XMLNumChildren(xml) - 1 do
		local child = XMLChild(xml, i)
		DrawText(nil, XMLName(child) .. " : " .. XMLText(child), 4, 4 + i*16, COLOR_WHITE)
	end
	EndDrawing()
end
