import ipdb
import sys, os.path
import inspect, typing, pybx
import dataclasses, enum

class ModuleDef:
    def __init__(self):
        self.interfaces = []
        self.structs = []
        self.enums = []

    def dump(self):
        print("interfaces:", [x.name for x in self.interfaces])
        print("structs:", [x.name for x in self.structs])
        print("enums:", [x.name for x in self.enums])

        for d in self.interfaces:
            d.dump()
        for s in self.structs:
            s.dump()
        for e in self.enums:
            e.dump()
            
class InterfaceDef:
    def __init__(self, def_type):
        self.def_type = def_type
        self.name = self.def_type.__name__
        self.methods = []

    def dump(self):
        print(f"interface {self.def_type}")
        for m in self.methods:
            m.dump()

class Type:
    def __init__(self, py_type):
        self.py_type = py_type

    def __repr__(self):
        ret = "Type " + repr(self.py_type)
        return ret

    def is_none(self):
        return self.py_type == type(None)
    
    def is_vector_type(self):
        return hasattr(self.py_type, '_name') and self.py_type._name == 'List'

    def is_ptr_type(self):
        return hasattr(self.py_type, '_name') and self.py_type._name == 'object'

    def is_struct(self):
        return dataclasses.is_dataclass(self.py_type)

    def is_enum(self):
        return inspect.isclass(self.py_type) and issubclass(self.py_type, enum.Enum)
    
    def get_py_code_name(self):
        t = self.py_type
        if t == type(None):
            ret = "type(None)"
        elif inspect.isclass(t):
            ret = f"{t.__module__}.{t.__name__}"
        elif hasattr(t, '__origin__'):
            #ipdb.set_trace()
            args_s = ",".join([f"{c.__module__}.{c.__name__}" for c in t.__args__])
            ret = f"{t.__module__}.{t._name}[{args_s}]"
            #ipdb.set_trace()
        else:
            raise Exception(f"can't get py_code_name for type {self.py_type}")
        return ret

class InterfaceMethodDef:
    def __init__(self, interface_def, method_name, type_hints, sig):
        self.interface_def = interface_def
        self.name = method_name
        self.type_hints = type_hints
        self.sig = sig

    def get_method_args(self):
        return [x for x in self.sig.parameters if x != 'self']

    def get_method_typed_args(self):
        return [f"{x[1]} {x[0]}" for x in zip(self.get_method_args(), self.get_method_arg_types())]
    
    def get_method_arg_types(self):
        args = self.get_method_args()
        return [Type(self.type_hints[x]) for x in args]

    def get_method_return_type(self):
        return Type(self.type_hints['return'])
    
    def dump(self):
        print(f" method {self.name}")
        m_args = self.get_method_args()
        m_arg_types = self.get_method_arg_types()
        m_ret_type = self.get_method_return_type()
        print(f"  arguments: {m_args}")
        print(f"  argument types: {m_arg_types}")
        print(f"  return type: {m_ret_type}")

class StructDef:
    def __init__(self, def_type):
        self.def_type = def_type
        self.name = self.def_type.__name__
        self.fields = [] # List[StructMemberDef]

    def dump(self):
        print(f"struct {self.def_type}")
        for m in self.fields:
            m.dump()
        
class StructMemberDef:
    def __init__(self, struct_def, field, field_type):
        self.struct_def = struct_def
        self.field = field
        self.field_type = Type(field_type)

    def get_member_name(self):
        return self.field.name

    def get_member_type(self):
        return self.field_type

    def dump(self):
        m_name = self.get_member_name()
        m_type = self.get_member_type()
        print(f" member: {m_name}")
        print(f" member type: {m_type}")

class EnumDef:
    def __init__(self, enum_def):
        self.def_type = enum_def
        self.name = enum_def.__name__        
        self.members = []
        self.member_values = []
        #ipdb.set_trace()
        for n, m in enum_def.__members__.items():
            self.members.append(n)
            self.member_values.append(m.value)

    def dump(self):
        print(f"enum {self.def_type}")
        for m, m_v in zip(self.members, self.member_values):
            print(f" enum member {m} {m_v}")

def parse_pybx_interface(interface_class):
    #ipdb.set_trace()
    print("parse_pybx_interface: ", interface_class.__name__)
    interface_def = InterfaceDef(interface_class)
    methods = inspect.getmembers(interface_class, predicate=inspect.isfunction)
    for m_name, m_func in methods:
        th = typing.get_type_hints(m_func)
        sig = inspect.signature(m_func)
        m_def = InterfaceMethodDef(interface_def, m_name, th, sig)
        interface_def.methods.append(m_def)
    return interface_def

def parse_pybx_struct(struct):
    struct_def = StructDef(struct)
    #ipdb.set_trace()
    dataclass_th = typing.get_type_hints(struct)
    for f in dataclasses.fields(struct):
        f_type = dataclass_th[f.name]
        f_def = StructMemberDef(struct_def, f, f_type)
        struct_def.fields.append(f_def)
    return struct_def

def parse_pybx_enum(enum_def):
    enumdef_def = EnumDef(enum_def)
    return enumdef_def

def parse_module(mod):
    #ipdb.set_trace()
    if not inspect.ismodule(mod):
        raise Exception("mod expected to be Module")

    mod_def = ModuleDef()
    for mod_el_name in dir(mod):
        mod_el = getattr(mod, mod_el_name)
        if inspect.isclass(mod_el) and issubclass(mod_el, pybx.interface):
            mod_def.interfaces.append(parse_pybx_interface(mod_el))
        elif dataclasses.is_dataclass(mod_el):
            mod_def.structs.append(parse_pybx_struct(mod_el))
        elif inspect.isclass(mod_el) and issubclass(mod_el, enum.Enum):
            mod_def.enums.append(parse_pybx_enum(mod_el))

    return mod_def
