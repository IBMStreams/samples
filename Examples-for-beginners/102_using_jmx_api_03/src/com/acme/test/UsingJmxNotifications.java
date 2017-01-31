/*
====================================================================
This is a plain Java application written to show how one can use
the JMX APIs available in Streams 4.x and higher versions.
Using the JMX (Java Management Extensions) APIs, it is convenient to
monitor and manage Streams artifacts.

Following example code is taken from the Streams Knowledge Center.
Full credit for this code goes to my colleague Janet Weber who wrote it.

Streams users can write their own management applications using
this versatile set of JMX APIs shipped with Streams.
There are three important JAR files that need to be included in
your Java applications that use the JMX API as shown below.

For this particular project, if you right click on this project
in Eclipse and select Properties, a dialog box will open up.
In that, click on "Java Build Path" in the left navigation bar.
On the right hand side, select the "Libraries" tab and you will
have to ensure that the following three required JAR files are added to this project.
If not, you have to add them using the "Add External Jars" button.
If they are already added and if they point to a different Streams installation
directory than yours, you have to delete them and add them fresh from your
Streams installation directory.
i.e. $STREAMS_INSTALL/<STREAMS_VERSION>/lib
     and
     $STREAMS_INSTALL/<STREAMS_VERSION>/ext/lib  

Following are the three required jar files.
1) $STREAMS_INSTALL/<STREAMS_VERSION>/lib/com.ibm.streams.management.jmxmp.jar
2) $STREAMS_INSTALL/<STREAMS_VERSION>/lib/com.ibm.streams.management.mx.jar
3) $STREAMS_INSTALL/<STREAMS_VERSION>/ext/lib/jmxremote_optional.jar

IMPORTANT: 
1) Please note that the com.ibm.streams.management.jmxmp.jar file should
appear before the jmxremote_optional.jar in your classpath.

2) To use the JMX APIs successfully, users must be authorized to access
InfoSphere Streams objects. You can use the Streams web console to configure
the access control lists. Alternatively, you can use the streamtool setacl to
set instance permissions and the streamtool setdomainacl to set domain permissions.


If you are compiling it from the command line outside of the Streams Studio,
you can do as shown below:

java -cp .;com.ibm.streams.management.jmxmp.jar:com.ibm.streams.management.mx.jar:jmxremote_optional.jar  <YOUR_JAVA_FILE_NAME>

To run this application from Streams Studio:
1) This application is about monitoring a running Streams job. You must start a Streams application first and note down its
   job id. (For example, you can start the 099_consistent_region_java_10 example
   included in the Examples-for-beginners-For-Beginners collection to test this JMX API feature.)
2) Select the top level application project in the Package Explore on your left.
3) Select Run-->Run Configurations from the Streams Studio menu bar.
4) In the resulting dialog box, select the "Arguments" tab and then provide the 6
   values (separated by spaces in between them) for JMX URL, Streams domain name, instance name,
   A streams job id to monitor, user id and password.
5) Now, click Apply and Run buttons in that dialog box.
6) After verifying this JMX API, you can stop the Streams job you started to test this JMX API.

In the example code below, we will use the JMX API to register a running Streams job and keep it 
registered by handling inactivity warning notifications. In order to get any meaningful
notifications, you have to set the following domain property to a very low level (1 minute instead of
the default value of 30 minutes).

 First get the current value and remember it:
 streamtool setdomainproperty -d <YOUR_DOMAIN_NAME> jmx.inactivityTimeout
  
 streamtool setdomainproperty -d <YOUR_DOMAIN_NAME> jmx.inactivityTimeout=1
 
 [After testing this JMX application, you can set that domain property back to its original value.]
====================================================================
*/
package com.acme.test;

import java.math.BigInteger;
import java.util.HashMap;

import javax.management.JMX;
import javax.management.MBeanServerConnection;
import javax.management.ObjectName;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;
import javax.management.Notification;
import javax.management.NotificationFilterSupport;
import javax.management.NotificationListener;

import com.ibm.streams.management.Notifications;
import com.ibm.streams.management.ObjectNameBuilder;
import com.ibm.streams.management.instance.InstanceMXBean;
import com.ibm.streams.management.job.JobMXBean;


public class UsingJmxNotifications implements NotificationListener {
	JobMXBean _job;
	int _inactivityCount = 0;
    
	public UsingJmxNotifications (String jmxUrl, String domainName, String instanceName, BigInteger jobId, String user, String password) {
		try {             
			HashMap<String, Object> env = new HashMap<String, Object>();
			String [] creds = {user, password};
			env.put("jmx.remote.credentials", creds);
			env.put("jmx.remote.protocol.provider.pkgs", "com.ibm.streams.management");
			JMXConnector jmxc = JMXConnectorFactory.connect(new JMXServiceURL(jmxUrl), env);
			MBeanServerConnection mbsc = jmxc.getMBeanServerConnection();
    	    	                                      
			ObjectName instanceObjName = ObjectNameBuilder.instance(domainName, instanceName);
			InstanceMXBean instance = JMX.newMXBeanProxy(mbsc, instanceObjName, InstanceMXBean.class, true);
    	    	                                                  
			ObjectName jobObjName = instance.registerJob(jobId);
			_job = JMX.newMXBeanProxy(mbsc, jobObjName, JobMXBean.class, true);
			System.out.println("Job name: " + _job.getName());
			NotificationFilterSupport filter = new NotificationFilterSupport();
			filter.enableType(Notifications.INACTIVITY_WARNING);
			mbsc.addNotificationListener(jobObjName, this, filter, null);
			jmxc.addConnectionNotificationListener(this,null,null); // listen for potential lost notifications
		}catch (Exception ex) {
			ex.printStackTrace();
		}
	}
      
	public void showStatus() {
		try {
			while (true) {  
				Thread.sleep(30000);
				System.out.println("Number of times in status loop since last job access: " + ++_inactivityCount);
			}
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
           
	public void handleNotification(Notification notification, Object handback) { 
		System.out.println("Notification type: " + notification.getType());
		System.out.println("Notification source: " + notification.getSource());
		_job.keepRegistered();  // call on inactivity warning of if notifications were potentially lost
		_inactivityCount = 0;
	}
           
	public static void main(String[] args) {
		try {
			UsingJmxNotifications client = new UsingJmxNotifications(
				args[0],  // jmxUrl - use streamtool getjmxconnect to find
				args[1],  // domainName
				args[2],  // instanceName
				new BigInteger(args[3]),  // jobId
				args[4],  // user - use streamtool setacl // or streamtool setdomainacl to assign required permissions
				args[5]); // password
			client.showStatus();
		} catch (Exception ex) {
			ex.printStackTrace();
		}
	}
}
