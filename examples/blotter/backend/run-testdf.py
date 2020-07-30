import sys, asyncio
from KVAN import fuargs, topdir
topdir.setup_syspath()
import dipole, pandas as pd
Blotter = dipole.import_pyidl("./Blotter.pyidl")
dipole.build_ptrs(Blotter)

class DFTestI(Blotter.DFTest):
    async def get_df(self):
        df = pd.DataFrame({'a': range(10)})
        df['b'] = 'hi'
        print(df)
        sret = Blotter.DataFrame(columns = list(df.columns), dataframeJSON = df.to_json())
        ret = {'retval': sret}
        return ret

async def test_coro():
    comm = dipole.Communicator()
    testdf_ptr = await comm.get_ptr(Blotter.DFTest, "ws://localhost:8080/", "test_df")

    df = await testdf_ptr.get_df()
    print("dataframe:", df)
    df = pd.read_json(df['retval']['dataframeJSON'])
    print("dataframe:", df)
    
@fuargs.action
def test():
    asyncio.get_event_loop().run_until_complete(test_coro())
    asyncio.get_event_loop().run_forever()

    
@fuargs.action
def run():
    port = 8080
    comm = dipole.Communicator()
    comm.set_listen_port(port)

    test_o = DFTestI()
    comm.add_object(test_o, "test_df")

    print("python server start")
    asyncio.get_event_loop().run_until_complete(comm.ws_l)
    asyncio.get_event_loop().run_forever()
    
if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
