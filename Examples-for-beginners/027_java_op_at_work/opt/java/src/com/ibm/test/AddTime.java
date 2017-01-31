package com.ibm.test;

import com.ibm.streams.operator.types.RString;
import com.ibm.streams.operator.OperatorContext;
import java.text.*;
import java.util.*;

public  class AddTime extends AbstractAddTime {


    /**
    ** Initialize the operator
    */
    @Override
    public void initialize(OperatorContext context) throws Exception {
        super.initialize(context);
        // TODO Auto-generated method stub
    }

    /**
    ** process method for port 0.
    */
    @Override
    protected void process0(IPort0 tuple) throws Exception {
        // TODO Auto-generated method stub
    	// In this Java operator, we will simply add the
    	// current DateTime value to the dateTime attribute and send the tuple.
    	DateFormat dateFormat = new SimpleDateFormat("MM/dd/yyyy HH:mm:ss");
    	Date date = new Date();
    	RString currentDateTimeStr = new RString(dateFormat.format(date));
    	
    	// Generate the Output tuple.
        OPort0 outputTuple = getOutput0().newTuple();
        outputTuple.set_symbol (tuple.get_symbol());
        outputTuple.set_price (tuple.get_price());
        outputTuple.set_quantity (tuple.get_quantity());
        outputTuple.set_tradeType (tuple.get_tradeType());
        outputTuple.set_dateTime (currentDateTimeStr);
        submit0 (outputTuple);
    }
}
