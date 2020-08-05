import ipdb
import importlib.machinery, os.path, sys, types, io
import typing, inspect, uuid, json, asyncio
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

    #ipdb.set_trace()
    globals_ns = inspect.stack()[1][0].f_globals
    globals_ns[absolute_name] = mod
    register_ptr_class_impls(mod, globals_ns)
    #return mod

def register_ptr_class_impls(pybx_mod, globals_ns):
    for class_name in dir(pybx_mod):
        interface_obj = getattr(pybx_mod, class_name)
        if inspect.isclass(interface_obj) and issubclass(interface_obj, interface):
            register_ptr_class_impl(pybx_mod, interface_obj, class_name, globals_ns)

ptr_class_def_code_tmpl = """
class ptr_impl_class(pybx_td.ptr_impl_base):
    def __init__(self, comm, ws, object_id):
        self.comm = comm
        self.ws = ws
        self.object_id = object_id
"""

ptr_class_method_def_code_tmpl = """
    async def {method_name}({method_args_l}):
        message_json = {{
            'message-type': 'method-call',
            'method-signature': '{method_signature}',
            'message-id': str(uuid.uuid1()),
            'object-id': self.object_id,
            'args': {{{method_args_d}}}
        }}

        print("send:", message_json)
        await self.ws.send(json.dumps(message_json))
        message_id = message_json['message-id']
        result_fut = asyncio.Future()
        loop = asyncio.get_event_loop()
        self.comm.add_call_waiter__(message_id, result_fut, loop)

        res_message_json = await result_fut
        print('res_message_json:', res_message_json['retval']['retval'])
        ret = pybx_json.from_json(res_message_json['retval']['retval'], {return_class_name})
        return ret
"""

def register_ptr_class_impl(pybx_mod, interface_obj, interface_class_name, globals_ns):
    #ipdb.set_trace()
    interface_methods = inspect.getmembers(interface_obj, predicate=inspect.isfunction)
    ptr_class_code = io.StringIO()
    print(ptr_class_def_code_tmpl, file = ptr_class_code)
        
    for interface_method in interface_methods:
        method_name, method_func = interface_method
        th = typing.get_type_hints(method_func)
        #print(class_name, method_name, th)
        method_args_d = dict(th); del method_args_d['return']
        method_signature = f"{interface_class_name}__{method_name}"
        pybx_td.register_method_descriptor(f"{interface_class_name}__{method_name}", f"{method_name}", method_args_d, th['return'])

        dd = {}
        dd['method_signature'] = method_signature
        dd['method_name'] = method_name
        if th['return'] == type(None):
            dd['return_class_name'] = 'None'
        else:
            dd['return_class_name'] = f"{th['return'].__module__}.{th['return'].__name__}"
        args_list = ["'%s': pybx_json.to_json(%s)" % (x,x) for x  in method_args_d.keys()]
        dd['method_args_d'] = ",".join(args_list)
        method_sig = inspect.signature(method_func)
        dd['method_args_l'] = ",".join([x for x in method_sig.parameters])
        print(ptr_class_method_def_code_tmpl.format(**dd), file = ptr_class_code)

    #ipdb.set_trace()
    nn = {}
    gen_code = ptr_class_code.getvalue()
    exec(gen_code, globals_ns, nn)
    new_class_obj = nn['ptr_impl_class']
    ptr_class_name = f"ptr_impl_{interface_class_name}"
    setattr(new_class_obj, '__name__', ptr_class_name)
    setattr(new_class_obj, '__qualname__', ptr_class_name)
    setattr(new_class_obj, '__module__', pybx_mod.__name__)
    setattr(pybx_mod, ptr_class_name, new_class_obj)
    pybx_td.register_interface_ptr_type(interface_obj, new_class_obj)
    
