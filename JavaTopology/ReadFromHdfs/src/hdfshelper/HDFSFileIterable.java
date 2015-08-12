package hdfshelper;

import java.util.Iterator;

public class HDFSFileIterable implements Iterable<String> {

	final String filename;
	final String coreSite;
	public HDFSFileIterable(String coreSitePath, String file) {
		filename  = file;
		coreSite = coreSitePath;
	}
	@Override
	public Iterator<String> iterator() {
		return new HDFSFileReadIterator(coreSite, filename);
	}

}
