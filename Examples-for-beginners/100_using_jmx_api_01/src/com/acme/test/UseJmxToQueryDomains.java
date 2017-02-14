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
1) Select the top level application project in the Package Explore on your left.
2) Select Run-->Run Configurations from the Streams Studio menu bar.
3) In the resulting dialog box, select the "Arguments" tab and then provide the 4
   values (separated by spaces in between them) for JMX URL, Streams domain name,
   user id and password.
4) Now, click Apply and Run buttons in that dialog box.

In the example code below, we will query the Streams domain status and
list all the available Streams instances in that domain. In addition,
we will query a few other values pertinent to a given domain.
====================================================================
*/
package com.acme.test;

//Import the JMX classes.
import javax.management.JMX;
import javax.management.MBeanServerConnection;
import javax.management.ObjectName;
import javax.management.remote.JMXConnector;
import javax.management.remote.JMXConnectorFactory;
import javax.management.remote.JMXServiceURL;
import com.ibm.streams.management.domain.DomainMXBean;
import com.ibm.streams.management.ObjectNameBuilder;

import java.util.HashMap;

public class UseJmxToQueryDomains {	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		try { 
			// use streamtool getjmxconnect to find the JMX URL.
			String jmxUrl = args[0];  
			String domainName = args[1];
			// use streamtool setacl or streamtool setdomainacl to assign required permissions
			String user = args[2];    
			String password = args[3];
    	                                    
			HashMap<String,Object> env = new HashMap<String,Object>();
			String [] creds = {user, password};
			env.put("jmx.remote.credentials", creds);
			env.put("jmx.remote.protocol.provider.pkgs", "com.ibm.streams.management");
			JMXConnector jmxc = JMXConnectorFactory.connect(new JMXServiceURL(jmxUrl), env);
			MBeanServerConnection mbsc = jmxc.getMBeanServerConnection();
    	                                                            
			ObjectName objName = ObjectNameBuilder.domain(domainName);
			DomainMXBean domain = JMX.newMXBeanProxy(mbsc, objName, DomainMXBean.class, true);
    	                                                                        
			System.out.println("Status: " + domain.getStatus());
			System.out.println("Instances: " + domain.getInstances());
			System.out.println("Application Bundle Path: " + domain.getApplicationBundlesPath());
			System.out.println("Application current working path: " + domain.getApplicationCurrentWorkingPath());
			System.out.println("Streams version: " + domain.getActiveVersion());
			System.out.println("Zookeeper connection string: " + domain.getZooKeeperConnectionString());
			System.out.println("Machines used by this domain: " + domain.getResources());
			System.out.println("Machine tags used in this domain: " + domain.getResourceTags());
			System.out.println("Domain services: " + domain.getServices());
			System.out.println("Domain properties: " + domain.getProperties(true));
		} catch (Exception ex) {
			ex.printStackTrace();     
		}   
	}
}
