from typing import List
import os

class Var:
    def __init__(self, name: str, type_: str, value: str):
        self.name = name
        self.type = type_
        self.value = value
    
    @staticmethod
    def parse(s: str) -> 'Var':
        split = s.split(' ')
        if len(split) != 2 and len(split) != 3:
            raise Exception('Malformed declaration: ' + s)
        type_ = split[0].strip()
        name = split[1].strip()
        value = split[2].strip() if len(split) == 3 else ''
        return Var(name, type_, value)


class Func:
    def __init__(self, name: str, type_: str, params: List[Var]):
        self.name = name
        self.type = type_
        self.params = params
    
    @staticmethod
    def parse(s: str) -> 'Func':
        first_split = s.split('(')
        if len(first_split) != 2:
            raise Exception('Malformed declaration: ' + s)
        split = first_split[0].split(' ')
        if len(split) != 2:
            raise Exception('Malformed declaration: ' + s)
        type_ = split[0].strip()
        name = split[1].strip()
        params_str = first_split[1][:-1].strip()
        params_split = params_str.split(',') \
            if params_str != '' \
            else None
        params = [Var.parse(p.strip()) for p in params_split] \
            if params_split is not None \
            else []
        return Func(name, type_, params)


class Defs:
    def __init__(self, vars: List[Var], funcs: List[Func]):
        self.vars = vars
        self.funcs = funcs
    
    def as_wrapper(self) -> str:
        custom = self.custom_types()
        vars_list = ['const {} _{} = {};'.format(v.type, v.name, v.value) for v in self.vars]
        funcs_list = ['void _{name}(asIScriptGeneric* gen) {{\n{args}{call}\n}}'.format(
            name=f.name,
            args=Defs.as_args(f.params),
            call=Defs.as_funccall(f)
        ) for f in self.funcs]
        regtypes_list = ['    engine->RegisterObjectType("{}", 0, asOBJ_REF | asOBJ_NOCOUNT);'.format(Defs.as_type(t, custom)[:-1]) for t in custom + ['VoidPtr@']]
        regdowncast_list = ['    engine->RegisterObjectMethod("Entity", "{}@ opCast()", asFUNCTION(CastEntity), asCALL_GENERIC);'.format(s) for s in Defs.entity_subclasses()]
        regupcast_list = ['    engine->RegisterObjectMethod("{}", "Entity@ opImplCast()", asFUNCTION(CastEntity), asCALL_GENERIC);'.format(s) for s in Defs.entity_subclasses()]
        regvars_list = ['    engine->RegisterGlobalProperty("const {} {}", (void*)&_{});'.format(v.type, v.name[2:], v.name) for v in self.vars]
        regfuncs_list = ['    engine->RegisterGlobalFunction("{} {}{}", asFUNCTION(_{}), asCALL_GENERIC);'.format(
            Defs.as_type(f.type, custom),
            f.name[2:],
            Defs.as_params(f.params, custom),
            f.name
        ) for f in self.funcs]
        includes = '#include <string>\n#include "../lib/angelscript/include/angelscript.h"\n#include "engine.h"\n\n'
        vars = '\n'.join(vars_list) + '\n\n'
        funcs = '\n\n'.join(funcs_list) + '\n\n'
        cast_func = 'void CastEntity(asIScriptGeneric* gen) { gen->SetReturnAddress(gen->GetObject()); }\n\n'
        func_header = 'void RegisterAstro(asIScriptEngine* engine) {\n'
        reg_types = '\n'.join(regtypes_list) + '\n'
        reg_downcast = '\n'.join(regdowncast_list) + '\n'
        reg_upcast = '\n'.join(regupcast_list) + '\n'
        reg_vars = '\n'.join(regvars_list) + '\n'
        reg_funcs = '\n'.join(regfuncs_list) + '\n'
        func_footer = '}\n'
        return includes + vars + funcs + cast_func + func_header + reg_types + reg_downcast + reg_upcast + reg_vars + reg_funcs + func_footer
    
    def custom_types(self) -> List[str]:
        std_types = ('void', 'bool', 'int', 'float', 'string', 'void*')
        func_types = [f.type for f in self.funcs if f.type not in std_types]
        param_types = [p.type for f in self.funcs for p in f.params if p.type not in std_types]
        return list(set(func_types + param_types))

    @staticmethod
    def as_params(params: List[Var], custom_types: List[str]) -> str:
        params_str = ['{} {}'.format(Defs.as_type(p.type, custom_types), p.name) for p in params]
        return '({})'.format(', '.join(params_str))
    
    @staticmethod
    def as_args(params: List[Var]) -> str:
        args_str = ['    {type} arg{index} = {cast_type}gen->GetArg{as_type}({index});'.format(
            type=Defs.irr_type(p.type),
            index=i,
            cast_type= '({})'.format(Defs.irr_type(p.type)) if p.type != 'string' else '*(std::string*)',
            as_type=Defs.as_arg_type(p.type)
        ) for i, p in enumerate(params)]
        return '\n'.join(args_str) + ('\n' if len(params) > 0 else '')

    @staticmethod
    def as_type(type_: str, custom_types: List[str]) -> str:
        if type_ in custom_types:
            return type_ + '@'
        elif type_ == 'void*':
            return 'VoidPtr@'
        else:
            return type_
    
    @staticmethod
    def as_arg_type(type_: str) -> str:
        arg_types = {
            'bool': 'Byte',
            'int': 'DWord',
            'float': 'Float',
            'string': 'Object'
        }
        return arg_types[type_] if type_ in arg_types else 'Address'
    
    @staticmethod
    def as_funccall(f: Func) -> str:
        result = '{} result = '.format(Defs.irr_type(f.type)) if f.type != 'void' else ''
        call = '{}({});'.format(f.name, ', '.join(['arg' + str(i) + ('.c_str()' if p.type == 'string' else '') for i, p in enumerate(f.params)]))
        return_ = ' gen->SetReturn{}({}result);'.format(Defs.as_arg_type(f.type), '&' if f.type == 'string' else '') if f.type != 'void' else ''
        return '    ' + result + call + return_
    
    @staticmethod
    def irr_type(type_: str) -> str:
        irr_types = {
            'string': 'std::string',
            'Camera': 'ICameraSceneNode*',
            'Control': 'IGUIElement*',
            'Entity': 'ISceneNode*',
            'Font': 'IGUIFont*',
            'Light': 'ILightSceneNode*',
            'Material': 'SMaterial*',
            'Memblock': 'Memblock*',
            'Mesh': 'IMesh*',
            'MeshBuffer': 'IMeshBuffer*',
            'MeshEntity': 'IMeshSceneNode*',
            'Sprite': 'IBillboardSceneNode*',
            'Sound': 'Sound*',
            'Texture': 'ITexture*',
            'XMLNode': 'XMLNode*',
        }
        return irr_types[type_] if type_ in irr_types else type_
    
    @staticmethod
    def entity_subclasses() -> List[str]:
        return ['Camera', 'Font', 'Light', 'Material', 'MeshEntity', 'Sprite', 'Texture']


def parse_defs(filename: str) -> Defs:
    with open(filename) as f:
        raw_lines = f.readlines()
    stripped_lines = [l.strip() for l in raw_lines]
    lines = [l for l in stripped_lines if l != '' and not l.startswith('#')]
    vars = [Var.parse(l) for l in lines if not l.endswith(')')]
    funcs = [Func.parse(l) for l in lines if l.endswith(')')]
    return Defs(vars, funcs)


def write_file(filename: str, content: str) -> None:
    full_path = os.path.abspath(filename)
    path = os.path.dirname(full_path)
    if not os.path.exists(path):
        os.makedirs(path)
    with open(full_path, 'wb') as f:
        f.write(content.encode())


if __name__ == '__main__':
    defs = parse_defs('defs.txt')
    write_file('src/as_wrapper.cpp', defs.as_wrapper())
