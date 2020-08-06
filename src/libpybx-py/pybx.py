#import ipdb
import importlib.machinery, os.path, sys, types
import inspect
import codegen_py
from pybx_type_descriptors import interface, ObjectPtr

pybx_path = ["."]

def find_pybx_module_file(pybx_mod_name):
    ret = None
    pybx_fn = pybx_mod_name + ".pybx"
    for p in pybx_path:
        pn = os.path.join(p, pybx_fn)
        #print(pn)
        if os.path.exists(pn):
            ret = os.path.abspath(pn)
            break
    return ret

def import_pybx(pybx_mod_name, do_ptr_class_impls_registration = True):
    #absolute_name = importlib.util.resolve_name(pybx_mod_name, None)
    #print("absolute_name:", absolute_name)
    absolute_name = pybx_mod_name

    if absolute_name in sys.modules:
        mod = sys.modules[absolute_name]
    else:
        fn = find_pybx_module_file(pybx_mod_name)
        print(f"pybx module {pybx_mod_name} -> {fn}")
        loader = importlib.machinery.SourceFileLoader(pybx_mod_name, fn)
        mod = types.ModuleType(loader.name)
        sys.modules[absolute_name] = mod
        loader.exec_module(mod)

    globals_ns = inspect.stack()[1][0].f_globals
    globals_ns[absolute_name] = mod
        
    #ipdb.set_trace()
    if do_ptr_class_impls_registration:
        codegen_py.register_ptr_class_impls(mod, globals_ns)
    #return mod

