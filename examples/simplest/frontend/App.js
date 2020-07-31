import React from 'react';
import * as libpybx from 'libpybx-js';
import * as backend from './backend.js';

class HelloCBI extends backend.HelloCB {
    confirmHello(hello) {
	console.log("HelloCBI::confirmHello")
	return "confirmed: " + hello;
    }
};

class App extends React.Component {
    constructor(props) {
	super(props);
	this.comm = new libpybx.Communicator();
	this.state = {greeting: 'none', greeting2: 'none'};
	this.onClick = this.onClick.bind(this);
    }

    componentDidMount() {
	let ws_url = "ws://localhost:8080/";
	let object_id = "hello";
	this.comm.connect(ws_url, object_id).then(o_ptr => {
	    this.hello_ptr = new backend.HelloPtr(o_ptr);
	}).then(() => {
	    let hellocb_o = new HelloCBI();
	    let hellocb_o_ptr = this.comm.add_object(hellocb_o);
	    let hellocb_ptr = new backend.HelloCBPtr(hellocb_o_ptr);
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
	gs.push(new backend.Greetings("russian", "privet", "GREEN"));
	gs.push(new backend.Greetings("german", "halo", "GREEN"));
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
