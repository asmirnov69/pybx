import ipdb
import websockets, asyncio, threading, uuid
import json
import pybx_type_descriptors as pybx_td
import pybx_json

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
                await self.comm.do_call__(message_json, self, False)
            elif message_json['message-type'] == 'method-oneway-call':
                await self.comm.do_call__(message_json, self, True)
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
        self.listen_host = None
        self.listen_port = -1

    def set_listen_port(self, listen_port, listen_host = "127.0.0.0"):
        self.listen_port = listen_port
        self.listen_host = listen_host
        
    def add_object(self, o, object_id):
        with self.objects_lock:
            self.objects__[object_id] = o
            interface_type = type(o).__bases__[0]
            interface_rop_type = pybx_td.get_interface_rop_type(interface_type)
            return interface_rop_type(self, None, object_id)

    def start_server(self):
        return websockets.serve(self.ws_handler_f.server_message_loop,
                                self.listen_host, self.listen_port)

    async def get_rop(self, interface_type, ws_url, object_id):
        #ipdb.set_trace()
        interface_rop_type = pybx_td.get_interface_rop_type(interface_type)
        ws_handler = WSHandler(self)
        await ws_handler.client_message_loop(ws_url)
        return interface_rop_type(self, ws_handler.ws, object_id)

    def add_call_waiter__(self, message_id, fut, loop):
        with self.messages_lock:
            self.messages__[message_id] = (fut, loop)
    
    def get_call_waiter__(self, orig_message_id):        
        with self.messages_lock:
            return self.messages__.pop(orig_message_id)
    
    async def do_call__(self, message_json, ws_handler, oneway_f):
        try:
            object_id = message_json['object-id']
            print("looking up obj", object_id)
            with self.objects_lock:
                if not object_id in self.objects__:
                    raise Exception("do_call__: can't find object", object_id)
                obj = self.objects__[object_id]
            #ipdb.set_trace()
            method_signature = message_json['method-signature']
            m_def = pybx_td.get_method_def(method_signature)
            if m_def == None:
                raise Exception("do_message_action: can't find method", method_signature, object_id)
            print("calling method", m_def.name)
            #ipdb.set_trace()
            args_json = message_json['args']
            args = {}
            args_d = dict(zip(m_def.get_method_args(), m_def.get_method_arg_types()))
            for k, v in args_json.items():
                arg_ann_type = args_d[k]
                arg_v = pybx_json.from_json(v, arg_ann_type)
                if isinstance(arg_v, pybx_td.rop_impl_base): # activation
                    arg_v.comm = self
                    arg_v.ws = ws_handler.ws
                args[k] = arg_v
            b_method = eval(f"obj.{m_def.name}")
            ret = await b_method(**args)
            if oneway_f == True:
                return
            print("ret:", ret)
            #ipdb.set_trace()
            ret_json = pybx_json.to_json(ret)
            res_message = {'message-type': 'method-call-return',
                           'message-id': str(uuid.uuid1()),
                           'orig-message-id': message_json['message-id'],
                           'retval': {'retval': ret_json}}
            print('return res_message:', res_message)
            res_message_s = json.dumps(res_message)
        except Exception as e:
            print("exception caught:", e)
            res_message = {'message-type': 'method-call-exception',
                           'message-id': str(uuid.uuid1()),
                           'orig-message-id': message_json['message-id'],
                           'remote-exception-text': str(e)}
            print('exception res_message:', res_message)
            res_message_s = json.dumps(res_message)

        await ws_handler.ws.send(res_message_s)
