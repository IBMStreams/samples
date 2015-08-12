package hdfshelper;

import com.ibm.streamsx.topology.function.Function;
import com.ibm.streamsx.topology.function.Supplier;

public class HDFSFileReader implements Function<String, Iterable<String>>, Supplier<Iterable<String>> {

	private final String filename;
	private final String coreSite;
	private static final long serialVersionUID = 1L;

	public HDFSFileReader(String coreSite,String filename) {
		this.filename = filename;
		this.coreSite = coreSite;
	}
	
	public HDFSFileReader(String coreSite) {
		this.filename = null;
		this.coreSite = coreSite;
	}
	
	@Override
	public Iterable<String> get() {
		return new HDFSFileIterable(coreSite,this.filename);
	}

	@Override
	public Iterable<String> apply(String file) {
		return new HDFSFileIterable(coreSite,file);
	}
}
