~~~~~~ Scala 
/*
This example demonstrates one of the important features desired in the
real-world (mostly in the retail banking and in the telco industries).
In many real-world scenarios, they still work via files that get dropped
into a directory for processing. It is shown here how the DirectoryScan
operator picks up a file as soon as it appears inside an input directory.
*/
namespace my.sample;
// In order to test this application, you have to ensure that three 
// sub-directories "test1", "test2", "test3" are present inside this application's directory.
// Then, you have to do the following:
// 1) Copy the data/Population.txt file to test1.
// 2) Copy the data/EmployeeRecords.txt file to test2.
// 3) Copy the data/DepartmentRecords.txt file to test3.
//
// In each of those three sub-directories (test1, test2, test3),
// keep copying the same file again and again in different filenames.
// Every new file you create there should be picked up by the DirectoryScan operator.

composite Main {
	type
		cityData = tuple<rstring city, rstring country, uint32 population, uint32 medianAge, uint32 percentageEducated>;
		employee = tuple<rstring name, uint32 employeeDepartment>;
		department = tuple<uint32 departmentId, rstring departmentName>;	

		graph
			// Directory scan for city data records.
			stream <rstring cityDataFile> CityDataFile = DirectoryScan() {
				param
					directory: "../test1";
					sleepTime: 3.0;
					pattern: ".*\\.txt";
					initDelay: 3.0; 
			} // End of CityDataFile = DirectoryScan()
			
			// Directory scan for employee data records.
			stream <rstring employeeDataFile> EmployeeDataFile = DirectoryScan() {
				param
					directory: "../test2";
					sleepTime: 3.0;
					pattern: ".*\\.txt";
					initDelay: 3.0; 
			} // End of EmployeeDataFile = DirectoryScan()

			// Directory scan for department data records.
			stream <rstring departmentDataFile> DepartmentDataFile = DirectoryScan() {
				param
					directory: "../test3";
					sleepTime: 3.0;
					pattern: ".*\\.txt";
					initDelay: 3.0; 
			} // End of DepartmentDataFile = DirectoryScan()
			
			// This FileSource will keep consuming data from any new file appearing in test1 directory.
			stream <cityData> CityDataRecord = FileSource(CityDataFile) {
				param
					format: csv;
					hasDelayField: true;
			} // End of CityDataRecord = FileSource()	
			
		    // This FileSink will flush every 4 tuples.
		    () as FileSink1 = FileSink(CityDataRecord) {
		    	param
		    		file: "../test1/result_population.csv";
		    		flush: 4u;
		    } // End of FileSink1 = FileSink(CityDataRecord)

			// This FileSource will keep consuming data from any new file appearing in test2 directory.
			stream <employee> EmployeeRecord = FileSource(EmployeeDataFile) {
				param
					format: csv;
					hasDelayField: true;
			} // End of EmployeeRecord = FileSource()	
			
		    // This FileSink will create a new output file for every 2 tuples.
		    () as FileSink2 = FileSink(EmployeeRecord) {
		    	param
		    		file: "../test2/result_employee.csv";
					flush: 4u;
		    } // End of FileSink2 = FileSink(EmployeeRecord)

			// This FileSource will keep consuming data from any new file appearing in test2 directory.
			stream <department> DepartmentRecord = FileSource(DepartmentDataFile) {
				param
					format: csv;
					hasDelayField: true;
			} // End of DepartmentRecord = FileSource()	
			
		    // This FileSink will flush the file for every 2 new tuples.
		    () as FileSink3 = FileSink(DepartmentRecord) {
		    	param
		    		file: "../test3/result_department.csv";
		    		flush: 4u;
		    } // End of FileSink3 = FileSink(DepartmentRecord)	
}

~~~~~~
