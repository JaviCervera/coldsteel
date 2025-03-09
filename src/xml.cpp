#include "core.h"
#include "string.h"
#include "xml.h"

struct XMLAttribute
{
  XMLAttribute(const stringc &name, const stringc &value)
      : name(name), value(value) {}

  const stringc &getName() const { return name; };

  const stringc &getValue() const { return value; }

private:
  stringc name;
  stringc value;
};

struct XMLNode
{
  XMLNode(const stringc &name,
          const stringc &text,
          const array<XMLAttribute> &attributes,
          const array<XMLNode> &children) : name(name),
                                            text(text),
                                            attributes(attributes),
                                            children(children) {}

  const stringc &getName() const { return name; }

  const stringc &getText() const { return text; }

  int getNumAttributes() const { return attributes.size(); }

  const stringc &getAttributeName(int index) const { return attributes[index].getName(); }

  const stringc &getAttributeValue(const stringc &name) const
  {
    static const stringc empty = "";
    const stringc lower = Lower(name.c_str());
    for (int i = 0; i < getNumAttributes(); ++i)
    {
      if (lower == Lower(attributes[i].getName().c_str()))
      {
        return attributes[i].getValue();
      }
    }
    return empty;
  }

  int getNumChildren() const { return children.size(); }

  XMLNode *getChild(int index)
  {
    if (index >= 0 && index < children.size())
    {
      return &children[index];
    }
    else
    {
      return NULL;
    }
  }

  array<XMLNode *> findChildren(const stringc &name)
  {
    const stringc lower = Lower(name.c_str());
    array<XMLNode *> arr;
    for (int i = 0; i < getNumChildren(); ++i)
    {
      if (lower == Lower(children[i].getName().c_str()))
      {
        arr.push_back(&children[i]);
      }
    }
    return arr;
  }

private:
  stringc name;
  stringc text;
  array<XMLAttribute> attributes;
  array<XMLNode> children;
};

extern "C"
{

  bool_t _asPrepareNextXMLElement(IXMLReaderUTF8 *xml);
  XMLNode *_asParseXMLNode(IXMLReaderUTF8 *xml);

  EXPORT XMLNode *CALL ParseXML(const char *filename)
  {
    IXMLReaderUTF8 *reader = _Device()->getFileSystem()->createXMLReaderUTF8(filename);
    if (reader != NULL)
    {
      while (_asPrepareNextXMLElement(reader) && reader->getNodeType() != EXN_ELEMENT)
      {
      }
      return (reader->getNodeType() == EXN_ELEMENT)
                 ? _asParseXMLNode(reader)
                 : NULL;
    }
    else
    {
      return NULL;
    }
  }

  EXPORT void CALL FreeXML(XMLNode *node)
  {
    delete node;
  }

  EXPORT const char *CALL XMLName(XMLNode *node)
  {
    return node ? node->getName().c_str() : "";
  }

  EXPORT const char *CALL XMLText(XMLNode *node)
  {
    return node ? node->getText().c_str() : "";
  }

  EXPORT int CALL XMLNumAttributes(XMLNode *node)
  {
    return node ? node->getNumAttributes() : 0;
  }

  EXPORT const char *CALL XMLAttributeName(XMLNode *node, int index)
  {
    return node ? node->getAttributeName(index - 1).c_str() : "";
  }

  EXPORT const char *CALL XMLAttributeValue(XMLNode *node, const char *name)
  {
    return node ? node->getAttributeValue(name).c_str() : "";
  }

  EXPORT int CALL XMLNumChildren(XMLNode *node)
  {
    return node ? node->getNumChildren() : 0;
  }

  EXPORT XMLNode *CALL XMLChild(XMLNode *node, int index)
  {
    return node ? node->getChild(index - 1) : NULL;
  }

  EXPORT int CALL XMLNumChildrenNamed(XMLNode *node, const char *name)
  {
    return node ? node->findChildren(name).size() : 0;
  }

  EXPORT XMLNode *CALL XMLChildNamed(XMLNode *node, const char *name, int index)
  {
    if (!node)
      return NULL;
    array<XMLNode *> children = node->findChildren(name);
    if (index > 0 && index <= children.size())
    {
      return children[index - 1];
    }
    else
    {
      return NULL;
    }
  }

  bool_t _asPrepareNextXMLElement(IXMLReaderUTF8 *xml)
  {
    bool_t valid = xml->read();
    while (valid && xml->getNodeType() == EXN_COMMENT)
    {
      valid = xml->read();
    }
    return valid;
  }

  XMLNode *_asParseXMLNode(IXMLReaderUTF8 *xml)
  {
    stringc name = xml->getNodeName();
    stringc text = "";
    array<XMLAttribute> attributes;
    for (int i = 0; i < xml->getAttributeCount(); ++i)
    {
      stringc attName = xml->getAttributeName(i);
      stringc attValue = xml->getAttributeValue(attName.c_str());
      attributes.push_back(XMLAttribute(attName, attValue));
    }
    array<XMLNode> children;
    if (!xml->isEmptyElement())
    {
      while (_asPrepareNextXMLElement(xml) && xml->getNodeType() != EXN_ELEMENT_END)
      {
        XMLNode *child = NULL;
        switch (xml->getNodeType())
        {
        case EXN_ELEMENT:
          child = _asParseXMLNode(xml);
          children.push_back(*child);
          delete child;
          break;
        case EXN_CDATA:
        case EXN_TEXT:
          text += xml->getNodeData();
          break;
        default:
          break;
        }
      }
    }
    return new XMLNode(name, text, attributes, children);
  }

} // extern "C"
