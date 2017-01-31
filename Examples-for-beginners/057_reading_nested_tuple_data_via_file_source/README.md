~~~~~~
/*
This example shows how one can ingest nested tuple data from an input data file using the
SPL FileSource operator. As it can be seen below, our main tuple type PreposData contains
other tuple types and they in turn use nested types including SPL collection types such as
lists and maps. This example also includes a CSV file with a single row that shows the
correct tuple input data values.

Please refer to the data/prepos_rps_in.csv file in this project directory to view how
the actual tuple input data is specified to comply with the nested tuple structure defined below.
*/
namespace com.acme.test;

composite Test1 {
	type 
		PreposData = tuple <
			PreposInitData    initdata,
			PreposRunData     rundata,
			boolean           run,
			DummyTypeStream   dummytypestream
		>;

		PreposInitData = tuple <
			rstring sInitDataFile
		>;

		DummyTypeStream = tuple <
			CrewShift                     tCrewShift,
			CrewShiftPr                   tCrewShiftPr,
			DamagePredSimpl               tDamagePredSimpl,
			DamagePred                    tDamagePred
		>;

		PreposRunData = tuple <
			uint32                                             iHorizon,
			float64                                            fCriticalRestrPt,
			uint32                                             iMaxPrimCasesPerCct,
			uint32                                             iEscalationLevel,
			uint64                                             iStartTimeOffset,
			map<rstring, map<rstring, list<CrewShift>>>        ltCrewShiftsRegional,
			map<rstring, map<rstring, list<CrewShift>>>        ltCrewShiftsContract,
			map<rstring, map<rstring, list<CrewShift>>>        ltCrewShiftsForeign,
			map<rstring, map<rstring, list<CrewShiftPr>>>      ltCrewShiftsPrRegional,
			map<rstring, map<rstring, list<CrewShiftPr>>>      ltCrewShiftsPrContract,
			map<rstring, map<rstring, list<CrewShiftPr>>>      ltCrewShiftsPrForeign,
			map<rstring, list<DamagePredSimpl>>                ltDamagesSimpl,
			map<rstring, list<DamagePred>>                     ltDamages
		>;

		CrewShift = tuple <
			uint64                        iStart,       // shift start time
			uint64                        iEnd,         // shift end time
			uint32                        iNCrew        // number of crew members available during this shift
		>;

		CrewShiftPr = tuple <
			CrewShift                     tCrewShift,   // a specific crew shift
			uint32                        iPeriodicity, // periodicity, e.g., every 24 hours
			uint32                        iNReps        // number of repetitions, e.g., repeat 5 times
		>;

		DamagePredSimpl = tuple <
			uint64                        iStart,       // start time of prediction slot                  
			uint64                        iEnd,         // end time of prediction slot
			map<rstring, uint32>          iNDamages     // number of expected damages per damage type
		>;

		DamagePred = tuple <
			uint64                        iStart,       // start time of prediction slot                  
			uint64                        iEnd,         // end time of prediction slot
			map<rstring, list<uint32> >   iNCustomers,  // number of expected customers affected per damage per damage type
			map<rstring, list<uint32> >   iUrgency      // urgency (due to critical customer, hospital, etc.) per damage per damage type
		>;

	graph
		// Read the input data from a file that complies with a moderately
		// complex tuple definition made of nested SPL types.
		stream<PreposData> MyData1 = FileSource() {
			param
				file: "prepos_rps_in.csv";
				format: csv;
		}
		
		// Display the complex tuple input data we ingested using the FileSource operator above.
		() as MySink1 = FileSink(MyData1) {
			param
				file: "/dev/stderr";
		}
}

~~~~~~
