#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


struct XMLNode;


EXPORT XMLNode* CALL asParseXML(const char* filename);
EXPORT void CALL asDeleteXML(XMLNode* node);
EXPORT const char* CALL asXMLName(XMLNode* node);
EXPORT const char* CALL asXMLText(XMLNode* node);
EXPORT int CALL asXMLNumAttributes(XMLNode* node);
EXPORT const char* CALL asXMLAttributeName(XMLNode* node, int index);
EXPORT const char* CALL asXMLAttributeValue(XMLNode* node, const char* name);
EXPORT int CALL asXMLNumChildren(XMLNode* node);
EXPORT XMLNode* CALL asXMLChild(XMLNode* node, int index);
EXPORT int CALL asXMLNumChildrenNamed(XMLNode* node, const char* name);
EXPORT XMLNode* CALL asXMLChildNamed(XMLNode* node, const char* name, int index);


#ifdef __cplusplus
} /* extern "C" */
#endif
