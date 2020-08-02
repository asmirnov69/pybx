import ipdb
import importlib.machinery, os.path
import websockets, asyncio, threading, uuid
import json, dataclasses, enum, inspect
import pybx_idl

ptrs_map2 = {}

# see also https://stackoverflow.com/q/51575294/1181482
#
def import_pybx(pybx_mod_fn):
    my_module = {}
    pybx_mod_name = os.path.basename(pybx_mod_fn).split(".")[0]
    print(f"loading {pybx_mod_name} from {pybx_mod_fn}")
    exec(open(pybx_mod_fn).read(), my_module)
    code = f"""
class pybx_module__{pybx_mod_name}:
    def __init__(self, adict):
        self.__dict__.update(adict)
{pybx_mod_name} = pybx_module__{pybx_mod_name}(my_module)
    """
    exec(code)    
    return eval(f"{pybx_mod_name}")
    
def build_ptr_code(pybx_mod):
    gen_code = ""
    interface_classes = []
    for mod_el in pybx_mod.__dict__.values():
        if inspect.isclass(mod_el) and issubclass(mod_el, pybx_idl.interface):
            interface_classes.append(mod_el)
            gen_code += pybx_idl.generate_ptr_class_code(mod_el)
    print(gen_code)
    return (interface_classes, gen_code)

def build_ptrs(pybx_mod):
    interface_classes, gen_code = build_ptr_code(pybx_mod)
    exec(gen_code)
    print(ptrs_map2)
    #ipdb.set_trace()
    for interface_class in interface_classes:
        #ipdb.set_trace()
        ptr_class_name = interface_class.__name__ + 'Ptr'
        print(f"defining {ptr_class_name}")
        setattr(pybx_mod, ptr_class_name, ptrs_map2[interface_class.__name__])
        
class ObjectPtrBase: pass
class EnhancedJSONEncoder(json.JSONEncoder):
    def default(self, o):
        #print("EnhancedJSONEncoder::default:", o, type(o))
        if isinstance(o, ObjectPtrBase):
            type_s = f"{o.__class__.__module__}.{o.__class__.__name__}"
            return {'__special_type': type_s, 'object_id': o.object_id}
        if dataclasses.is_dataclass(o):
            return dataclasses.asdict(o)
        if isinstance(o, enum.Enum):
            #print("enum branch", o)
            return o.name
        return super().default(o)

def set_result_f(args):
    fut = args[0]
    result = args[1]
    fut.set_result(result)
    
class WSHandler:
    def __init__(self, comm):
        self.comm = comm
        self.ws = None

    async def client_message_loop(self, ws_url):
        print(f"connecting to {ws_url}")
        self.ws = await websockets.connect(ws_url)
        asyncio.create_task(self.run_message_loop())

    async def run_message_loop(self):
        print("entering message loop")
        while 1:
            try:
                message = await self.ws.recv()
            except Exception as e:
                print("recv() failed with exception", type(e))
                print(e)
                if isinstance(e, websockets.exceptions.ConnectionClosedError) or isinstance(e, websockets.exceptions.ConnectionClosedOK):
                    await self.ws.close()
                    break
                continue

            message_json = json.loads(message)
            if message_json['message-type'] == 'method-call':
                await self.comm.do_call__(message_json, self)
            elif message_json['message-type'] in ['method-call-return', 'method-call-exception']:
                orig_message_id = message_json['orig-message-id']
                result_fut, loop = self.comm.get_call_waiter__(orig_message_id)
                loop.call_soon_threadsafe(set_result_f, [result_fut, message_json])
            else:
                print("run_message_loop: unknown message type:", message_json)
            
class WSHandlerFactory:
    def __init__(self, comm):
        self.comm = comm
        
    async def server_message_loop(self, ws, path):
        print("accept passed", path)
        #ipdb.set_trace()
        ws_handler = WSHandler(self.comm)
        ws_handler.ws = ws
        await ws_handler.run_message_loop()

class Communicator:
    def __init__(self):
        self.objects_lock = threading.Lock()
        self.objects__ = {}
        self.messages_lock = threading.Lock()
        self.messages__ = {}
        self.ws_handler_f = WSHandlerFactory(self)
    
    def add_object(self, o, object_id):
        with self.objects_lock:
            self.objects__[object_id] = o
        return object_id

    def start_server(self, port):
        return websockets.serve(self.ws_handler_f.server_message_loop,
                                'localhost', port)

    async def get_ptr(self, ptr_object_type, ws_url, object_id):
        #ipdb.set_trace()
        ptr_type = ptrs_map2[ptr_object_type.__name__]
        ws_handler = WSHandler(self)
        await ws_handler.client_message_loop(ws_url)
        return ptr_type(ws_handler, object_id)

    def add_call_waiter__(self, message_id, fut, loop):
        with self.messages_lock:
            self.messages__[message_id] = (fut, loop)
    
    def get_call_waiter__(self, orig_message_id):        
        with self.messages_lock:
            return self.messages__.pop(orig_message_id)
    
    async def do_call__(self, message_json, ws_handler):
        try:
            args = message_json['args']
            method_signature = message_json['method-signature']
            method = method_signature.split("__")[1]
            object_id = message_json['object-id']
            print("looking up obj", object_id)
            with self.objects_lock:
                if not object_id in self.objects__:
                    raise Exception("do_message_action: can't find object", object_id)
                obj = self.objects__[object_id]
            b_method = getattr(obj, method)
            if b_method == None:
                raise Exception("do_message_action: can't find method", method, object_id)
            print("calling method", b_method)
            obj.caller_ws_hanlder = ws_handler
            ret = await b_method(**args)
            print("ret:", ret)
            res_message = {'message-type': 'method-call-return',
                           'message-id': str(uuid.uuid1()),
                           'orig-message-id': message_json['message-id'],
                           'retval': {'ret': ret}}
        except Exception as e:
            print("exception caught:", e)
            res_message = {'message-type': 'method-call-exception',
                           'message-id': str(uuid.uuid1()),
                           'orig-message-id': message_json['message-id'],
                           'remote-exception-text': str(e)}

        print("res_message:", json.dumps(res_message, cls = EnhancedJSONEncoder))
        await ws_handler.ws.send(json.dumps(res_message, cls = EnhancedJSONEncoder))