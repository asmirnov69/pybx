import React from 'react';

function generateQuickGuid() {
    return Math.random().toString(36).substring(2, 15) +
        Math.random().toString(36).substring(2, 15);
}

class Greetings {
    constructor(language, text, color) {
	this.language = language;
	this.text = text;
	this.color = color;
    }
};

class ObjectPtr
{
    constructor(comm, ws, object_id) {
	this.comm = comm;
	this.ws = ws;
	this.object_id = object_id;
    }
}

class Communicator
{
    constructor() {
	this.ws = null;
	this.messages = new Map();
	this.objects = new Map();
    }

    add_object(o, object_id) {
	if (!object_id) {
	    object_id = generateQuickGuid();
	}
	this.objects.set(object_id, o);
	return new ObjectPtr(this, this.ws, object_id);
    }
    
    add_message_handler(message_id, cbs) {
	this.messages.set(message_id, cbs)
    }
    
    connect(ws_url, object_id) {	
	return new Promise((resolve, reject) => {
	    this.ws = new WebSocket(ws_url);
	    this.ws.onopen = (e) => {
		console.log("WS: connection established");
		let o_ptr = new ObjectPtr(this, this.ws, object_id);
		resolve(o_ptr);
	    };

	    this.ws.onerror = function(error) {
		console.log(`WS error: ${error.message}`);
		reject(error);
	    };

	    this.ws.onclose = function(event) {
		if (event.wasClean) {
		    console.log(`[close] Connection closed cleanly, code=${event.code} reason=${event.reason}`);
		} else {
		    // e.g. server process killed or network down
		    // event.code is usually 1006 in this case
		    console.log('[close] Connection died');
		}
	    };

	    this.ws.onmessage = (e) => {
		e.data.text().then(e_data => {
		    console.log(`WS message: ${e_data}`);
		    let message = JSON.parse(e_data);
		    if (message['message-type'] == 'method-call-return') {
			let [resolve, reject] = this.messages.get(message['orig-message-id']);
			let ret = message.retval.ret;
			resolve(ret);
		    } else if (message['message-type'] == 'method-call-exception') {
			let [resolve, reject] = this.messages.get(message['orig-message-id']);
			reject(message['remote-exception-text']);
		    } else if (message['message-type'] == 'method-call') {
			let o = this.objects.get((message['object-id']));
			let method = message['method-signature'];
			let args = message['args'];
			let res = o.__call_method(method, args);
			let res_message = {
			    'message-type': 'method-call-return',
			    'message-id': generateQuickGuid(),
			    'orig-message-id': message['message-id'],
			    'retval': {
				'ret': res
			    }
			};
			this.ws.send(JSON.stringify(res_message));
		    }
		});
	    };
			     
	});
    }
};

class HelloPtr
{
    constructor(o_ptr) {
	this.o_ptr = o_ptr;
	this.sayHello = this.sayHello.bind(this);
    }

    sayHello(weSay) {
	let p = new Promise((resolve, reject) => {
	    let call_req = {
		'message-type': 'method-call',
		'message-id': generateQuickGuid(),
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
		'message-id': generateQuickGuid(),
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
		'message-id': generateQuickGuid(),
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

class HelloCBPtr
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

class HelloCB {
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

class HelloCBI extends HelloCB {
    confirmHello(hello) {
	console.log("HelloCBI::confirmHello")
	return "confirmed: " + hello;
    }
};

class App extends React.Component {
    constructor(props) {
	super(props);
	this.comm = new Communicator();
	this.state = {greeting: 'none', greeting2: 'none'};
	this.onClick = this.onClick.bind(this);
    }

    componentDidMount() {
	let ws_url = "ws://localhost:8080/";
	let object_id = "hello";
	this.comm.connect(ws_url, object_id).then(o_ptr => {
	    this.hello_ptr = new HelloPtr(o_ptr);
	}).then(() => {
	    let hellocb_o = new HelloCBI();
	    let hellocb_o_ptr = this.comm.add_object(hellocb_o);
	    let hellocb_ptr = new HelloCBPtr(hellocb_o_ptr);
	    return this.hello_ptr.register_hello_cb(hellocb_ptr);
	}).then(res => {
	    console.log("register_hello_cb: ", res);	    
	    return this.hello_ptr.sayHello("hi");
	}).then(g => {
	    console.log("server response:", g);
	    this.setState({...this.state, greeting: g.text});
	});
    }
    
    onClick() {
	this.pb.disabled = true;
	let gs = [];
	gs.push(new Greetings("russian", "privet", "GREEN"));
	gs.push(new Greetings("german", "halo", "GREEN"));
	this.hello_ptr.reformatGreetings(gs).then(ret => {
	    this.setState({...this.state, greeting2: gs.length},
			  () => {this.pb.disabled = false; });
	});
    }
    
    render() {
	return (<div>
		<h1>Hello from modules</h1>
		<h2>{this.state.greeting}</h2>
		<h2>{this.state.greeting2}</h2>
		<button ref={r => this.pb = r} onClick={this.onClick}>PRESS</button>
	       </div>);
    }
};

export default App;
