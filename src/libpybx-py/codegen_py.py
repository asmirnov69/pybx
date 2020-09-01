import ipdb
import pybx_json
from pybx_parser import Type
import uuid, json, asyncio, inspect, io, typing
import pybx_type_descriptors as pybx_td

show_gen_code = False

rop_class_def_code_tmpl = """
class rop_impl_class(pybx_td.rop_impl_base):
    def __init__(self, comm, ws, object_id):
        self.comm = comm
        self.ws = ws
        self.object_id = object_id
"""

rop_class_method_def_code_tmpl = """
    async def {method_name}(self, {method_args_l}):
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

def generate_rop_classes(pybx_mod, mod_defs, globals_ns):
    for interface_def in mod_defs.interfaces:
        gen_code = io.StringIO()
        print(rop_class_def_code_tmpl, file = gen_code)
        interface_class_name = interface_def.name
        for m in interface_def.methods:
            method_name = m.name
            method_signature = f"{interface_class_name}__{method_name}"

            dd = {}
            dd['method_signature'] = method_signature
            dd['method_name'] = method_name
            dd['return_class_name'] = m.get_method_return_type().get_py_code_name()

            method_args = m.get_method_args()
            args_list = ["'%s': pybx_json.to_json(%s)" % (x,x) for x in method_args]
            dd['method_args_d'] = ",".join(args_list)
            dd['method_args_l'] = ",".join([x for x in method_args])

            print(rop_class_method_def_code_tmpl.format(**dd), file = gen_code)

        if show_gen_code:
            print(gen_code.getvalue())

        nn = {}
        #ipdb.set_trace()
        exec(gen_code.getvalue(), globals_ns, nn)
        new_class_obj = nn['rop_impl_class']
        rop_class_name = f"rop_impl_{interface_class_name}"
        setattr(new_class_obj, '__name__', rop_class_name)
        setattr(new_class_obj, '__qualname__', rop_class_name)
        setattr(new_class_obj, '__module__', pybx_mod.__name__)
        setattr(pybx_mod, rop_class_name, new_class_obj)            

        pybx_td.register_interface_rop_type(interface_def.def_type, new_class_obj)
        for m in interface_def.methods:
            method_signature = f"{interface_class_name}__{m.name}"
            pybx_td.register_method_def(method_signature, m)

