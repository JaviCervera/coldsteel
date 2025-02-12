#pragma once

#include "common.h"

struct XMLNode;

XMLNode *ParseXML(const char *filename);
void FreeXML(XMLNode *node);
const char *XMLName(XMLNode *node);
const char *XMLText(XMLNode *node);
int XMLNumAttributes(XMLNode *node);
const char *XMLAttributeName(XMLNode *node, int index);
const char *XMLAttributeValue(XMLNode *node, const char *name);
int XMLNumChildren(XMLNode *node);
XMLNode *XMLChild(XMLNode *node, int index);
int XMLNumChildrenNamed(XMLNode *node, const char *name);
XMLNode *XMLChildNamed(XMLNode *node, const char *name, int index);
