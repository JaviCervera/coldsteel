local xml = ParseXML("example.xml")
for i = 1, XMLNumChildren(xml) do
	local child = XMLChild(xml, i)
	print(XMLName(child) .. " : " .. XMLText(child))
end

while not KeyDown(KEY_ESC) do end
