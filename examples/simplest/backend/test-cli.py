import ipdb
import sys
from KVAN import fuargs, topdir
topdir.setup_syspath()
import asyncio, pybx, uuid, json
backend_idl = pybx.import_pybx("./backend.pybx")
pybx.build_ptrs(backend_idl)
#gen_code = pybx.build(backend_idl)
##print(gen_code)
#exec(gen_code)

"""
# this code should be generated using defintion of backend_idl.Hello
class HelloPtr:
    def __init__(self, ws_handler, object_id):
        self.ws_handler = ws_handler
        self.object_id = object_id

    async def sayHello(self, weSay):
        message_json = {
            'message-type': 'method-call',
            'method-signature': 'Hello__sayHello',
            'message-id': str(uuid.uuid1()),
            'object-id': self.object_id,
            'args': {
                'weSay': weSay
                }
            }
        await self.ws_handler.ws.send(json.dumps(message_json))
        message_id = message_json['message-id']
        result_fut = asyncio.Future()
        loop = asyncio.get_event_loop()
        self.ws_handler.comm.messages[message_id] = (result_fut, loop)
        res_message_json = await result_fut
        print("res_message_json:", res_message_json)
        return res_message_json
        
pybx.ptrs_map[backend_idl.Hello] = HelloPtr
#end of generated code
"""

async def a_run_client():
    #ipdb.set_trace()
    comm = pybx.Communicator()
    hello_ptr = await comm.get_ptr(backend_idl.Hello, "ws://localhost:8080/", "hello")

    g = await hello_ptr.sayHello("hi")
    print("sayHello:", g)

@fuargs.action
def run_client():
    #ipdb.set_trace()
    asyncio.get_event_loop().run_until_complete(a_run_client())
    asyncio.get_event_loop().run_forever()
    
if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
