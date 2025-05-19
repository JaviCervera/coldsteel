/** @file */
#pragma once

#include "common.h"

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
