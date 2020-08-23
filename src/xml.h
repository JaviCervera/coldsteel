#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif


struct XMLNode;


EXPORT XMLNode* CALL csParseXML(const char* filename);
EXPORT void CALL csFreeXML(XMLNode* node);
EXPORT const char* CALL csXMLName(XMLNode* node);
EXPORT const char* CALL csXMLText(XMLNode* node);
EXPORT int CALL csXMLNumAttributes(XMLNode* node);
EXPORT const char* CALL csXMLAttributeName(XMLNode* node, int index);
EXPORT const char* CALL csXMLAttributeValue(XMLNode* node, const char* name);
EXPORT int CALL csXMLNumChildren(XMLNode* node);
EXPORT XMLNode* CALL csXMLChild(XMLNode* node, int index);
EXPORT int CALL csXMLNumChildrenNamed(XMLNode* node, const char* name);
EXPORT XMLNode* CALL csXMLChildNamed(XMLNode* node, const char* name, int index);


#ifdef __cplusplus
} /* extern "C" */
#endif
