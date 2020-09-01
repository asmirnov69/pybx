import ipdb
import pybx_type_descriptors as pybx_td
import dataclasses, typing, enum, inspect
from pybx_parser import Type

def dataclass_to_json(o):
    ret = {}
    for m in o.__dict__:
        v = getattr(o, m)
        ret[m] = to_json(v)
    return ret

def to_json__(o):
    if isinstance(o, list):
        ret = []
        for el in o:
            ret.append(to_json__(el))
    elif dataclasses.is_dataclass(o):
        ret = dataclass_to_json(o)
    elif isinstance(o, enum.Enum):
        ret = o.name
    elif isinstance(o, pybx_td.rop_impl_base):
        #ipdb.set_trace()
        interface_type = pybx_td.get_interface_type(type(o))
        type_s = f"{interface_type.__module__}.{interface_type.__name__}"
        ret = {'__interface_type': type_s, 'object_id': o.object_id}
    else:
        ret = o

    return ret
    
def to_json(o):
    return to_json__(o)

def from_json(o_json, o_ann_type):
    #ipdb.set_trace()
    if not isinstance(o_ann_type, Type):
        o_ann_type = Type(o_ann_type)

    if o_ann_type.is_vector_type():
        ret = []
        el_type = o_ann_type.py_type.__args__[0]
        for o_json_el in o_json:
            ret.append(from_json(o_json_el, el_type))
    elif o_ann_type.is_rop_type():
        #ipdb.set_trace()
        interface_type = o_ann_type.py_type.__args__[0]
        assert(o_json['__interface_type'] == f"{interface_type.__module__}.{interface_type.__name__}")
        rop_type = pybx_td.get_interface_rop_type(interface_type)
        ret = rop_type(None, None, o_json['object_id'])
    elif o_ann_type.is_struct():
        dflt_args = {f.name:None for f in dataclasses.fields(o_ann_type.py_type)}
        ret = o_ann_type.py_type(**dflt_args)
        for f, f_type in typing.get_type_hints(o_ann_type.py_type).items():
            setattr(ret, f, from_json(o_json[f], f_type))
    elif o_ann_type.is_enum():
        #ipdb.set_trace()
        ret = o_ann_type.py_type[o_json]
    else: # is_interface to be implemented
        print("from_json:", o_json, o_ann_type)
        #ipdb.set_trace()
        ret = o_json
        #else:
        #    raise Exception(f"json value {o_json} is not {o_ann_type}")

    return ret
    
class E(enum.Enum):
    E_AA = 0
    E_AB = enum.auto()

@dataclasses.dataclass
class C:
    c: int
    e: typing.List[E]

@dataclasses.dataclass
class D:
    i: int
    j: float
    k: C
    
if __name__ == "__main__":
    c = C(c = 22, e = [E.E_AA, E.E_AB])
    d = D(i = 1, j = 2, k = c)
    l = [d, d]
    print(l)
    print(to_json(l))

    l_json = to_json(l)
    print(from_json(l_json, typing.List[D]))
    
