//
// generated code: source - ../backend/Blotter.pybx
import * as libpybx from 'libpybx-js';

class dataclass {};

export class DataFrame extends dataclass {
  constructor(columns, dataframeJSON) {
      super();
      this.columns = columns;
      this.dataframeJSON = dataframeJSON;
  }
};

export class DFWUPC extends dataclass {
    constructor(df, update_c) {
	super();
	this.df = df;
	this.update_c = update_c;
    }
};

export class DFTestPtr extends libpybx.ObjectPtr {
    constructor(comm, ws, object_id) {
	super(comm, ws, object_id);
	this.get_df = this.get_df.bind(this);
	this.subscribe = this.subscribe.bind(this);
    }

    get_df() {
	let p = new Promise((resolve, reject) => {
            let call_req = {
		'message-type': 'method-call',
		'message-id': libpybx.generateQuickGuid(),
		'object-id': this.object_id,
		'method-signature': 'DFTest__get_df',
		'args': {}
	    };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
	    this.ws.send(libpybx.to_json_string(call_req));
	});
	return p.then(ret_json => {
	    let ret = libpybx.from_json(ret_json, new DFWUPC());
	    return ret;
	});
    }

    subscribe(ptr) {
	let p = new Promise((resolve, reject) => {
            let call_req = {
		'message-type': 'method-call',
		'message-id': libpybx.generateQuickGuid(),
		'object-id': this.object_id,
		'method-signature': 'DFTest__subscribe',
		'args': {'ptr': ptr}
	    };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
	    console.log("subscribe:", libpybx.to_json_string(call_req));
	    this.ws.send(libpybx.to_json_string(call_req));
	});
	return p;
    }
};

export class ObserverPtr extends libpybx.ObjectPtr {
    get_type_name() { return 'Blotter.Observer'; }

    constructor(comm, ws, object_id) {
	super(comm, ws, object_id);
	this.show = this.show.bind(this);
    }

    show(df) {
	let p = new Promise((resolve, reject) => {
            let call_req = {
		'message-type': 'method-call',
		'message-id': libpybx.generateQuickGuid(),
		'object-id': this.object_id,
		'method-signature': 'Observer__show',
		'args': {'df': df}
	    };
	    this.comm.add_message_handler(call_req['message-id'], [resolve, reject]);
	    this.ws.send(libpybx.to_json_string(call_req));
	});
	return p;
    }
};


export class DFTest
{
    get_ptr_type() { return DFTestPtr; }
    __call_method(method, args) {
	method = method.split("__")[1];
	if (method == 'get_df') {
	    //return this.get_df(...args);
	    return this.get_df(Array.from(args));
	}
	if (method == 'subscribe') {
	    return this.subscribe(Array.from(args));
	}
	throw new Error("unknown method " + method)
    }
    get_df() {throw new Error("not implemented");}
    subscribe(ptr) {throw new Error("not implemented");}
};

export class Observer
{
    get_ptr_type() { return ObserverPtr; }
    
    __call_method(method, args) {
	method = method.split("__")[1];
	if (method == 'show') {
	    let arg_0 = args.df; // generated call
	    return this.show(arg_0);
	}
	throw new Error("unknown method " + method)
    }
    show(df) {throw new Error("not implemented");}
};
    
