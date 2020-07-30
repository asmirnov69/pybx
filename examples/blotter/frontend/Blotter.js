//
// generated code: source - ../../blotter/backend/Blotter.pyidl
import * as libdipole from 'libdipole-js';
    
export class DataFrame {
  constructor(columns, dataframeJSON) {
    this.columns = columns;
    this.dataframeJSON = dataframeJSON;
  }
};
export class DFTestPtr {
  constructor(o_ptr) {
    this.o_ptr = o_ptr;
  this.get_df = this.get_df.bind(this);
  }
   to_json() { return {'object_id': this.o_ptr.object_id}; }
   get_df() {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libdipole.generateQuickGuid(),
               'object-id': this.o_ptr.object_id,
              'method-signature': 'DFTest__get_df',
          'args': {
          }
       };
	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      this.o_ptr.ws.send(JSON.stringify(call_req));
   });
 return p.then(ret => {
 let tret = new DataFrame(); Object.assign(tret, ret);
  return tret; });
  }
};
export class DFTest
{
   __call_method(method, args) {
      method = method.split("__")[1];
   if (method == 'get_df') {
   //return this.get_df(...args);
   return this.get_df(Array.from(args));
}
  throw new Error("unknown method " + method)
}
get_df() {throw new Error("not implemented");}
};
