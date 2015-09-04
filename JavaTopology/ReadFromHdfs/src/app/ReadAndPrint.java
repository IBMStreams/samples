package app;


import hdfshelper.HDFSFileReader;

import com.ibm.streamsx.topology.TStream;
import com.ibm.streamsx.topology.Topology;
import com.ibm.streamsx.topology.function.Supplier;
import com.ibm.streamsx.topology.context.StreamsContextFactory;

public class ReadAndPrint {

	// Set these for your install.
	private static final String hadoopDir="/opt/ibm/biginsights/IHC";
	
	// We expect core-site.xml to be in this directory
	private static final String hadoopConfDir="/opt/ibm/biginsights/hadoop-conf/";

	public static void addHdfsDependencies(Topology flow) {
		flow.addJarDependency(hadoopDir+"/share/hadoop/common/lib/commons-configuration-1.6.jar");
		flow.addJarDependency(hadoopDir+"/share/hadoop/common/lib/commons-cli-1.2.jar");
		flow.addJarDependency(hadoopDir+"/hadoop-core.jar");
		flow.addJarDependency(hadoopDir+"/share/hadoop/common/lib/commons-logging-1.1.1.jar");
	}
	
	public static void main(String argv[]) throws Exception {
		// Uses ReadHDFSFile as a source 
		sourceDemo(argv);
		// Uses ReadHDFSFile as a multiTransform
		multiTransformDemo(argv);
	}
	
	public static void sourceDemo(String argv[]) throws Exception{
		//final String filename = "inputTwitterMessages.txt";
		final String filename = "enron100.txt";
		Topology flow = new Topology();
		TStream<String> lines = flow.source(new HDFSFileReader(hadoopConfDir+"/core-site.xml",filename));
		lines.print();
		
		addHdfsDependencies(flow);
		StreamsContextFactory.getStreamsContext("EMBEDDED").submit(flow).get();
	}
	
	public static void multiTransformDemo(String argv[]) throws Exception{
		final String filename = "inputTwitterMessages.txt";
		Topology flow = new Topology();
	
		// Let's read the file iterations times, where iterations can be
		// an argument.
		long iterations = 1;
		if (argv.length > 0) {
			iterations = Long.parseLong(argv[0]);
		}
		
		TStream<String> files = flow.limitedSource(new Supplier<String>() {
			private static final long serialVersionUID = 1L;

			@Override
			public String get() {
				return filename;
			}
		},iterations );
		
		// There are other ways of getting the filename stream--
		// Or we could do to read the file just once
		//TStream<String> files  = flow.strings("file1");
			
		// Or to read  two files: 
		//TStream<String> files  = flow.strings("file1","file2");
		
		TStream<String> lines =files.multiTransform(new HDFSFileReader(hadoopConfDir+"/core-site.xml"));
		
		lines.print();
		
		addHdfsDependencies(flow);
		StreamsContextFactory.getStreamsContext("EMBEDDED").submit(flow).get();
	}
	

		
}
	
