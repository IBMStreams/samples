package com.ibm.test;

import com.ibm.streams.operator.types.RString;

import com.ibm.streams.operator.OperatorContext;
import com.ibm.streams.operator.AbstractOperator;
import com.ibm.streams.operator.OutputTuple;
import com.ibm.streams.operator.StreamingInput;
import com.ibm.streams.operator.Tuple;
import com.ibm.streams.operator.StreamingOutput;
import com.ibm.streams.operator.internal.operator.StreamingOutputWrapper;

public abstract class AbstractAddTime extends AbstractOperator {


    /**
    ** Initialize the operator
    */
    @Override
    public void initialize(OperatorContext context) throws Exception {
        super.initialize(context);
        checkPorts(1,1);
        output0 = new StreamingOutputWrapper<OPort0>(getOutput(0)) {
            protected OutputTuple unwrap(OPort0 tuple) {
                return tuple.getOutputTuple();
            }
            public OPort0 newTuple() {
                final OutputTuple tuple = getRealOutput().newTuple();
                return new OPort0() {
                    public RString get_symbol(){
                        return (RString) tuple.getObject(0);
                    }
                    public float get_price(){
                        return tuple.getFloat(1);
                    }
                    public int get_quantity(){
                        return tuple.getInt(2);
                    }
                    public RString get_tradeType(){
                        return (RString) tuple.getObject(3);
                    }
                    public RString get_dateTime(){
                        return (RString) tuple.getObject(4);
                    }
                    public void set_symbol(final RString value){
                        tuple.setObject(0, value);
                    }
                    public void set_price(final float value){
                        tuple.setFloat(1, value);
                    }
                    public void set_quantity(final int value){
                        tuple.setInt(2, value);
                    }
                    public void set_tradeType(final RString value){
                        tuple.setObject(3, value);
                    }
                    public void set_dateTime(final RString value){
                        tuple.setObject(4, value);
                    }
                    public OutputTuple getOutputTuple() {
                        return tuple;
                    }
                }
;
            }
            };
    }
    /**
    ** Tuple interface for input port 0.
    **
    ** Stream Schema
    ** rstring symbol, float32 price, uint32 quantity, rstring tradeType, rstring dateTime
    */
    public interface IPort0 {
        public RString get_symbol();
        public float get_price();
        public int get_quantity();
        public RString get_tradeType();
        public RString get_dateTime();
        /**
        ** Obtain the generic view of this tuple.
        */
        public Tuple getTuple();
    }

    /**
    ** process method for port 0.
    */
    protected abstract void process0(IPort0 tuple) throws Exception;

    /**
    ** Tuple interface for output port 0.
    **
    ** Stream Schema
    ** rstring symbol, float32 price, uint32 quantity, rstring tradeType, rstring dateTime
    */
    public interface OPort0 {
        public RString get_symbol();
        public float get_price();
        public int get_quantity();
        public RString get_tradeType();
        public RString get_dateTime();
        public void set_symbol(final RString value);
        public void set_price(final float value);
        public void set_quantity(final int value);
        public void set_tradeType(final RString value);
        public void set_dateTime(final RString value);
        /**
        ** Obtain the generic view of this tuple.
        */
        public OutputTuple getOutputTuple();
    }

    /**
    ** submit method for port 0.
    */
    public final synchronized boolean submit0(OPort0 tuple) throws Exception {
        return getOutput0().submit(tuple);
    }
    private StreamingOutput<OPort0> output0;

    /**
    ** getOutput method for port 0.
    */
    public final StreamingOutput<OPort0> getOutput0() {
        return this.output0;
    }

    @Override
    public final synchronized void process(final StreamingInput input, final Tuple tuple) throws Exception {
        switch (input.getPortNumber()) {
        case 0:
            process0(new IPort0() {
                    public RString get_symbol(){
                        return (RString) tuple.getObject(0);
                    }
                    public float get_price(){
                        return tuple.getFloat(1);
                    }
                    public int get_quantity(){
                        return tuple.getInt(2);
                    }
                    public RString get_tradeType(){
                        return (RString) tuple.getObject(3);
                    }
                    public RString get_dateTime(){
                        return (RString) tuple.getObject(4);
                    }
                    public Tuple getTuple() {
                        return tuple;
                    }
                }
            );
            break;
        }
    }
}
