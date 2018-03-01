//*******************************************************************************
//* Copyright (C)2018, International Business Machines Corporation and *
//* others. All Rights Reserved. *
//***
package com.ibm.streamsx.bridge.http;

public class FailedToFindStreamsException extends Exception {

	public FailedToFindStreamsException() {

	}

	public FailedToFindStreamsException(String message)

	{
		super(message);
	}

	public FailedToFindStreamsException(Throwable cause) {
		super(cause);
	}

	public FailedToFindStreamsException(String message, Throwable cause) {
		super(message, cause);
	}

	public FailedToFindStreamsException(String message, Throwable cause, boolean enableSuppression,
			boolean writableStackTrace) {
		super(message, cause, enableSuppression, writableStackTrace);
	}

}
