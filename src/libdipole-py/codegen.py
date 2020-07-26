import sys, os.path
import ast
from codegen_cpp import *

def find_import_file(import_name, pathes):
    ret = None
    for p in pathes:
        import_fn = os.path.join(p, import_name + ".py")
        if os.path.exists(import_fn):
            ret = import_fn
            break
    return ret

class Type:
    def __init__(self, py_type):
        self.py_type = py_type
        
    def get_cpp_type(self):
        if self.py_type == 'str':
            ret = 'std::string'
        elif self.py_type == None:
            ret = 'void'
        else:
            ret = self.py_type
        return ret

class ModuleDef:
    def __init__(self, imports, enums, structs, typedefs, interfaces):
        self.imports = imports
        self.enums = enums
        self.structs = structs
        self.typedefs = typedefs
        self.interfaces = interfaces

    def merge(self, o_mod_def):
        self.imports.extend(o_mod_def.imports)
        self.enums.extend(o_mod_def.enums)
        self.structs.extend(o_mod_def.structs)
        self.typedefs.extend(o_mod_def.typedefs)
        self.interfaces.extend(o_mod_def.interfaces)

    def dump(self):
        print("imports: ", [x for x in self.imports])
        print("enums:", [x.name for x in self.enums])
        print("structs:", [x.name for x in self.structs])
        print("interfaces:", [x.name for x in self.interfaces])
        print("typedefs:", [x.name for x in self.typedefs])

    def find_typedef(self, type_name):
        ret = None
        for typedef in self.typedefs:
            if typedef.name == type_name:
                ret = typedef
        return ret
        
        
class InterfaceDef:
    def __init__(self, name):
        self.name = name
        self.methods = []

class InterfaceMethodDef:
    def __init__(self, class_def, method_name):
        self.class_def = class_def
        self.method_name = method_name
        self.method_args = MethodArgs()
        self.method_ret_type = None # Type

class MethodArgs:
    def __init__(self):
        self.args = [] # List[MethodArg]

    def add_arg(self, arg_name, arg_type):
        self.args.append(MethodArg(arg_name, Type(arg_type)))
        
    def get_typed_args_list(self):
        ret = []
        for arg in self.args:
            arg_type = arg.arg_type.get_cpp_type()
            arg_name = arg.arg_name
            ret.append(f"{arg_type} {arg_name}")
        return ret

    def get_args_list(self):
        return [x.arg_name for x in self.args]            
        
class MethodArg:
    def __init__(self, arg_name, arg_type):
        self.arg_name = arg_name
        self.arg_type = arg_type
        
class TypedefDef:
    def __init__(self, name, typedef_container, typedef_element_type):
        self.name = name
        self.typedef_container = typedef_container
        self.typedef_element_type = typedef_element_type

class EnumDef:
    def __init__(self, name):
        self.name = name
        self.members = [] # name -> num value

class StructMemberDef:
    def __init__(self, name, type):
        if not isinstance(type, Type):
            raise Exception("StructMemberDef::__init__: type must be Type")
        self.name = name
        self.type = type
        
class StructDef:
    def __init__(self, name):
        self.name = name
        self.members = [] # List[StructMemberDef]

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
                        m_def.method_ret_type = Type(el.returns.id)
                    elif isinstance(el.returns, ast.NameConstant):
                        if el.returns.value == None:
                            m_def.method_ret_type = Type(None)
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
                        m_def.method_args.add_arg(m_arg_name, m_arg_type)
                        
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
                typedef_def = TypedefDef(typedef_target, 'ObjectPtr', node.value.slice.value.id)
            elif node.value.value.value.id == 'typing' and node.value.value.attr == 'List':
                typedef_def = TypedefDef(typedef_target, 'List', node.value.slice.value.id)
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
                    struct_def.members.append(StructMemberDef(m.target.id, Type(m.annotation.id)))
                        
    
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

def parse_module(pyidl_fn):
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
    import_defs = []
    if not isinstance(pt, ast.Module):
        raise Exception("top node in parsed tree expected to be Module")
    
    for node in ast.iter_child_nodes(pt):
        node_name = node.name if hasattr(node, 'name') else None
        print(node, type(node), node_name)
        #ipdb.set_trace()

        if isinstance(node, ast.ImportFrom):
            if node.module.find("_idl") != -1:
                print("PYIDL module from import found: ", node.module)
                import_defs.append(node.module)
                
        if isinstance(node, ast.Import):
            for n in node.names:
                if n.name.find("_idl") != -1:
                    if n.name != 'dipole_idl':
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
    return ModuleDef(import_defs, enum_defs, struct_defs, typedef_defs, interface_defs)
