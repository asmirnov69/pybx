//
// generated code: source - ../backend/backend.pybx
import * as libpybx from 'libpybx-js';
    
export class Greetings {
  constructor(language, text, color) {
    this.language = language;
    this.text = text;
    this.color = color;
  }
};
export class HelloPtr {
  constructor(o_ptr) {
    this.o_ptr = o_ptr;
  this.sayHello = this.sayHello.bind(this);
  this.reformatGreetings = this.reformatGreetings.bind(this);
  this.register_hello_cb = this.register_hello_cb.bind(this);
  }
   to_json() { return {'object_id': this.o_ptr.object_id}; }
   sayHello(weSay) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.o_ptr.object_id,
              'method-signature': 'Hello__sayHello',
          'args': {
                 weSay: weSay,
          }
       };
	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      this.o_ptr.ws.send(JSON.stringify(call_req));
   });
 return p.then(ret => {
 let tret = new Greetings(); Object.assign(tret, ret);
  return tret; });
  }
   reformatGreetings(gs) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.o_ptr.object_id,
              'method-signature': 'Hello__reformatGreetings',
          'args': {
                 gs: gs,
          }
       };
	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      this.o_ptr.ws.send(JSON.stringify(call_req));
   });
 return p.then(ret => {
   let tret = [];
   for (let i = 0; i < ret.lenght; i++) {
    let o = new Greetings(); Object.assign(o, ret[i]);
     tret.push(o);
   }
  return tret; });
  }
   register_hello_cb(cb) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.o_ptr.object_id,
              'method-signature': 'Hello__register_hello_cb',
          'args': {
                 cb: cb.to_json(),
          }
       };
	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      this.o_ptr.ws.send(JSON.stringify(call_req));
   });
 return p.then(ret => {
 let tret = ret;
  return tret; });
  }
};
export class HelloCBPtr {
  constructor(o_ptr) {
    this.o_ptr = o_ptr;
  this.confirmHello = this.confirmHello.bind(this);
  }
   to_json() { return {'object_id': this.o_ptr.object_id}; }
   confirmHello(hello) {
    let p = new Promise((resolve, reject) => {
         let call_req = {
               'message-type': 'method-call',
               'message-id': libpybx.generateQuickGuid(),
               'object-id': this.o_ptr.object_id,
              'method-signature': 'HelloCB__confirmHello',
          'args': {
                 hello: hello,
          }
       };
	    this.o_ptr.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
      this.o_ptr.ws.send(JSON.stringify(call_req));
   });
 return p.then(ret => {
 let tret = ret;
  return tret; });
  }
};
export class Hello
{
   __call_method(method, args) {
      method = method.split("__")[1];
   if (method == 'sayHello') {
   //return this.sayHello(...args);
   return this.sayHello(Array.from(args));
}
   if (method == 'reformatGreetings') {
   //return this.reformatGreetings(...args);
   return this.reformatGreetings(Array.from(args));
}
   if (method == 'register_hello_cb') {
   //return this.register_hello_cb(...args);
   return this.register_hello_cb(Array.from(args));
}
  throw new Error("unknown method " + method)
}
sayHello(weSay) {throw new Error("not implemented");}
reformatGreetings(gs) {throw new Error("not implemented");}
register_hello_cb(cb) {throw new Error("not implemented");}
};
export class HelloCB
{
   __call_method(method, args) {
      method = method.split("__")[1];
   if (method == 'confirmHello') {
   //return this.confirmHello(...args);
   return this.confirmHello(Array.from(args));
}
  throw new Error("unknown method " + method)
}
confirmHello(hello) {throw new Error("not implemented");}
};
