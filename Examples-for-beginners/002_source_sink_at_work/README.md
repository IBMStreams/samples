~~~~~~ Scala 
/*
This example shows how a FileSource operator can be used to
read CSV formatted records from a file and then receive those 
tuples in a FileSink to be written to another file in the data 
directory of this application.
*/
namespace sample;

composite source_sink_at_work {
	type 
		EmployeeSchema = tuple<rstring name, rstring title, rstring department, float64 salary>;

	graph
		stream<EmployeeSchema> Employee = FileSource() {
			param
				file: "Employee.dat";
				format: csv;
				initDelay: 4.0;
	} // End of Employee = FileSource()

	() as FileWriter = FileSink(Employee) {
		param
			file: "/tmp/MyResults.txt";
	} // End of FileSink.


	() as Printer = Custom(Employee as in0){
			logic
			onTuple in0: {
				println(in0);
			}
		}
} // End of Composite source_sink_at_work

~~~~~~


## Sample output

```
{name="John Tesh",title="Software Engineer",department="DB2 Development",salary=93000.5}
{name="Mary Mason",title="Accountant",department="Finance",salary=105000.26}
{name="Chris Coda",title="Hardware Engineer",department="PPC Design",salary=115292.94}
{name="Gretta Jeno",title="Group Manager",department="Legal",salary=109318.51}

```