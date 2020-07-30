import React, { useState } from 'react';
import { Grid,
	 Table,
	 TableHeaderRow,
	 DragDropProvider,
	 TableColumnReordering } from '@devexpress/dx-react-grid-bootstrap4';
import '@devexpress/dx-react-grid-bootstrap4/dist/dx-react-grid-bootstrap4.css';

import * as libdipole from 'libdipole-js';
import * as Blotter from './Blotter.js';

function App() {
    let comm = new libdipole.Communicator();
    const [columns, setColumns] = useState([{ name: 'id', title: 'ID' },
				{ name: 'product', title: 'Product' },
				{ name: 'owner', title: 'Owner' }]);
    const [rows] = useState([{ id: 0, product: 'DevExtreme', owner: 'DevExpress' },
			     { id: 1, product: 'DevExtreme Reactive', owner: 'DevExpress' }
			    ]);

    const [columnOrder, setColumnOrder] = useState(['id', 'product', 'owner']);

    let blotter_ptr;
    React.useEffect(() => {
	let ws_url = "ws://localhost:8080/";
	let object_id = "test_df";	
	comm.connect(ws_url, object_id).then(o_ptr => {
	    blotter_ptr = new Blotter.DFTestPtr(o_ptr);
	    return blotter_ptr.get_df();
	}).then(df_json => {
	    console.log("server response:", df_json);
	    //this.setState({...this.state, blotter: df_json.retval.dataframeJSON});
	});
    });

    const onClick = () => {
	blotter_ptr.get_df().then(df_json => {
	    console.log("onClick:", df_json);
	});
    }
    
    return (<div>
	    <div className="card">
	    <Grid rows={rows} columns={columns}>
	    <DragDropProvider />
	    <Table />
	    <TableColumnReordering order={columnOrder}
	    onOrderChange={setColumnOrder}
	    />
	    <TableHeaderRow />
	    </Grid>
	    </div>
	    <button onClick={onClick}>PRESS</button>
	    </div>);
}

export default App;
