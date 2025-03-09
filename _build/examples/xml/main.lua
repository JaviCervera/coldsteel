OpenConsole()
local xml = ParseXML("example.xml")
for i = 0, XMLNumChildren(xml) - 1 do
	local child = XMLChild(xml, i)
	print(XMLName(child) .. " : " .. XMLText(child))
end
