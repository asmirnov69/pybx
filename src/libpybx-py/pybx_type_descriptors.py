interface_ptr_types = {} # interface type -> interface ptr type
inv_interface_ptr_types = {} # interface ptr type -> interface type
def register_interface_ptr_type(interface_type, interface_ptr_type):
    interface_ptr_types[interface_type] = interface_ptr_type
    inv_interface_ptr_types[interface_ptr_type] = interface_type

def get_interface_ptr_type(interface_type):
    return interface_ptr_types[interface_type]

def get_interface_type(interface_ptr_type):
    return inv_interface_ptr_types[interface_ptr_type]

method_descriptors = {} # method signature -> MethodDescriptor
class MethodDescriptor:
    def __init__(self, method_name, arg_ann_types, ret_ann_type):
        self.method_name = method_name
        self.arg_ann_types = arg_ann_types
        self.ret_ann_type = ret_ann_type
        
def register_method_descriptor(method_signature, method_name, arg_ann_types, ret_ann_type):
    method_descriptors[method_signature] = MethodDescriptor(method_name, arg_ann_types, ret_ann_type)
def get_method_descriptor(method_signature):
    return method_descriptors[method_signature]
