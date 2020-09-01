import ipdb
import sys
from KVAN import fuargs, topdir
topdir.setup_syspath()
import asyncio, pybx, pybx_comm
pybx.import_pybx("backend")

async def a_run_client():
    #ipdb.set_trace()
    comm = pybx_comm.Communicator()
    hello_rop = await comm.get_rop(backend.Hello, "ws://localhost:8080/", "hello")

    g = await hello_rop.sayHello("hi")
    print("sayHello:", g)

@fuargs.action
def run_client():
    #ipdb.set_trace()
    asyncio.get_event_loop().run_until_complete(a_run_client())
    
if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
