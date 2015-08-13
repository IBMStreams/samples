
/* Copyright (C) 2015, International Business Machines Corporation */
/* All Rights Reserved */
package hdfshelper;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Iterator;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.Path;

/**
 * An iterator over an HDFS file.  When the file is empty, sends the string "done".
 * @author hildrum
 *
 */
public class HDFSFileReadIterator implements  Iterator<String> {

	final String filename; 
	BufferedReader reader; 
	boolean inputDone=false;


	public HDFSFileReadIterator(String coreSitePath, String file) {
		filename = file;
		inputDone=false;

		Configuration conf = new Configuration();
		conf.addResource(new Path(coreSitePath));
		File coreSiteFile = new File(coreSitePath);
		if (!coreSiteFile.exists()) {
			System.err.println("Warning: the core site file does not exist.");
		}
		FileSystem fSystem;
		try {
			fSystem = FileSystem.get(conf);
			if (fSystem == null) {
				System.out.println("Problem getting FileSystem");
			}
			reader = new BufferedReader( new InputStreamReader (fSystem.open(new Path(filename))));
			System.out.println("Reader successfully initialized");
		} catch (IOException e) {
			System.out.println("Caught IOException ");
			e.printStackTrace();
			reader = null;
		}
	}


	@Override
	public boolean hasNext() {
		return !inputDone;
	}

	@Override
	public String next() {

		try {
			String line = reader.readLine();
			if (line == null) {
				// File is done, let's cleanup.
				reader.close();
				reader = null;
				inputDone = true;
				return null;
			}
			else {
				return line;
			}
		}
		catch (IOException e) {
			inputDone = true;
			e.printStackTrace();
			return null;
		}
	}



	@Override
	public void remove() {
		throw new UnsupportedOperationException("remove not supported");
	}



}
