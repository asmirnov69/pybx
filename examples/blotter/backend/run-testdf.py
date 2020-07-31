import sys, asyncio
from KVAN import fuargs, topdir
topdir.setup_syspath()
import dipole, pandas as pd
Blotter = dipole.import_pyidl("./Blotter.pyidl")
dipole.build_ptrs(Blotter)

cities = [
    ['Boston', 'MA'],
    ['Worcester', 'MA'],
    ['New York', 'NY'],
    ['Albany', 'NY']
]

class DFTestI(Blotter.DFTest):
    def __init__(self):
        self.df = pd.DataFrame.from_records(cities, columns = ['city', 'state'])
        self.df['temp'] = 25
        self.c = 0

    def update(self):
        print("DFTestI::update", self.c)
        self.c += 1
        self.df['temp'] = self.df.temp - 0.1
        
    async def get_df(self):
        sret = Blotter.DataFrame(columns = list(self.df.columns), dataframeJSON = self.df.to_json(orient = 'records'))
        ret = {'retval': sret}
        self.update()
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
