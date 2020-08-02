import ipdb
import pybx, pybx_type_descriptors as pybx_td
import dataclasses, typing, enum, inspect

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
    elif isinstance(o, pybx.ptr_impl_base):
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
    if hasattr(o_ann_type, '_name'):
        if o_ann_type._name == 'List':
            ret = []
            el_type = o_ann_type.__args__[0]
            for o_json_el in o_json:
                ret.append(from_json(o_json_el, el_type))
        elif o_ann_type._name == 'object':
            #ipdb.set_trace()
            interface_type = o_ann_type.__args__[0]
            assert(o_json['__interface_type'] == f"{interface_type.__module__}.{interface_type.__name__}")
            ptr_type = pybx_td.get_interface_ptr_type(interface_type)
            ret = ptr_type(None, None, o_json['object_id'])
        else:
            raise Exception(f"unhandled ann type {o_ann_type._name}")
    elif dataclasses.is_dataclass(o_ann_type):
        dflt_args = {f.name:None for f in dataclasses.fields(o_ann_type)}
        ret = o_ann_type(**dflt_args)
        for f, f_type in typing.get_type_hints(o_ann_type).items():
            setattr(ret, f, from_json(o_json[f], f_type))
    elif inspect.isclass(o_ann_type) and issubclass(o_ann_type, enum.Enum):
        ret = o_ann_type(o_ann_type[o_json])
    else:
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
    
