from dataclasses import dataclass
import os
from typing import List
import xml.etree.ElementTree as ET

DOCS_PATH = 'doc_out/xml'

@dataclass(frozen=True)
class Param:
  type: str
  name: str

@dataclass(frozen=True)
class Function:
  type: str
  name: str
  params: List[Param]

@dataclass(frozen=True)
class File:
  definitions: List[str]
  functions: List[Function]

  def __bool__(self):
    return bool(self.definitions) or bool(self.functions)

def main():
  with open(f'doc_out/documentation.md', mode='w') as f:
    for header in headers_to_parse():
      module = os.path.basename(header[:header.find('_8h.xml')])
      file = md_file(parse_file(header))
      if file:
        f.write(f'## {module.capitalize()}\n\n{file}\n\n')

# -------------------------------------
# XML Parsing
# -------------------------------------

def headers_to_parse():
  return sorted([os.path.join(DOCS_PATH, f) for f in os.listdir(DOCS_PATH) if f.endswith('_8h.xml')])


def parse_file(filename):
  root = ET.parse(filename).getroot()
  defs = sorted([d.text for d in root.findall('./compounddef/sectiondef/memberdef[@kind="define"]/name')])
  funcs = sorted([parse_function(f) for f in root.findall('./compounddef/sectiondef/memberdef[@kind="function"]')], key=lambda x: x.name)
  return File(definitions=defs, functions=funcs)


def parse_function(func):
  return Function(
    type=parse_type((func.find('./type/ref') if func.find('./type/ref') is not None else func.find('./type')).text),
    name=func.find('./name').text,
    params=[parse_param(p) for p in func.findall('./param')]
  )


def parse_param(param):
  return Param(
    type=parse_type((param.find('./type/ref') if param.find('./type/ref') is not None else param.find('./type')).text),
    name=param.find('./declname').text,
  )

def parse_type(type):
  if type is None:
    return 'None'
  type = type.replace('EXPORT ', '').replace(' CALL', '').replace('CALL', '').replace('const ', '')
  if type == 'bool_t':
    return 'bool'
  if type == 'char *':
    return 'string'
  if type == 'ICameraSceneNode *':
    return 'Camera'
  if type == 'ITexture *':
    return 'Texture'
  if type == 'ISceneNode *':
    return 'Entity'
  if type == 'SMaterial *':
    return 'Material'
  if type == 'IGUIElement *':
    return 'Control'
  if type == 'IGUIFont *':
    return 'Font'
  if type == 'ILightSceneNode *':
    return 'Light'
  if type == 'Memblock *':
    return 'Memblock'
  if type == 'IMesh *':
    return 'Mesh'
  if type == 'IImage *':
    return 'Pixmap'
  if type == 'IMeshBuffer *':
    return 'Surface'
  if type == 'IMeshSceneNode *':
    return 'Model'
  if type == 'void *':
    return 'ptr'
  if type == 'Sound *':
    return 'Sound'
  if type == 'IBillboardSceneNode *':
    return 'Sprite'
  if type == 'ITerrainSceneNode *':
    return 'Terrain'
  return type

# -------------------------------------
# Markdown generation
# -------------------------------------

def md_file(file):
  if not file:
    return ''
  str = ''
  if file.definitions:
    str += '### Constants\n' + '\n'.join([f'#### `{d}`' for d in file.definitions])
  if file.functions:
    if str:
      str += '\n\n'
    str += '### Functions\n' + '\n'.join([md_func(f) for f in file.functions])
  return str


def md_func(func):
  params = f'({", ".join([f"{p.name}: {p.type}" for p in func.params])})'
  str = f'{func.name}{params}'
  if func.type != "void":
    str += f': {func.type}'
  return f'#### `{str}`'


if __name__ == '__main__':
  main()
