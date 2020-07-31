import sys, asyncio
from KVAN import fuargs, topdir
topdir.setup_syspath()
import dipole, pandas as pd, threading, time
import random
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
        self.df_lock = threading.Lock()
        self.df = pd.DataFrame.from_records(cities, columns = ['city', 'state'])
        self.df['temp'] = 25
        self.c = 0

    # see about data race example and GIL role -> https://stackoverflow.com/a/33435680/1181482
    def update_thread(self):
        while 1:
            with self.df_lock:
                print("DFTestI::update", self.c)
                for ii, rr in self.df.iterrows():
                    incr = (random.random() - 0.5) * 0.45
                    if abs(incr) > 0.1:
                        self.c += 1
                        self.df.loc[ii, 'temp'] = rr['temp'] + incr
            time.sleep(2)
        
    async def get_df(self):
        with self.df_lock:
            df_ret = Blotter.DataFrame(columns = list(self.df.columns), dataframeJSON = self.df.to_json(orient = 'records'))
            sret = Blotter.DFWUPC(df = df_ret, update_c = self.c)
            ret = {'retval': sret}
            return ret

async def test_coro():
    comm = dipole.Communicator()
    testdf_ptr = await comm.get_ptr(Blotter.DFTest, "ws://localhost:8080/", "test_df")

    df = await testdf_ptr.get_df()
    print("dataframe:", df)
    df = pd.read_json(df['retval']['df']['dataframeJSON'])
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
    t = threading.Thread(target = test_o.update_thread)
    t.daemon = True
    t.start()

    print("python server start")
    asyncio.get_event_loop().run_until_complete(comm.ws_l)
    asyncio.get_event_loop().run_forever()
    
if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
