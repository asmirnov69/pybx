import React, { useState, useRef } from 'react';
import {
  GroupingState,
  IntegratedGrouping,
} from '@devexpress/dx-react-grid';
import { Grid,
	 Table,
	 TableHeaderRow,
	 GroupingPanel,
	 DragDropProvider,
	 Toolbar,
	 TableColumnReordering,
         TableGroupRow
       } from '@devexpress/dx-react-grid-bootstrap4';
import '@devexpress/dx-react-grid-bootstrap4/dist/dx-react-grid-bootstrap4.css';

import * as libpybx from 'libpybx-js';
import * as Blotter from './Blotter.js';

class ObserverI extends Blotter.Observer
{
    constructor(set_row_func, set_update_c_func) {
	super();	
	this.set_row_func = set_row_func;
	this.set_update_c_func = set_update_c_func;
    }
    
    show(df) {
	console.log("ObserverI::show:", df);
	let df_rows = JSON.parse(df.df.dataframeJSON)
	this.set_row_func(df_rows);
	this.set_update_c_func(df.update_c);
    }
};
	

function App() {
    let comm = new libpybx.Communicator();
    const blotter_rop = useRef(null);
    const [update_c, set_update_c] = useState(0);
    const [columns, setColumns] = useState([]);
    const [rows, setRows] = useState([]);
    const [columnOrder, setColumnOrder] = useState([]);

    React.useEffect(() => {
	let ws_url = "ws://localhost:8080/";
	let object_id = "test_df";	
	comm.get_rop(Blotter.DFTest, ws_url, object_id).then(o_rop => {
	    blotter_rop.current = o_rop;
	}).then(() => {
	    let o_obj = new ObserverI(setRows, set_update_c);
	    let s_rop = comm.add_object(o_obj, "aa")
	    return blotter_rop.current.subscribe(s_rop);
	}).then(() => {
	    console.log("connection setup is done");
	});
    }, []);

    const onClick = () => {
	blotter_rop.current.get_df().then(df => {
	    console.log("onClick:", df);
	    set_update_c(df.update_c);
	    setColumns(df.df.columns.map(x => { return {name: x}; }));
	    setColumnOrder(df.df.columns);
	    let df_rows = JSON.parse(df.df.dataframeJSON)
	    setRows(df_rows);
	});
    }
    
    return (<div>
	    <div className="card">
	    <Grid rows={rows} columns={columns}>
	    <GroupingState />
	    <DragDropProvider />
	    <IntegratedGrouping />
	    <Table />
	    <TableHeaderRow />
	    <TableGroupRow />
	    <TableColumnReordering order={columnOrder} onOrderChange={setColumnOrder} />
	    <Toolbar />
            <GroupingPanel />
	    </Grid>
	    </div>
	    <h1>{update_c}</h1>
	    <button onClick={onClick}>PRESS</button>
	    </div>);
}

export default App;
