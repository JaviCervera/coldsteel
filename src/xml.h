#pragma once

#include "common.h"

// XML node types
#define XML_NULL 0
#define XML_NODE 1
#define XML_NODEEND 2
#define XML_TEXT 3
#define XML_CDATA 5
#define XML_UNKNOWN 6

#ifdef __cplusplus
extern "C"
{
#endif

  struct XMLNode;

  EXPORT XMLNode *CALL ParseXML(const char *filename);
  EXPORT void CALL FreeXML(XMLNode *node);
  EXPORT const char *CALL XMLName(XMLNode *node);
  EXPORT const char *CALL XMLText(XMLNode *node);
  EXPORT int CALL XMLNumAttributes(XMLNode *node);
  EXPORT const char *CALL XMLAttributeName(XMLNode *node, int index);
  EXPORT const char *CALL XMLAttributeValue(XMLNode *node, const char *name);
  EXPORT int CALL XMLNumChildren(XMLNode *node);
  EXPORT XMLNode *CALL XMLChild(XMLNode *node, int index);
  EXPORT int CALL XMLNumChildrenNamed(XMLNode *node, const char *name);
  EXPORT XMLNode *CALL XMLChildNamed(XMLNode *node, const char *name, int index);

#ifdef __cplusplus
} /* extern "C" */
#endif
