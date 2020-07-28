export function generateQuickGuid() {
    return Math.random().toString(36).substring(2, 15) +
        Math.random().toString(36).substring(2, 15);
}

export class ObjectPtr
{
    constructor(comm, ws, object_id) {
	this.comm = comm;
	this.ws = ws;
	this.object_id = object_id;
    }
};

export class Communicator
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
