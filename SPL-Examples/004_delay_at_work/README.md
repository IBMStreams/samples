~~~~~~
/*
This example shows how a Delay standard toolkit operator can be
used to delay a stream. This example also introduces the
Custom operator that can be used to perform custom logic.
You can also notice the use of a state variable that is mutable
inside the Custom operator. It also shows how to create a 
new tuple on the fly and do your own submissions onto the output ports.
*/
namespace sample;

composite delay_at_work {
	type
		VoteCastedSchema = tuple<timestamp ts1, timestamp ts2, rstring voterName, rstring streetName, rstring townName, uint32 partyId>;

	graph
		stream <VoteCastedSchema> VoteCasted = Beacon() {
			param
				iterations: 20u;
			output
				VoteCasted: ts1 = getTimestamp();
		} // End of Beacon.

		stream <VoteCastedSchema> DelayedVoteCasted = Delay(VoteCasted) {
		param
			delay: 1.5;
		} // End of DelayedVoteCasted = Delay(VoteCasted)

		stream <VoteCastedSchema> CustomizedVoteCasted = Custom(DelayedVoteCasted as newVoteCasted) {
			logic	
				state: mutable uint32 voteCnt = 0;

				onTuple newVoteCasted: {
					voteCnt++;  
					VoteCastedSchema outTuple = {ts1 = newVoteCasted.ts1, ts2 = getTimestamp(), voterName = "Voter" + (rstring)voteCnt,
					streetName = "Street" + (rstring)voteCnt, townName = "Town" + (rstring)voteCnt,
					partyId = ((voteCnt%(uint32)2) == (uint32)0) ? (uint32)1 : (uint32)2};
					submit(outTuple, CustomizedVoteCasted);
				} // End of onTuple newVoteCasted.
		} // End of Custom(DelayedVoteCasted).

		() as FileWriter = FileSink(CustomizedVoteCasted) {
			param
				file: "MyResults.txt";
		} // End of FileWriter = FileSink(CustomizedVoteCasted)
} // End of composite delay_at_work.

~~~~~~
