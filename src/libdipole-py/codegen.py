import sys
import ast
from codegen_cpp import *

class ModuleDef:
    def __init__(self, enums, structs, typedefs, interfaces):
        self.enums = enums
        self.structs = structs
        self.typedefs = typedefs
        self.interfaces = interfaces

class InterfaceDef:
    def __init__(self, name):
        self.name = name
        self.methods = []

class InterfaceMethodDef:
    def __init__(self, class_def, method_name):
        self.class_def = class_def
        self.method_name = method_name
        self.method_args = []
        self.method_ret_type = None
        
class TypedefDef:
    def __init__(self, name, typedef_value):
        self.name = name
        self.typedef_value = typedef_value

class EnumDef:
    def __init__(self, name):
        self.name = name
        self.members = [] # name -> num value
        
class StructDef:
    def __init__(self, name):
        self.name = name
        self.members = [] # name -> type

def parse_dipole_interface(node):
    interface_def = None
    if isinstance(node, ast.ClassDef):
        if len(node.bases) == 1: # otherwise it is forward declaration
            if node.bases[0].value.id == 'dipole_idl' and node.bases[0].attr == 'interface':
                interface_def = InterfaceDef(node.name)
                print("parse_dipole_interface: ", node.name)
                for el in node.body:
                    if not isinstance(el, ast.FunctionDef):
                        raise Exception("interface expected to have methods only")
                    if el.body[0].value.elts != []:
                        raise Exception("method declaration with non-empty body")
                    m_def = InterfaceMethodDef(interface_def, el.name)
                    #ipdb.set_trace()
                    if isinstance(el.returns, ast.Name):
                        m_def.method_ret_type = el.returns.id
                    elif isinstance(el.returns, ast.NameConstant):
                        if el.returns.value == None:
                            m_def.method_ret_type = None
                        else:
                            raise Exception("unexpected value of NameConstant")
                                
                    for i in range(len(el.args.args)):
                        m_arg = el.args.args[i]
                        if i == 0:
                            if m_arg.arg != 'self':
                                raise Exception("first arg must be self")
                            if m_arg.annotation != None:
                                raise Exception("first arg should not have annotation")
                            continue
                        m_arg_name = m_arg.arg
                        m_arg_type = m_arg.annotation.id
                        m_def.method_args.append((m_arg_name, m_arg_type))
                        
                    #ipdb.set_trace()
                    interface_def.methods.append(m_def)
        else:
            if node.body[0].value.elts == []:
                pass
            else:
                raise Exception("forward class declaration body is not empty")
            
    return interface_def

def parse_dipole_typedef(node):
    typedef_def = None
    if isinstance(node, ast.Assign):
        if len(node.targets) == 1:
            if isinstance(node.targets[0], ast.Name):
                typedef_target = node.targets[0].id
        if isinstance(node.value, ast.Subscript):
            if node.value.value.value.id == 'dipole_idl' and node.value.value.attr == 'ObjectPtr':
                typedef_source = ('ObjectPtr', node.value.slice.value.id)
                typedef_def = TypedefDef(typedef_target, typedef_source)
            elif node.value.value.value.id == 'typing' and node.value.value.attr == 'List':
                typedef_source = ('List', node.value.slice.value.id)
                typedef_def = TypedefDef(typedef_target, typedef_source)
            else:
                raise Exception(f"can't process typedef at line {node.lineno}")

    return typedef_def

def parse_dipole_struct(node):
    struct_def = None
    if isinstance(node, ast.ClassDef):
        if len(node.bases) == 1: # otherwise it is forward declaration
            if node.bases[0].value.id == 'dipole_idl' and node.bases[0].attr == 'struct':
                struct_def = StructDef(node.name)
                for m in node.body:
                    if not isinstance(m, ast.AnnAssign):
                        raise Exception(f"parse_dipole_struct: unexpected member at line {m.lineno}")
                    struct_def.members.append((m.target.id, m.annotation.id))
                        
    
    return struct_def

def parse_dipole_enumdef(node):
    enumdef_def = None
    if isinstance(node, ast.ClassDef):
        if len(node.bases) == 1: # otherwise it is forward declaration
            if node.bases[0].value.id == 'enum' and node.bases[0].attr == 'Enum':
                enumdef_def = EnumDef(node.name)
                for m_def in node.body:
                    if isinstance(m_def, ast.Assign):
                        if hasattr(m_def.value, 'n'):
                            member = (m_def.targets[0].id, m_def.value.n)
                        elif hasattr(m_def.value, 'func'):
                            if m_def.value.func.attr == 'auto':
                                member = (m_def.targets[0].id, None)
                            else:
                                raise Exception("expected auto")
                        else:
                            raise Exception("expected either num or auto")
                        enumdef_def.members.append(member)
    
    return enumdef_def

def parse_file(pyidl_fn):
    source_code = "\n".join(open(pyidl_fn).readlines())

    #print source_code[:100]
    pt = ast.parse(source_code)
    #ipdb.set_trace()
    #print(ast.dump(pt))
    #return None

    typedef_defs = []
    enum_defs = []
    struct_defs = []
    interface_defs = []
    if not isinstance(pt, ast.Module):
        raise Exception("top node in parsed tree expected to be Module")
    
    for node in ast.iter_child_nodes(pt):
        node_name = node.name if hasattr(node, 'name') else None
        print(node, type(node), node_name)
        #ipdb.set_trace()

        if isinstance(node, ast.ImportFrom):
            if node.module.find("_idl") != -1:
                print("PYIDL module from import found: ", node.module)
        if isinstance(node, ast.Import):
            for n in node.names:
                if n.name.find("_idl") != -1:
                    if n.name != 'dipole_idl':
                        print("PYIDL module import found: ", n.name)
                        raise Exception(f"'import {n.name}' is not supported, use 'from {n.name} import *' instead")
        
        interface_def = parse_dipole_interface(node)
        if interface_def:
            interface_defs.append(interface_def)
            continue

        struct_def = parse_dipole_struct(node)
        if struct_def:
            struct_defs.append(struct_def)
            continue
        
        typedef_def = parse_dipole_typedef(node)
        if typedef_def:
            typedef_defs.append(typedef_def)
            continue

        enum_def = parse_dipole_enumdef(node)
        if enum_def:
            enum_defs.append(enum_def)
            continue

    #ipdb.set_trace()
    #print("walk is done")
    return ModuleDef(enum_defs, struct_defs, typedef_defs, interface_defs)
