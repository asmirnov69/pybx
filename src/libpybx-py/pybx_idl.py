import typing, inspect, io
# pybx primitive definitions

class interface: pass
ObjectPtr = typing._alias(object, typing.T, inst = False)

def generate_ptr_class_code(interface_class):
    out = io.StringIO()
    ptr_class_name = interface_class.__name__ + "Ptr"
    print(f"class {ptr_class_name}:", file = out)
    print(f"    def __init__(self, ws_handler, object_id):", file = out)
    print(f"        self.ws_handler = ws_handler", file = out)
    print(f"        self.object_id = object_id", file = out)
    
    for member in inspect.getmembers(interface_class, predicate = inspect.isfunction):
        method_name = member[0]
        signature = inspect.signature(member[1])
        method_signature = f"{interface_class.__name__}__{method_name}"
        args = ", ".join(signature.parameters)
        arg_values = ", ".join(["'{arg}': {arg}".format(arg = x) for x in signature.parameters if x != 'self'])
        print(f"    async def {method_name}({args}):", file = out)
        print(f"        message_json = {{'message-type': 'method-call', 'method-signature': '{method_signature}', 'message-id': str(uuid.uuid1()), 'object-id': self.object_id, 'args': {{ {arg_values} }} }}", file = out)
        print(f"        await self.ws_handler.ws.send(json.dumps(message_json, cls = EnhancedJSONEncoder))", file = out)
        print(f"        message_id = message_json['message-id']", file = out)
        print(f"        result_fut = asyncio.Future()", file = out)
        print(f"        loop = asyncio.get_event_loop()", file = out)
        print(f"        self.ws_handler.comm.add_call_waiter__(message_id, result_fut, loop)", file = out)
        print(f"        res_message_json = await result_fut", file = out)
        print(f"        return res_message_json['retval']['ret']", file = out)

        #for p in signature.parameters:
        #    print(name, p)

    print(f"ptrs_map[backend_idl.{interface_class.__name__}] = {ptr_class_name}", file = out)
    print(f"ptrs_map2['{interface_class.__name__}'] = {ptr_class_name}", file = out)
        
    return out.getvalue()

