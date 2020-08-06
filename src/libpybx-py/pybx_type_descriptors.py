import typing

class interface: pass
ObjectPtr = typing._alias(object, typing.T, inst = False)
class ptr_impl_base: pass

interface_ptr_types = {} # interface type -> interface ptr type
inv_interface_ptr_types = {} # interface ptr type -> interface type
def register_interface_ptr_type(interface_type, interface_ptr_type):
    interface_ptr_types[interface_type] = interface_ptr_type
    inv_interface_ptr_types[interface_ptr_type] = interface_type

def get_interface_ptr_type(interface_type):
    return interface_ptr_types[interface_type]

def get_interface_type(interface_ptr_type):
    return inv_interface_ptr_types[interface_ptr_type]

method_defs = {} # method signature -> method def
def register_method_def(method_signature, method_def):
    method_defs[method_signature] = method_def

def get_method_def(method_signature):
    return method_defs[method_signature]
