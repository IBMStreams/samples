~~~~~~ Scala 
/*
This example shows one of the power-packed standard toolkit operators;
i.e. Join. This operator is so versatile that it is hard to do justice in 
explaining it thoroughly in a simple example such as this one. 
This example provides coverage to the following Join operator features.

a) Inner Join
b) Inner (Equi) Join
c) Left Outer Join
d) Right Outer Join
e) Full Outer Join
*/
namespace my.sample;

composite Main {
	type
		employee = tuple<rstring name, uint32 employeeDepartment>;
		department = tuple<uint32 departmentId, rstring departmentName>;
		orders = tuple<uint32 orderId, rstring customerName, rstring orderDate, rstring product, uint32 quantity>;
		inventory = tuple<uint32 productId, rstring productName, uint32 quantity, float32 price>;
		
	graph
		stream <employee> EmployeeRecord = FileSource() {
			param
				file:	"EmployeeRecords.txt";
				format:	csv;
				hasDelayField: true;
				initDelay: 2.0;
		} // End of EmployeeRecord = FileSource()
				
		stream <department> DepartmentRecord = FileSource() {
			param
				file:	"DepartmentRecords.txt";
				format:	csv;
				hasDelayField: true;
				initDelay: 2.0;
		} // End of DepartmentRecord = FileSource()

		stream <orders> OrderRecord = FileSource() {
			param
				file:	"orders.txt";
				format:	csv;
				hasDelayField: true;
				initDelay: 2.0;
		} // End of OrderRecord = FileSource()
		
		stream <inventory> InventoryRecord = FileSource() {
			param
				file:	"inventory.txt";
				format:	csv;
				hasDelayField: true;
				initDelay: 2.0;
		} // End of InventoryRecord = FileSource()
		
		// Inner Join of two streams.
		stream <employee, department> InnerJoin1 = Join(EmployeeRecord; DepartmentRecord) {
			window
				EmployeeRecord: sliding, count(20);
				DepartmentRecord: sliding, count(20);
				
			param
				match: EmployeeRecord.employeeDepartment == DepartmentRecord.departmentId;
				algorithm: inner;
		} // End of InnerJoin1 = Join(EmployeeRecord; DepartmentRecord)
		
		// Inner (Equi) Join of two streams.
		stream <orders, inventory, tuple<uint32 quantity2>> InnerEquiJoin1 = Join(OrderRecord; InventoryRecord) {
			window
				OrderRecord: sliding, delta(orderId, 1u);
				InventoryRecord: sliding, time(7);
			
			param
				algorithm: inner;
				match: productId == 125u;
				equalityLHS: product;
				equalityRHS: productName;
				
			output
				InnerEquiJoin1: 
					quantity = OrderRecord.quantity,
					quantity2 = InventoryRecord.quantity;
		} // End of InnerEquiJoin1 = Join(OrderRecord, InventoryRecord)
		
		// Left Outer Join.
		// It has two output ports; First port to send the matched tuples and
		// the second port to send the unmatched tuples.
		(stream <orders> LeftOuterJoin1; 
		 stream <orders> LeftOuterJoin2)  = Join(OrderRecord; InventoryRecord) {
			window		
				OrderRecord: sliding, time(6); 
				InventoryRecord: sliding, time(6);
			
			param
				algorithm: leftOuter;
				match: product == productName;
		} // End of (... LeftOuterJoin2) = Join(OrderRecord, InventoryRecord)		
		
		// Right Outer Join. It has only one output port.
		stream <orders, inventory, tuple<uint32 quantity2>> RightOuterJoin1 = Join(OrderRecord; InventoryRecord) {
			window		
				OrderRecord: sliding, count(5); 
				InventoryRecord: sliding, count(5);
			
			param
				algorithm: rightOuter;
				match: product == productName;
				defaultTupleLHS: {orderId=0u, customerName="XXXXX", orderDate="XXXXX", product="XXXXX", quantity=0u};
			
			output
				RightOuterJoin1: 
					quantity = OrderRecord.quantity,
					quantity2 = InventoryRecord.quantity;
		} // End of RightOuterJoin1 = Join(OrderRecord, InventoryRecord)

		// Outer Join. It has three output ports.
		(stream <orders, inventory, tuple<uint32 quantity2>> OuterJoin1;
		 stream <orders> OuterJoin2;
		 stream <inventory> OuterJoin3) = Join(OrderRecord; InventoryRecord) {
			window		
				OrderRecord: sliding, time(6); 
				InventoryRecord: sliding, time(6);
			
			param
				algorithm: outer;
				match: product == productName;
			
			output
				OuterJoin1: 
					quantity = OrderRecord.quantity,
					quantity2 = InventoryRecord.quantity;
		} // End of (... OuterJoin3) = Join(OrderRecord, InventoryRecord)
	
		() as ScreenWriter1 = Custom(InnerJoin1) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple InnerJoin1: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\na)Tuples joined during Inner Join with sliding count(5)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "a)" + (rstring) InnerJoin1);
				} // End of onTuple InnerJoin1
		} // End of Custom(InnerJoin1)
		
		() as ScreenWriter2 = Custom(InnerEquiJoin1) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple InnerEquiJoin1: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\nb)Tuples joined during Inner Equi Join with sliding delta(orderId, 1u)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "b)" + (rstring) InnerEquiJoin1);
				} // End of onTuple InnerJoin1
		} // End of Custom(InnerJoin1)		

		() as ScreenWriter3 = Custom(LeftOuterJoin1) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple LeftOuterJoin1: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\nc)Tuples joined during Left Outer Join with sliding time(6)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "c)" + (rstring) LeftOuterJoin1);
				} // End of onTuple LeftOuterJoin1
		} // End of Custom(LeftOuterJoin1)		

		() as ScreenWriter4 = Custom(LeftOuterJoin2) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple LeftOuterJoin2: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\nd)Tuples failed to match during Left Outer Join with sliding time(6)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "d)" + (rstring) LeftOuterJoin2);
				} // End of onTuple LeftOuterJoin2
		} // End of Custom(LeftOuterJoin2)

		() as ScreenWriter5 = Custom(RightOuterJoin1) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple RightOuterJoin1: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\ne)Tuples matched and unmatched during Right Outer Join with sliding count(5)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "e)" + (rstring) RightOuterJoin1);
				} // End of onTuple RightOuterJoin1		
		} // End of Custom(RightOuterJoin1)	

		() as ScreenWriter6 = Custom(OuterJoin1) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple OuterJoin1: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\nf)Tuples matched during Outer Join with sliding time(6)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "f)" + (rstring) OuterJoin1);
				} // End of onTuple OuterJoin1
		} // End of Custom(OuterJoin1)

		() as ScreenWriter7 = Custom(OuterJoin2) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple OuterJoin2: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\ng)Tuples unmatched on left during Outer Join with sliding time(6)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "g)" + (rstring) OuterJoin2);
				} // End of onTuple OuterJoin2
		} // End of Custom(OuterJoin2)

		() as ScreenWriter8 = Custom(OuterJoin3) {
			logic
				state: 
					mutable int32 joinedTupleCnt = 0;
				
				onTuple OuterJoin3: {
					if (joinedTupleCnt++ == 0) {
						printStringLn("\nh)Tuples unmatched on right during Outer Join with sliding time(6)");
					} // End of if (joinedTupleCnt++ == 0)
					
					printStringLn ((rstring) joinedTupleCnt + "h)" + (rstring) OuterJoin3);
				} // End of onTuple OuterJoin3
		} // End of Custom(OuterJoin3)
} // End of composite Main.

~~~~~~
