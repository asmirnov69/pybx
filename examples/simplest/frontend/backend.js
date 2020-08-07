//
// generated code: source - ../backend/backend.pybx
import * as libpybx from 'libpybx-js';
    
export class Greetings extends libpybx.dataclass {
  constructor(language, text, color) {
  super();
    this.language = language;
    this.text = text;
    this.color = color;
  }
};
export class HelloPtr extends libpybx.ObjectPtr {
  get_type_name() { return 'backend.Hello'; }
  constructor(comm, ws, object_id) {
     super(comm, ws, object_id);
  this.reformatGreetings = this.reformatGreetings.bind(this);
  this.register_hello_cb = this.register_hello_cb.bind(this);
  this.sayHello = this.sayHello.bind(this);
  }
   to_json() { return {'object_id': this.object_id}; }
   reformatGreetings(gs) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.object_id,
              'method-signature': 'Hello__reformatGreetings',
          'args': {
                 gs: gs,
          }
       };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      console.log("reformatGreetings:", libpybx.to_json_string(call_req));
      this.ws.send(libpybx.to_json_string(call_req));
   });
 return p.then(ret_json => {
 let ret = libpybx.from_json(ret_json, []);
 return ret; });
 }
   register_hello_cb(cb) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.object_id,
              'method-signature': 'Hello__register_hello_cb',
          'args': {
                 cb: cb,
          }
       };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      console.log("register_hello_cb:", libpybx.to_json_string(call_req));
      this.ws.send(libpybx.to_json_string(call_req));
   });
 return p.then(ret_json => {
 let ret = libpybx.from_json(ret_json, null);
 return ret; });
 }
   sayHello(weSay) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.object_id,
              'method-signature': 'Hello__sayHello',
          'args': {
                 weSay: weSay,
          }
       };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      console.log("sayHello:", libpybx.to_json_string(call_req));
      this.ws.send(libpybx.to_json_string(call_req));
   });
 return p.then(ret_json => {
 let ret = libpybx.from_json(ret_json, new Greetings());
 return ret; });
 }
};
export class HelloCBPtr extends libpybx.ObjectPtr {
  get_type_name() { return 'backend.HelloCB'; }
  constructor(comm, ws, object_id) {
     super(comm, ws, object_id);
  this.confirmHello = this.confirmHello.bind(this);
  }
   to_json() { return {'object_id': this.object_id}; }
   confirmHello(hello) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.object_id,
              'method-signature': 'HelloCB__confirmHello',
          'args': {
                 hello: hello,
          }
       };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      console.log("confirmHello:", libpybx.to_json_string(call_req));
      this.ws.send(libpybx.to_json_string(call_req));
   });
 return p.then(ret_json => {
 let ret = libpybx.from_json(ret_json, null);
 return ret; });
 }
};
export class Hello
{
  get_ptr_type() { return HelloPtr; }
   __call_method(method, args) {
      method = method.split("__")[1];
   if (method == 'reformatGreetings') {
    let arg_0 = libpybx.from_json(args.gs, []);
    return this.reformatGreetings(arg_0);
  }
   if (method == 'register_hello_cb') {
    let arg_0 = libpybx.from_json(args.cb, new HelloCBPtr());
    return this.register_hello_cb(arg_0);
  }
   if (method == 'sayHello') {
    let arg_0 = libpybx.from_json(args.weSay, null);
    return this.sayHello(arg_0);
  }
  throw new Error("unknown method " + method)
}
reformatGreetings(gs) {throw new Error("not implemented");}
register_hello_cb(cb) {throw new Error("not implemented");}
sayHello(weSay) {throw new Error("not implemented");}
};
export class HelloCB
{
  get_ptr_type() { return HelloCBPtr; }
   __call_method(method, args) {
      method = method.split("__")[1];
   if (method == 'confirmHello') {
    let arg_0 = libpybx.from_json(args.hello, null);
    return this.confirmHello(arg_0);
  }
  throw new Error("unknown method " + method)
}
confirmHello(hello) {throw new Error("not implemented");}
};
