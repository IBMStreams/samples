package com.acme.test.email.sender;

@com.ibm.streams.operator.internal.model.ShadowClass("com.acme.test.email.sender.EmailSender")
@javax.annotation.Generated("com.ibm.streams.operator.internal.model.processors.ShadowClassGenerator")
public class EmailSender$StreamsModel {

@com.ibm.streams.function.model.Function(name="sendEmail", description="Send email notifications", namespace="com.acme.test.email")
public static void sendEmail(java.lang.String mailServerAddress, java.lang.String fromAddress, java.lang.String toAddress, java.lang.String subject, java.lang.String messageContent) {}
}