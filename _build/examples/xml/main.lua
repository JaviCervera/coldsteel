local xml = ParseXML("example.xml")

function Loop()
	BeginDrawing(true, false, RGB(0, 0, 0))
	for i = 0, XMLNumChildren(xml) - 1 do
		local child = XMLChild(xml, i)
		DrawText(nil, XMLName(child) .. " : " .. XMLText(child), 4, 4 + i*16, RGB(255, 255, 255))
	end
	EndDrawing()
end
