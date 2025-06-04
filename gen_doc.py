from dataclasses import dataclass
import os
import re
from typing import List
import xml.etree.ElementTree as ET

import markdown
from markdown.extensions import Extension

DOCS_PATH = 'doc_out/xml'

@dataclass(frozen=True)
class Param:
  type: str
  name: str
  description: str

@dataclass(frozen=True)
class Function:
  description: str
  type: str
  name: str
  params: List[Param]
  return_desc: str

@dataclass(frozen=True)
class File:
  description: str
  definitions: List[str]
  functions: List[Function]

  def __bool__(self):
    return bool(self.description) or bool(self.definitions) or bool(self.functions)

class AnchorHeaderExtension(Extension):
  def extendMarkdown(self, md):
    md.preprocessors.register(AnchorHeaderPreprocessor(md), 'anchor_headers', 175)

class AnchorHeaderPreprocessor(markdown.preprocessors.Preprocessor):
  def run(self, lines):
    new_lines = []
    for line in lines:
        # Match headers (e.g., # Header, ## Header, etc.)
        if line.startswith('#'):
          # Create an anchor link from the header text
          header_text = line.strip('# ').strip()
          code_tag_start = '<code>' if header_text.startswith('`') else ''
          code_tag_end = '</code>' if header_text.startswith('`') else ''
          if code_tag_start:
            header_text = header_text.strip('`')
            anchor = re.sub(r'\s+', '-', header_text).lower()
            anchor = anchor.split('(')[0]
            line = f'<h{line.count("#")}>{code_tag_start}<a id="{anchor}" href="#{anchor}">{header_text}</a>{code_tag_end}</h{line.count("#")}>'
          else:
            line = f'<h{line.count("#")}>{header_text}</h{line.count("#")}>'
        new_lines.append(line)
    return new_lines

def main():
  doc = '# ColdSteel Engine documentation\n\n'
  for header in headers_to_parse():
    module = os.path.basename(header[:header.find('_8h.xml')])
    file = md_file(module, parse_file(header))
    if file:
      doc += f'{file}\n\n'
  html = f"""
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="styles.css">
      <title>ColdSteel Engine documentation</title>
  </head>
  <body>
      {markdown.markdown(doc, extensions=[AnchorHeaderExtension()])}
  </body>
  </html>
  """
  with open(f'_build/help/index.html', mode='w') as f:
    f.write(html)

# -------------------------------------
# XML Parsing
# -------------------------------------

def headers_to_parse():
  return sorted([os.path.join(DOCS_PATH, f) for f in os.listdir(DOCS_PATH) if f.endswith('_8h.xml')])


def parse_file(filename):
  root = ET.parse(filename).getroot()
  desc = ''
  desc_node = root.find('./compounddef/detaileddescription/para')
  if desc_node is not None:
    desc = desc_node.text.strip()
  defs = sorted([d.text for d in root.findall('./compounddef/sectiondef/memberdef[@kind="define"]/name')])
  funcs = sorted([parse_function(f) for f in root.findall('./compounddef/sectiondef/memberdef[@kind="function"]')], key=lambda x: x.name)
  return File(description=desc, definitions=defs, functions=funcs)


def parse_function(func):
  return Function(
    description=func.find('./detaileddescription/para').text.strip() if func.find('./detaileddescription/para') is not None else '',
    type=parse_type((func.find('./type/ref') if func.find('./type/ref') is not None else func.find('./type')).text),
    name=func.find('./name').text,
    params=[parse_param(p, func.findall('./detaileddescription/para/parameterlist[@kind="param"]/parameteritem')) for p in func.findall('./param')],
    return_desc=func.find('./detaileddescription/para/simplesect[@kind="return"]/para').text.strip() if func.find('./detaileddescription/para/simplesect[@kind="return"]/para') is not None else '',
  )


def parse_param(param, parameteritems):
  name=param.find('./declname').text
  description=''
  for item in parameteritems:
    if item.find('./parameternamelist/parametername').text == name:
      description = item.find('./parameterdescription/para').text.strip()
      break
  return Param(
    type=parse_type((param.find('./type/ref') if param.find('./type/ref') is not None else param.find('./type')).text),
    name=name,
    description=description
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

def md_file(module, file):
  if not file:
    return ''
  str = f'## {module.capitalize()}\n\n'
  if file.description:
    str += file.description + '\n\n'
  if file.definitions:
    str += '### Constants\n' + '\n'.join([f'#### `{d}`' for d in file.definitions])
  if file.functions:
    if str:
      str += '\n\n'
    str += '### Functions\n' + '\n'.join([md_func(f) for f in file.functions])
  return str


def md_func(func):
  str = f'### `{func.name}({", ".join([f"{p.name}" for p in func.params])})`'
  if func.description:
    str += f'\n{func.description}'
  if func.params:
    str += '\n\nParameters:\n\n'
    for param in func.params:
      str += f'* {param.name} ({param.type})'
      if param.description:
        str += f': {param.description}'
      str += '\n'
  if func.type != 'void':
    str += f'\n\nReturns:\n\n* {func.type}'
    if func.return_desc:
      str += f' - {func.return_desc}'
    str += '\n'
  return str


if __name__ == '__main__':
  main()
