import ipdb
import importlib.machinery, os.path, sys, types
import typing
import pybx_type_descriptors as pybx_td
import pybx_json

class interface: pass
ObjectPtr = typing._alias(object, typing.T, inst = False)

def import_pybx(fn):
    mod_name = os.path.basename(fn).split(".")[0]
    absolute_name = importlib.util.resolve_name(mod_name, None)
    print("absolute_name:", absolute_name)
    if absolute_name in sys.modules:
        mod = sys.modules[absolute_name]
    else:
        loader = importlib.machinery.SourceFileLoader(mod_name, fn)
        mod = types.ModuleType(loader.name)
        sys.modules[absolute_name] = mod
        loader.exec_module(mod)
    return mod

