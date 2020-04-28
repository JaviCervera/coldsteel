#include "core.h"
#include "xml.h"

extern "C" {


EXPORT IXMLReaderUTF8* CALL asLoadXML(const char* filename) {
    return _asDevice()->getFileSystem()->createXMLReaderUTF8(filename);
}


EXPORT void CALL asCloseXML(IXMLReaderUTF8* xml) {
    xml->drop();
}


EXPORT bool_t CALL asPrepareNextXMLNode(IXMLReaderUTF8* xml) {
    bool_t valid = xml->read();
    while (valid && xml->getNodeType() == EXN_COMMENT) {
        valid = xml->read();
    }
    return valid;
}


EXPORT int CALL asXMLNodeType(IXMLReaderUTF8* xml) {
    map<EXML_NODE, int> types;
    types[EXN_NONE] = ASXML_NULL;
    types[EXN_ELEMENT] = ASXML_NODE;
    types[EXN_ELEMENT_END] = ASXML_NODEEND;
    types[EXN_TEXT] = ASXML_TEXT;
    types[EXN_CDATA] = ASXML_CDATA;
    types[EXN_UNKNOWN] = ASXML_UNKNOWN;
    return (types.find(xml->getNodeType()) != NULL)
        ? types[xml->getNodeType()]
        : ASXML_UNKNOWN;
}


EXPORT const char* CALL asXMLNodeText(IXMLReaderUTF8* xml) {
    return xml->getNodeName();
}


EXPORT int CALL asXMLNumAttributes(IXMLReaderUTF8* xml) {
    return xml->getAttributeCount();
}


EXPORT const char* CALL asXMLAttributeName(IXMLReaderUTF8* xml, int index) {
    return xml->getAttributeName(index);
}


EXPORT const char* CALL asXMLAttributeValue(IXMLReaderUTF8* xml, const char* name) {
    return xml->getAttributeValue(name);
}


} // extern "C"
