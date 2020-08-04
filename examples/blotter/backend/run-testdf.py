import ipdb
import sys, asyncio
from KVAN import fuargs, topdir
topdir.setup_syspath()
import pybx, pybx_type_descriptors as pybx_td, pandas as pd, json, threading
import random, uuid
import pybx_json
Blotter = pybx.import_pybx("./Blotter.pybx")

pybx_td.register_method_descriptor('DFTest__get_df', 'get_df', {}, Blotter.DFWUPC)
pybx_td.register_method_descriptor('DFTest__subscribe', 'subscribe', {'ptr': Blotter.ObserverPtr}, None)
pybx_td.register_method_descriptor('Observer__show', 'show', {'df': Blotter.DFWUPC}, None)

DFTestPtr_code = """
class ptr_impl_DFTest(pybx_td.ptr_impl_base):
    def __init__(self, comm, ws, object_id):
        self.comm = comm
        self.ws = ws
        self.object_id = object_id

    async def get_df(self):
        message_json = {
            'message-type': 'method-call',
            'method-signature': 'DFTest__get_df',
            'message-id': str(uuid.uuid1()),
            'object-id': self.object_id,
            'args': {}
        }

        print("send:", message_json)
        await self.ws.send(json.dumps(message_json))
        message_id = message_json['message-id']
        result_fut = asyncio.Future()
        loop = asyncio.get_event_loop()
        self.comm.add_call_waiter__(message_id, result_fut, loop)

        res_message_json = await result_fut
        print('res_message_json:', res_message_json['retval']['retval'])
        ret = pybx_json.from_json(res_message_json['retval']['retval'], Blotter.DFWUPC)
        return ret

    async def subscribe(self, ptr):
        message_json = {
            'message-type': 'method-call',
            'method-signature': 'DFTest__subscribe',
            'message-id': str(uuid.uuid1()),
            'object-id': self.object_id,
            'args': {'ptr': pybx_json.to_json(ptr)}
        }

        #ipdb.set_trace()
        print("send:", message_json)
        await self.ws.send(json.dumps(message_json))
        message_id = message_json['message-id']
        result_fut = asyncio.Future()
        loop = asyncio.get_event_loop()
        self.comm.add_call_waiter__(message_id, result_fut, loop)

        res_message_json = await result_fut
        print('res_message_json:', res_message_json['retval']['retval'])
        ret = pybx_json.from_json(res_message_json['retval']['retval'], None)
        return ret

"""

ObserverPtr_code = """
class ptr_impl_Observer(pybx_td.ptr_impl_base):
    def __init__(self, comm, ws, object_id):
        self.comm = comm
        self.ws = ws
        self.object_id = object_id

    async def show(self, df):
        message_json = {
            'message-type': 'method-call',
            'method-signature': 'Observer__show',
            'message-id': str(uuid.uuid1()),
            'object-id': self.object_id,
            'args': {'df': pybx_json.to_json(df)}
        }

        print("send:", message_json)
        #ipdb.set_trace()
        await self.ws.send(json.dumps(message_json))
        message_id = message_json['message-id']
        result_fut = asyncio.Future()
        loop = asyncio.get_event_loop()
        self.comm.add_call_waiter__(message_id, result_fut, loop)
        
        res_message_json = await result_fut
        print('res_message_json:', res_message_json)
        print('res_message_json:', res_message_json['retval']['retval'])
        ret = pybx_json.from_json(res_message_json['retval']['retval'], None)
        return ret
"""

new_defs = {}
exec(DFTestPtr_code, globals(), new_defs)
pybx_td.register_interface_ptr_type(Blotter.DFTest, new_defs['ptr_impl_DFTest'])

new_defs = {}
exec(ObserverPtr_code, globals(), new_defs)
pybx_td.register_interface_ptr_type(Blotter.Observer, new_defs['ptr_impl_Observer'])

ipdb.set_trace()

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
            df_ret = Blotter.DataFrame(columns = list(self.df.columns), dataframeJSON = self.df.to_json(orient = 'records'))
            return Blotter.DFWUPC(df = df_ret, update_c = self.c)

    async def subscribe(self, ptr):
        print("DFTestI::subscribe:", ptr)
        #ipdb.set_trace()
        with self.df_lock:
            self.subscribers.append(ptr)

    async def publish(self):
        with self.df_lock:
            for ptr in self.subscribers:
                try:
                    #ipdb.set_trace()
                    df_o = Blotter.DataFrame(columns = list(self.df.columns), dataframeJSON = self.df.to_json(orient = 'records'))
                    j = Blotter.DFWUPC(df = df_o, update_c = self.c)
                    await ptr.show(j)
                except Exception as e:
                    print("ptr.show failed", ptr)
                    print(e)
            
async def test_coro():
    comm = pybx.Communicator()
    testdf_ptr = await comm.get_ptr(Blotter.DFTest, "ws://localhost:8080/", "test_df")

    df = await testdf_ptr.get_df()
    print("dataframe:", df)

@fuargs.action
def test():
    asyncio.get_event_loop().run_until_complete(test_coro())
    
async def test_subscriber_coro():
    comm = pybx.Communicator()
    testdf_ptr = await comm.get_ptr(Blotter.DFTest, "ws://localhost:8080/", "test_df")

    subscriber_o = ObserverI()
    subscriber_ptr = comm.add_object(subscriber_o, "uu")
    await testdf_ptr.subscribe(subscriber_ptr)    
    
@fuargs.action
def test_subscriber():
    asyncio.get_event_loop().run_until_complete(test_subscriber_coro())
    asyncio.get_event_loop().run_forever()
    
async def run_coro():
    port = 8080
    comm = pybx.Communicator()
    test_o = DFTestI()
    comm.add_object(test_o, "test_df")

    update_task = test_o.update_task()
    print("update_task:", update_task, type(update_task))
    ws_server_task = comm.start_server(port)
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
