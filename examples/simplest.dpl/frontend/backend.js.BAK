import * as libdipole from 'libdipole-js';

export class Greetings {
    constructor(language, text, color) {
	this.language = language;
	this.text = text;
	this.color = color;
    }
};

export class HelloPtr
{
    constructor(o_ptr) {
	this.o_ptr = o_ptr;
	this.sayHello = this.sayHello.bind(this);
    }

    sayHello(weSay) {
	let p = new Promise((resolve, reject) => {
	    let call_req = {
		'message-type': 'method-call',
		'message-id': libdipole.generateQuickGuid(),
		'object-id': this.o_ptr.object_id,
		'method-signature': 'Hello__sayHello',
		'args': {
		    weSay: weSay
		}
	    };

	    this.o_ptr.comm.add_message_handler(call_req['message-id'],
						[resolve, reject]);
	    this.o_ptr.ws.send(JSON.stringify(call_req));
	});

	return p.then(ret => {
	    return new Greetings(ret.language, ret.text, ret.color);
	});
    }

    reformatGreetings(gs) {
	let p = new Promise((resolve, reject) => {
	    let call_req = {
		'message-type': 'method-call',
		'message-id': libdipole.generateQuickGuid(),
		'object-id': this.o_ptr.object_id,
		'method-signature': 'Hello__reformatGreetings',
		'args': {
		    gs: gs
		}
	    };

	    this.o_ptr.comm.add_message_handler(call_req['message-id'],
						[resolve, reject]);
	    this.o_ptr.ws.send(JSON.stringify(call_req));
	});

	return p.then(ret => {
	    let tret = [];
	    for (var i = 0; i < ret.length; i++) {
		tret.push(new Greetings(ret[i].language, ret[i].text, ret[i].color));
	    }
	    return tret;
	});
    }

    register_hello_cb(cb) {
	let p = new Promise((resolve, reject) => {
	    let call_req = {
		'message-type': 'method-call',
		'message-id': libdipole.generateQuickGuid(),
		'object-id': this.o_ptr.object_id,
		'method-signature': 'Hello__register_hello_cb',
		'args': {
		    cb: cb.to_json()
		}
	    };
	    this.o_ptr.comm.add_message_handler(call_req['message-id'],
						[resolve, reject]);
	    this.o_ptr.ws.send(JSON.stringify(call_req));
	});

	return p.then(ret => {
	    return ret;
	});	
    }
};

export class HelloCBPtr
{
    constructor(o_ptr) {
	this.o_ptr = o_ptr;
    }

    to_json() {
	return {'object_id': this.o_ptr.object_id};
    }
    
    confirmHello(hello) {
	throw Error("confirmHello: not implemented");
    }
};

export class HelloCB {
    __call_method(method, args) {
	method = method.split("__")[1];
	if (method == 'confirmHello') {
	    //return this.confirmHello(...args);
	    return this.confirmHello(Array.from(args));
	}
	throw new Error("unknown method " + method);
    }

    confirmHello(hello) {
	throw new Error("not implemented");
    }
};
