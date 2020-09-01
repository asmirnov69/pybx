import typing

class interface: pass
ROP = typing._alias(object, typing.T, inst = False)
class rop_impl_base: pass

interface_rop_types = {} # interface type -> interface rop type
inv_interface_rop_types = {} # interface rop type -> interface type
def register_interface_rop_type(interface_type, interface_rop_type):
    interface_rop_types[interface_type] = interface_rop_type
    inv_interface_rop_types[interface_rop_type] = interface_type

def get_interface_rop_type(interface_type):
    return interface_rop_types[interface_type]

def get_interface_type(interface_rop_type):
    return inv_interface_rop_types[interface_rop_type]

method_defs = {} # method signature -> method def
def register_method_def(method_signature, method_def):
    method_defs[method_signature] = method_def

def get_method_def(method_signature):
    return method_defs[method_signature]
