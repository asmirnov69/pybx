import sys
from KVAN import fuargs, topdir
topdir.setup_syspath()
import asyncio, pybx, pybx_comm
pybx.import_pybx("backend")

class HelloI(backend.Hello):
    def __init__(self):
        self.cbs = []
    
    async def sayHello(self, weSay):
        if weSay != "hi":
            raise Exception(f"unexpected weSay value: {weSay}")
        print("HelloI::sayHello")
        ret = backend.Greetings(language = "python", text = "Hello from python", color = backend.Color.NORMAL)
        ret.color = backend.Color.RED
        return ret

    async def register_hello_cb(self, cb):
        print("HelloI::register_hello_cb:", cb)
        self.cbs.append(cb)
        return "jkj"

    async def reformatGreetings(self, gs):
        print("HelloI::reformatGreetings:", gs)
        for g in gs:
            g['color'] = backend.Color.GREEN
        return gs
    
@fuargs.action
def run_server():
    port = 8080
    comm = pybx_comm.Communicator()
    ws_server_task = comm.start_server(port)

    hello_o = HelloI()
    comm.add_object(hello_o, "hello")

    print("python server start")
    asyncio.get_event_loop().run_until_complete(ws_server_task)
    asyncio.get_event_loop().run_forever()

if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
