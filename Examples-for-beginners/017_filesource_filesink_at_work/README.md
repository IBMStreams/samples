~~~~~~ Scala 
/*
We have used the FileSource and the FileSink operators in other examples
before. However, this example shows off the following intriguing
features that will become handy. 

a) Automatic deletion of a file after the FileSource reads all the records.
b) Flushing the sink file on demand after writing a certain number of tuples.
c) Ability of the FileSource to move the file once it is done with it.
d) Creating a fresh and new output sink file after writing certain number of tuples.
e) Ability of the FileSource to keep reading from a hot file as new CSV records get written.
*/
namespace my.sample;
// In order to test this application, you have to ensure that two 
// sub-directories "test1", "test2" are present inside this application's directory.
// Then, you have to ensure that all the files in "data" directory are copied to "test1".
//
// To test the hotFile function, you have to keep appending new records
// to the test1/DepartmentRecordsFile using a text editor.
composite Main {
	type
		cityData = tuple<rstring city, rstring country, uint32 population, uint32 medianAge, uint32 percentageEducated>;
		employee = tuple<rstring name, uint32 employeeDepartment>;
		department = tuple<uint32 departmentId, rstring departmentName>;	

		graph
			// This FileSource will delete the file after it is fully read..
			stream <cityData> CityDataRecord = FileSource() {
				param
					file: "../test1/Population.txt";
					format: csv;
					deleteFile: true;
					hasDelayField: true;
			} // End of CityDataRecord = FileSource()	
			
		    // This FileSink will flush every 4 tuples.
		    () as FileSink1 = FileSink(CityDataRecord) {
		    	param
		    		file: "../test1/result_population.txt";
		    		flush: 4u;
		    } // End of FileSink1 = FileSink(CityDataRecord)

			// This FileSource will move the file after it is fully read..
			stream <employee> EmployeeRecord = FileSource() {
				param
					file: "../test1/EmployeeRecords.txt";
					format: csv;
					moveFileToDirectory: "../test2";
					hasDelayField: true;
			} // End of EmployeeRecord = FileSource()	
			
		    // This FileSink will create a new output file for every 2 tuples.
		    () as FileSink2 = FileSink(EmployeeRecord) {
		    	param
		    		file: "../test2/result_employee{id}.txt";
		    		closeMode: count;
		    		tuplesPerFile: 2u;
		    } // End of FileSink2 = FileSink(EmployeeRecord)

			// This FileSource will keep reading tuples from a hot file
			stream <department> DepartmentRecord = FileSource() {
				param
					file: "../test1/DepartmentRecords.txt";
					format: csv;
					hotFile: true;
					hasDelayField: true;
			} // End of DepartmentRecord = FileSource()	
			
		    // This FileSink will flush the file for every 2 new tuples.
		    () as FileSink3 = FileSink(DepartmentRecord) {
		    	param
		    		file: "../test1/result_department.txt";
		    		flush: 2u;
		    } // End of FileSink3 = FileSink(DepartmentRecord)
			
}

~~~~~~
