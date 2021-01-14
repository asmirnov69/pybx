import ipdb
import sys, asyncio
from KVAN import fuargs, topdir
topdir.setup_syspath()
import pybx, pandas as pd, threading
import pybx_comm
import random
pybx.import_pybx("Blotter")
pybx.import_pybx("Utils")

class ObserverI(Blotter.Observer):
    async def show(self, df):
        print("ObserverI::show:", df)

cities = [
    ['Boston', 'MA'],
    ['Worcester', 'MA'],
    ['New York', 'NY'],
    ['Albany', 'NY']
]

class DFTestI(Blotter.DFTest):
    def __init__(self):
        self.subscribers = []
        self.df_lock = threading.Lock()
        self.df = pd.DataFrame.from_records(cities, columns = ['city', 'state'])
        self.df['temp'] = 25
        self.c = 0

    # see about data race example and GIL role -> https://stackoverflow.com/a/33435680/1181482
    async def update_task(self):
        print("staring update_task")
        while 1:
            with self.df_lock:
                print("DFTestI::update", self.c)
                for ii, rr in self.df.iterrows():
                    incr = (random.random() - 0.5) * 0.45
                    if abs(incr) > 0.1:
                        self.c += 1
                        self.df.loc[ii, 'temp'] = rr['temp'] + incr
            await self.publish()
            await asyncio.sleep(2)
        
    async def get_df(self):
        with self.df_lock:
            df_ret = Utils.DataFrame(columns = list(self.df.columns), dataframeJSON = self.df.to_json(orient = 'records'))
            return Blotter.DFWUPC(df = df_ret, update_c = self.c)

    async def subscribe(self, rop):
        print("DFTestI::subscribe:", rop)
        #ipdb.set_trace()
        with self.df_lock:
            res_rop = self.subscribers.append(rop)
            print("res_rop: ", res_rop)

    async def publish(self):
        with self.df_lock:
            for rop in self.subscribers:
                try:
                    #ipdb.set_trace()
                    df_o = Utils.DataFrame(columns = list(self.df.columns), dataframeJSON = self.df.to_json(orient = 'records'))
                    j = Blotter.DFWUPC(df = df_o, update_c = self.c)
                    await rop.show(j)
                except Exception as e:
                    print("rop.show failed", rop)
                    print(e)
            
async def test_coro():
    comm = pybx_comm.Communicator()
    testdf_rop = await comm.get_rop(Blotter.DFTest, "ws://localhost:8080/", "test_df")

    df = await testdf_rop.get_df()
    print("dataframe:", df)

@fuargs.action
def test():
    asyncio.get_event_loop().run_until_complete(test_coro())
    
async def test_subscriber_coro():
    comm = pybx_comm.Communicator()
    testdf_rop = await comm.get_rop(Blotter.DFTest, "ws://localhost:8080/", "test_df")

    subscriber_o = ObserverI()
    subscriber_rop = comm.add_object(subscriber_o, "uu")
    res_rop = await testdf_rop.subscribe(subscriber_rop)
    print("RES_ROP:", res_rop)

@fuargs.action
def test_subscriber():
    asyncio.get_event_loop().run_until_complete(test_subscriber_coro())
    asyncio.get_event_loop().run_forever()
    
async def run_coro():
    port = 8080
    host = "0.0.0.0"
    comm = pybx_comm.Communicator()
    comm.set_listen_port(port, host)
    test_o = DFTestI()
    comm.add_object(test_o, "test_df")

    update_task = test_o.update_task()
    print("update_task:", update_task, type(update_task))
    ws_server_task = comm.start_server()
    print("ws_server_task:", ws_server_task, type(ws_server_task))        
    if 1:
        asyncio.create_task(update_task)
        await ws_server_task
    else:
        await asyncio.gather(update_task, ws_server_task)
        #asyncio.gather(update_task, ws_server_task, return_exceptions=True)

    print("python server start")    

@fuargs.action
def run():
    asyncio.get_event_loop().run_until_complete(run_coro())
    asyncio.get_event_loop().run_forever()
    
if __name__ == "__main__":
    fuargs.exec_actions(sys.argv[1:])
