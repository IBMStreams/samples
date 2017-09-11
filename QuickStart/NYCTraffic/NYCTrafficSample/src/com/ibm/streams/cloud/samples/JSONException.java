package com.ibm.streams.cloud.samples;

public class JSONException extends Exception {
	private Throwable cause;

    /**
     * Constructs a JSONException with an explanatory message.
     * @param message Detail about the reason for the exception.
     */
	public JSONException() {
		super();
	}
	
	public JSONException(String message, Throwable cause) {
		super(message, cause);
	}
	
    public JSONException(String message) {
        super(message);
    }

    public JSONException(Throwable t) {
        super(t.getMessage());
        this.cause = t;
    }

    public Throwable getCause() {
        return this.cause;
    }

}
