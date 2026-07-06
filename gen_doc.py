from dataclasses import dataclass
from html import escape
import os
from typing import List
import xml.etree.ElementTree as ET

DOCS_PATH = 'doc_out/xml'
OUTPUT_PATH = '_build/help'

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


def html_page(title, css_path, breadcrumbs_html, body_html):
  return f"""<!DOCTYPE html>
<html lang=\"en\">
<head>
  <meta charset=\"UTF-8\">
  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
  <link rel=\"stylesheet\" href=\"{css_path}\">
  <title>{escape(title)}</title>
</head>
<body>
  {f'<p>{breadcrumbs_html}</p>' if breadcrumbs_html else ''}
  {body_html}
</body>
</html>
"""


def write_html(path, content):
  with open(path, mode='w', encoding='utf-8') as f:
    f.write(content)


def function_signature(func):
  params = ', '.join([f'{param.type} {param.name}' for param in func.params])
  return f'{func.type} {func.name}({params})'


def module_page(module, file):
  title = f'ColdSteel - {module.capitalize()}'
  breadcrumbs = f'<a href="../index.html">Index</a> &gt; {escape(module.capitalize())}'
  body = [f'<h1>{escape(module.capitalize())}</h1>']

  if file.description:
    body.append(f'<p>{escape(file.description)}</p>')

  if file.definitions:
    body.append('<h2>Constants</h2>')
    body.append('<ul>')
    body.extend([f'<li>{escape(definition)}</li>' for definition in file.definitions])
    body.append('</ul>')

  if file.functions:
    body.append('<h2>Functions</h2>')
    body.append('<ul>')
    body.extend([
      f'<li><a href="../functions/{func.name.lower()}.html">{escape(function_signature(func))}</a></li>'
      for func in file.functions
    ])
    body.append('</ul>')

  return html_page(title, '../styles.css', breadcrumbs, '\n  '.join(body))


def function_page(module, func):
  module_name = module.capitalize()
  breadcrumbs = (
    f'<a href="../index.html">Index</a> &gt; '
    f'<a href="../modules/{module}.html">{escape(module_name)}</a> &gt; '
    f'{escape(func.name)}'
  )
  body = [
    f'<h1>{escape(func.name)}</h1>',
    f'<pre><code>{escape(function_signature(func))}</code></pre>'
  ]

  if func.description:
    body.append(f'<p>{escape(func.description)}</p>')

  if func.params:
    body.append('<h2>Parameters</h2>')
    body.append('<ul>')
    body.extend([
      f'<li><code>{escape(param.name)}</code> ({escape(param.type)}): {escape(param.description)}</li>'
      for param in func.params
    ])
    body.append('</ul>')

  if func.type != 'void':
    body.append('<h2>Returns</h2>')
    return_desc = f' - {escape(func.return_desc)}' if func.return_desc else ''
    body.append(f'<p>{escape(func.type)}{return_desc}</p>')

  return html_page(f'ColdSteel - {module_name} - {func.name}', '../styles.css', breadcrumbs, '\n  '.join(body))


def index_page(modules):
  rows = ['<h1>ColdSteel Engine Documentation</h1>', '<ul>']
  rows.extend([
    f'<li><a href="modules/{module}.html">{escape(module.capitalize())}</a></li>'
    for module, _ in modules
  ])
  rows.append('</ul>')
  return html_page('ColdSteel Engine Documentation', 'styles.css', '', '\n  '.join(rows))


def main():
  modules_path = os.path.join(OUTPUT_PATH, 'modules')
  functions_path = os.path.join(OUTPUT_PATH, 'functions')
  os.makedirs(modules_path, exist_ok=True)
  os.makedirs(functions_path, exist_ok=True)

  modules = []
  for header in headers_to_parse():
    module = os.path.basename(header[:header.find('_8h.xml')]).lower()
    file = parse_file(header)
    if not file:
      continue

    modules.append((module, file))
    write_html(os.path.join(modules_path, f'{module}.html'), module_page(module, file))
    for func in file.functions:
      write_html(os.path.join(functions_path, f'{func.name.lower()}.html'), function_page(module, func))

  write_html(os.path.join(OUTPUT_PATH, 'index.html'), index_page(modules))

# -------------------------------------
# XML Parsing
# -------------------------------------

def headers_to_parse():
  return sorted([os.path.join(DOCS_PATH, f) for f in os.listdir(DOCS_PATH) if f.endswith('_8h.xml') and f != 'common_8h.xml'])


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
  if type == 'Font *':
    return 'Font'
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
  if type == 'IParticleSystemSceneNode *':
    return 'ParticleSystem'
  if type == 'ITerrainSceneNode *':
    return 'Terrain'
  return type


if __name__ == '__main__':
  main()
