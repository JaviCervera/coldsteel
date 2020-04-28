#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


EXPORT IXMLReaderUTF8* CALL asLoadXML(const char* filename);
EXPORT void CALL asCloseXML(IXMLReaderUTF8* xml);
EXPORT bool_t CALL asPrepareNextXMLNode(IXMLReaderUTF8* xml);
EXPORT int CALL asXMLNodeType(IXMLReaderUTF8* xml);
EXPORT const char* CALL asXMLNodeText(IXMLReaderUTF8* xml);
EXPORT int CALL asXMLNumAttributes(IXMLReaderUTF8* xml);
EXPORT const char* CALL asXMLAttributeName(IXMLReaderUTF8* xml, int index);
EXPORT const char* CALL asXMLAttributeValue(IXMLReaderUTF8* xml, const char* name);


#ifdef __cplusplus
} /* extern "C" */
#endif
