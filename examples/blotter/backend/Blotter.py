import ipdb
import pybx, uuid, json, asyncio
import pybx_type_descriptors as pybx_td
import pybx_json

exec('\n'.join(open("./Blotter.pybx").readlines()))

class ptr_impl_DFTest(pybx.ptr_impl_base):
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
        print('res_message_json:', res_message_json['retval']['ret'])
        ret = pybx_json.from_json(res_message_json['retval']['ret'], DFWUPC)
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
        print('res_message_json:', res_message_json['retval']['ret'])
        ret = pybx_json.from_json(res_message_json['retval']['ret'], None)
        return ret

class ptr_impl_Observer(pybx.ptr_impl_base):
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
        print('res_message_json:', res_message_json['retval']['ret'])
        ret = pybx_json.from_json(res_message_json['retval']['ret'], None)
        return ret

pybx_td.register_interface_ptr_type(DFTest, ptr_impl_DFTest)
pybx_td.register_interface_ptr_type(Observer, ptr_impl_Observer)

pybx_td.register_method_descriptor('DFTest__get_df', 'get_df', {}, DFWUPC)
pybx_td.register_method_descriptor('DFTest__subscribe', 'subscribe', {'ptr': ObserverPtr}, None)
pybx_td.register_method_descriptor('Observer__show', 'show', {'df': DFWUPC}, None)

