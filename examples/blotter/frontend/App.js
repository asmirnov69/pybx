import React from 'react';
import * as libdipole from 'libdipole-js';
import * as Blotter from './Blotter.js';

class App extends React.Component {
    constructor(props) {
	super(props);
	this.comm = new libdipole.Communicator();
	this.state = {blotter: null}
	this.onClick = this.onClick.bind(this);
    }

    componentDidMount() {
	let ws_url = "ws://localhost:8080/";
	let object_id = "test_df";
	this.comm.connect(ws_url, object_id).then(o_ptr => {
	    this.blotter_ptr = new Blotter.DFTestPtr(o_ptr);
	    return this.blotter_ptr.get_df();
	}).then(df_json => {
	    console.log("server response:", df_json);
	    this.setState({...this.state, blotter: df_json.retval.dataframeJSON});
	});
    }
    
    onClick() {
    }
    
    render() {
	return (<div>
		<h1>{this.state.blotter}</h1>
		<button ref={r => this.pb = r} onClick={this.onClick}>PRESS</button>
	       </div>);
    }
};

export default App;
