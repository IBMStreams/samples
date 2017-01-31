package com.acme.test.email.sender;

import com.ibm.streams.function.model.Function;

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;

public class EmailSender {
	// Define the String constants here.
	private static final String MAIL_HOST_KEYWORD = "mail.host";
	private static final String MAIL_TO_KEYWORD = "mailto:";
	private static final String FROM_KEYWORD = "From: ";
	private static final String TO_KEYWORD = "To: ";
	private static final String SUBJECT_KEYWORD = "Subject: ";
	private static final String NEWLINE_CHAR = "\n";

	@Function(name="sendEmail", description="Send email notifications", namespace="com.acme.test.email")
	public static void sendEmail(String mailServerAddress,String fromAddress,String toAddress,String subject,String messageContent) 
	{
		// Set the mail server address to the system property so that the
		// internal mailto: protocol handler can read it later.
		System.getProperties().put(MAIL_HOST_KEYWORD, mailServerAddress);
		
		URL url;
		String status;
		boolean statusFlag=true;
		
	
		try {
			// Create an URL connection to send the email.
			url = new URL(MAIL_TO_KEYWORD + toAddress);
		} catch (MalformedURLException ex) {
			// We got a malformed URL exception.
			status = "EmailSender:sendMsg-->MalformedURLException ("
					+ toAddress
					+ ")" ;
	
			//System.out.println(status);
			return;
		}
	
		URLConnection urlConnection;
	
		try {
			// Open the url connection now.
			urlConnection = url.openConnection();
		} catch (IOException ex) {
			// We couldn't open the URL connection.
	
			status = 
				"EmailSender:sendMsg-->IOException "
					+ "while opening URLConnection ("
					+ toAddress
					+ ")";
			//System.out.println(status	);
			return;
		}
	
		// We don't need any input from this.
		urlConnection.setDoInput(false);
		// We will (obviously) send output stuff using this.
		urlConnection.setDoOutput(true);
	
		try {
			// Let us now connect to the user specified mail server.
			urlConnection.connect();
		} catch (IOException ex) {
			// We couldn't connect to the mail server.
			
			status = "EmailSender:sendMsg-->IOException "
				+ "while connecting to the mail server ("
				+ mailServerAddress
				+ ")";
			//System.out.println(status);
			return;
		}
	
		OutputStreamWriter outStreamWriter;
	
		try {
			// Get an output stream writer for the mail server.
			outStreamWriter =
				new OutputStreamWriter(urlConnection.getOutputStream());
		} catch (IOException ex) {
			// We couldn't create an output stream for the mailserver.
			
			status="EmailSender:sendMsg-->IOException "
				+ "while creating an output stream for the "
				+ "mail server("
				+ mailServerAddress
				+ ")";
			//System.out.println(status);
			return;
		}
	
		// Create an output stream to the mail server.
		PrintWriter printWriter = new PrintWriter(outStreamWriter);
	
		if (printWriter == null) {
			// We couldn't create a print writer.
			
			status = "Unable to create a print writer for the "
				+ "mail server("
				+ mailServerAddress
				+ ")";
			//System.out.println(status);
			return;
		}
	
		// Write the From mail header.
		// fromAddress format should be: "Peter Pan" <peterPan@exploratorium.ibm.com>
		printWriter.print(FROM_KEYWORD + fromAddress + NEWLINE_CHAR);
		// toAddress format should be: "Captain Hook" <capHook@exploratorium.ibm.com>
		printWriter.print(TO_KEYWORD + toAddress + NEWLINE_CHAR);
		// Print the subject line now.
		printWriter.print(SUBJECT_KEYWORD + subject + NEWLINE_CHAR);
		// Print a new line char to end the list of mail headers.
		printWriter.print(NEWLINE_CHAR);
		//Print the message content now. 
		printWriter.print(messageContent);
	
		// The stream can be closed now so that the message will be
		// sent to the target address.        
		printWriter.close();
	
		
		status = "EmailSender: sendMsg-->Email has been sent."
			+ " (MailServer= "
			+ mailServerAddress
			+ ", ToAddress= "
			+ toAddress
			+ ", Subject= "
			+ subject
			+ ")";
		System.out.println(status);
		return;
	}
}
