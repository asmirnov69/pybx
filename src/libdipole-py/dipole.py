import websockets, uuid
from websockets.extensions import permessage_deflate
import json, dataclasses, enum

class EnhancedJSONEncoder(json.JSONEncoder):
    def default(self, o):
        #print("EnhancedJSONEncoder::default:", o, type(o))
        if dataclasses.is_dataclass(o):
            return dataclasses.asdict(o)
        if isinstance(o, enum.Enum):
            #print("enum branch", o)
            return o.name
        return super().default(o)

class WSHandler:
    def __init__(self, comm):
        self.comm = comm
        self.ws = None

    async def run_message_loop(self):
        print("entering message loop")
        while 1:
            try:
                message = await self.ws.recv()
            except Exception as e:
                print("recv() failed with exception", type(e))
                print(e)
                if isinstance(e, websockets.exceptions.ConnectionClosedError):
                    await self.ws.close()
                    break
                continue
            print("WSHandler::run_message_loop: ", message)        
            message_json = json.loads(message)
            print(message_json)
            if message_json['message-type'] == 'method-call':
                await self.comm.do_call__(message_json, self.ws)
            
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
        self.objects = {}
        self.ws_handler_f = WSHandlerFactory(self)
    
    def set_listen_port(self, port):
        self.ws_l = websockets.serve(self.ws_handler_f.server_message_loop,
                                     'localhost', port,
                                     extensions=[
                                         permessage_deflate.ServerPerMessageDeflateFactory(
                                             server_max_window_bits=15,
                                             client_max_window_bits=15
                                         ),
                                     ],)

    def add_object(self, o, object_id):
        self.objects[object_id] = o

    async def do_call__(self, message_json, ws):
        try:
            args = message_json['args']
            method_signature = message_json['method-signature']
            method = method_signature.split("__")[1]
            object_id = message_json['object-id']
            print("looking up obj", object_id)
            if not object_id in self.objects:
                raise Exception("do_message_action: can't find object", object_id)
            obj = self.objects[object_id]
            b_method = getattr(obj, method)
            if b_method == None:
                raise Exception("do_message_action: can't find method", method, object_id)
            print("calling method", b_method)
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
        await ws.send(json.dumps(res_message, cls = EnhancedJSONEncoder))
