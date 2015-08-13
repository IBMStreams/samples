/* Copyright (C) 2015, International Business Machines Corporation */
/* All Rights Reserved */
package app;


import hdfshelper.HDFSFileReader;

import com.ibm.streamsx.topology.TStream;
import com.ibm.streamsx.topology.Topology;
import com.ibm.streamsx.topology.context.StreamsContextFactory;

public class ReadAndPrint {

	// Set these for your install.
	private static final String hadoopDir="/opt/ibm/biginsights/IHC/";
	private static String hadoopConfDir="/opt/ibm/biginsights/hadoop-conf/";
	
	
	public static void main(String argv[]) throws Exception{
		//final String filename = "inputTwitterMessages.txt";
		final String filename = "enron100.txt";
		Topology flow = new Topology();
		
	
		TStream<String> files  = flow.strings(filename);
				
		TStream<String> lines =files.multiTransform(new HDFSFileReader(hadoopConfDir+"/core-site.xml"));
		
		lines.print();
		
		flow.addJarDependency(hadoopDir+"/share/hadoop/common/lib/commons-configuration-1.6.jar");
		flow.addJarDependency(hadoopDir+"/share/hadoop/common/lib/commons-cli-1.2.jar");
		flow.addJarDependency(hadoopDir+"/hadoop-core.jar");
		flow.addJarDependency(hadoopDir+"/share/hadoop/common/lib/commons-logging-1.1.1.jar");
		//StreamsContextFactory.getEmbedded().submit(flow).get();
		StreamsContextFactory.getStreamsContext("EMBEDDED").submit(flow).get();
	}
		
}
	
