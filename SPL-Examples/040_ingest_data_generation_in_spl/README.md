~~~~~~
/*
This example shows how SPL provides rich support to generate synthetic data required for
large scale testing. Many real-life applications in the Telco and the Retail Banking
sectors consume large amounts of daily transaction data through CSV formatted text files.
There could be huge amounts of CDR data from several telecom circles or daily transaction
data for millions of accounts in a retail bank.

While building and testing SPL applications, it will become necessary to generate such
ingest data files with fake data that is somewhat realistic. This application shows
how such large amounts of data in several thousands of files can be created very quickly
using the SPL standard toolkit operators and the SPL built-in functions (file IO and math random).
*/
namespace my.sample;
// Define an use directive with the namespace for the C++ native function(s) used 
// in this application. 
use test.datagen::*;
// By defining this, we can use all of the SPL built-in file functions.
use spl.file::*;

composite Main {
	// Main operator can't receive any operator parameters.
	// However, we can define all the constants needed for our 
	// application as expression parameters.
	// CAUTION:
	// The default settings below should be fine for testing the function of this application.
	//
	// If not carefully configured, large values for these constants will result in
	// the creation of large number of files with unmanagebale amounts of data.
	// If you are going to change these values, be very careful so as
	// not to fill up your disk space quota by configuring it wrongly.
	// 
	param
		// Have the number_of_months_data_needed value to anything from 3 to 10.
		expression <uint32>			$UINT32_NUMBER_OF_MONTHS_DATA_NEEDED : 2u;
		expression <uint32>			$UINT32_TOTAL_MONTHS_IN_A_YEAR : 12u;
		expression <list <rstring>>	$LIST_RSTRING_MONTHS : ["", "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
										      				"JUL", "AUG", "SEP", "OCT", "NOV", "DEC"];
		expression <rstring>		$RSTRING_YEAR_TO_BE_USED : "2015";
		expression <uint32>			$UINT32_NUMBER_OF_DAYS_IN_A_MONTH : 30u;
		// As it is configured here, we will create 60 test ingest data files with 
		// 5 CSV records in each file.
		// 2 months time 30 days will give us a total of 60 files.
		expression <uint32>			$UINT32_MAX_TRANSACTIONS_NEEDED_PER_DAY : 5u; 	
		expression <uint32>			$UINT32_FIRST_ACCOUNT_ID : 1000000u;
		expression <uint32>			$UINT32_NUMBER_OF_ACCOUNT_IDS_NEEDED : 500000u;
		expression <rstring>		$RSTRING_FILE_MODE : "w+";
		expression <rstring>		$RSTRING_TRANSACTION_FILE_EXTENSION : ".transactions";
		// In Streams 4.x, it is necessary to get the data directory set either by the developer at the
		// compile time or by the user at the application submission time.
		expression <rstring>		$RSTRING_TRANSACTION_FILE_STAGING_DIR : dataDirectory() + "/";
		expression <rstring>		$RSTRING_TRANSACTION_FILE_HEADER : "# tuple<uint32 accountId, rstring activityDate, " + 
																	    "uint32 transactionCode, uint32 branchId, uint32 activityType, " +
																	    "float32 amount, rstring checkNumber>\n";
																	    
	type
		// We will not use this type anywhere in this program.
		// It is here for reference. Typically, we have to generate a lot of random test data
		// matching this tuple's structure. 
		accountActivity = tuple<uint32 accountId, rstring activityDate, uint32 transactionCode, 
			uint32 branchId, uint32 activityType, float32 amount, rstring checkNumber>;
	
		dummyType = tuple<int32 ignore>;
	
	graph
		// Let us create the deposit/debit transactions data.
		// In this Beacon, we will simply send a single dummy tuple so that
		// that will act as a signal to the downstream operator to go ahead
		// and generate the required amount of test data and store them in files.
		stream <dummyType> AccountActivityRecord = Beacon () {
			param
				initDelay: 1f;
				iterations: 1u;
		} // End of AccountActivityRecord = Beacon ()
		
		// Let us customize the data now for the deposit/debit transaction files.
		// We will stay in a loop inside of this custom operator's onTuple section and
		// create as many CSV records needed in as many days worth of 
		// ingest data files are required.
		() as AccountActivityFileWriter = Custom(AccountActivityRecord) {
			logic
				state: {
					mutable uint32 tupleCnt = 0u;
					mutable uint32 accountId = 0u;
					mutable uint32 activityDay = 0u;
					mutable uint32 monthIndex = 3u;
					list<rstring> month = $LIST_RSTRING_MONTHS;
					rstring year = $RSTRING_YEAR_TO_BE_USED;
					mutable uint32 transactionCode = 0u;
					mutable uint32 branchId = 0;
					mutable uint32 activityType = 0u;
					mutable float32 amount = 0f;
					mutable rstring checkNumber = "";
					mutable rstring tupleLine;
					mutable boolean createFileForNewDay = false;
					mutable rstring fileName = "";
					mutable int32 fileError = 0;
					mutable uint64 fileHandle = 0ul;
					rstring fileMode = $RSTRING_FILE_MODE;
					mutable uint32 totalFilesCreated = 0u;
				} // End of state.
				
				// Beacon operator above is configured to send us only one tuple.
				// As soon as that single tuple arrives, we will stay in a loop until
				// all the ingest data files are created with the required amount of 
				// CSV records.
				onTuple AccountActivityRecord: {
					uint32 totalCsvRecordsToBeCreated = ($UINT32_MAX_TRANSACTIONS_NEEDED_PER_DAY * 
						$UINT32_NUMBER_OF_MONTHS_DATA_NEEDED *
						$UINT32_NUMBER_OF_DAYS_IN_A_MONTH);
						
					uint32 totalFilesToBeCreated = $UINT32_NUMBER_OF_MONTHS_DATA_NEEDED *
						$UINT32_NUMBER_OF_DAYS_IN_A_MONTH;
						
					printStringLn(ctime(getTimestamp()) + "    Attempting to create a total of " +
						(rstring) totalCsvRecordsToBeCreated + " CSV records in " + 
						(rstring) totalFilesToBeCreated + " daily transaction files...");
					
					// Stay in a loop and finish creating the required number of input files with the account activity tuples in CSV format.
					while (tupleCnt < totalCsvRecordsToBeCreated) {
						// Go with the assumption that no new file needs to be created.
						// If needed, some other logic below will take care of setting this flag.
						createFileForNewDay = false;
						
						// Keep incrementing the account ids for successive CSV lines.					
						if (accountId++ > $UINT32_NUMBER_OF_ACCOUNT_IDS_NEEDED) {
							accountId = 0u; 
						} // End of if (accountId > $UINT32_NUMBER_OF_ACCOUNT_IDS_NEEDED)
												
						tupleLine = (rstring) ($UINT32_FIRST_ACCOUNT_ID + accountId) + ",";
						
						// Create the required number of records for one day and then move on to the next day of the month.
						if ((tupleCnt % $UINT32_MAX_TRANSACTIONS_NEEDED_PER_DAY) == 0u) {
							++activityDay;
							// We just switched to a different day. We need to create a new file.
							createFileForNewDay = true;
						} // End of if ((++tupleCnt % $UINT32_MAX_TRANSACTIONS_NEEDED_PER_DAY) == 0u)
						
						if (activityDay > $UINT32_NUMBER_OF_DAYS_IN_A_MONTH) {
							activityDay = 1u;
							// We just switched to a different day. We need to create a new file.
							createFileForNewDay = true;
							
							// Adjust the month index.
							if (++monthIndex > $UINT32_TOTAL_MONTHS_IN_A_YEAR) {
								// We will always start from March. (Skip Jan and Feb)
								monthIndex = 3u; 
							} // End of if (++monthIndex > $UINT32_TOTAL_MONTHS_IN_A_YEAR)
						} // End of if (++activityDay > $UINT32_NUMBER_OF_DAYS_IN_A_MONTH)
								
						tupleLine += (rstring) activityDay + "-" + month[monthIndex] + "-" + year + ",";
						tupleLine += (rstring) (++transactionCode) + ",";
						tupleLine += (rstring) (++branchId) + ",";
						activityType = (random() * 1000f > 600f) ? 1u : 2u;
						tupleLine += (rstring) activityType + ",";
						
						// An example of the return value from random() = 0.107221318874508 
						mutable float64 randomValue1 = random();
						mutable float64 randomValue2 = random();
						
						if (randomValue1 * 100f > 50f) {
							// This random asignment may give us an amount > $1000 now and then.
							randomValue2 = (randomValue2 * 1000f) + (randomValue2 * 1000f);
						} else {
							// This will guarantee us an amount less than $100.
							randomValue2 *= 100f;
						} // End of if (randomValue1 * 100f > 50f)
						
						amount = (float32) randomValue2;
						tupleLine += (rstring) amount + ",";
						checkNumber = (rstring) ((uint32)(randomValue1 * 10000f));
						tupleLine += checkNumber + "\n";

						// If we need to create a new file, do that here.
						if (createFileForNewDay == true) {
							createFileForNewDay = false;
							
							// If we have already been processing a file, let us close that file now.
							// This is true for except for the very first tuple that we are yet to create.
							if ((tupleCnt > 0u) && (fileHandle != 0ul)) {
								// Close the file.
								fclose (fileHandle, fileError);
								fileHandle = 0ul;
							} // End of if ((tupleCnt > 0u) && ...) 
							
							fileName = $RSTRING_TRANSACTION_FILE_STAGING_DIR;
							// File name format: MM-DD-YYYY.transactions
							// sprintf function below is not part of the SPL built-in function set.
							// Instead, we are using a C++ native function that is part of this project.
							// Look inside the test.datagen/native.function/function.xml file to
							// learn about the model of this native function.
							// For the actual file containing the C++ code of this native function,
							// you have to refer to the impl/include directory.
							fileName += sprintf((uint64)monthIndex, (uint16)2u) + "-";
							fileName += sprintf((uint64)activityDay, (uint16)2u) + "-" + year;
							fileName += $RSTRING_TRANSACTION_FILE_EXTENSION;

							totalFilesCreated++;
							
							// We are going to use the SPL built-in file functions to open/write/close.
							fileHandle = fopen(fileName, $RSTRING_FILE_MODE, fileError);
							
							if (fileError != 0) {
								printStringLn ("Error while creating the file " + fileName + 
									". (Error: " + (rstring) fileError + ") Giving up.\n");
								break;
							} // End of if (fileError != 0)
							
							// Write the header line at the very top of the CSV file.
							fwriteString($RSTRING_TRANSACTION_FILE_HEADER, fileHandle, fileError);
						} // End of if (createFileForNewDay == true)

						// Let us write this tuple to the file.						
						fwriteString (tupleLine, fileHandle, fileError);
						
						if (fileError != 0) {
							printStringLn ("Error while writing to the file " + fileName + 
								". (Error: " + (rstring) fileError + ") Giving up.\n");
							break;
						} // End of if (fileError != 0)
						
						// Move to the next tuple.
						++tupleCnt;
					} // End of while (tupleCnt < ...)
					
					// Close the very last file we created.
					fclose(fileHandle, fileError);
					printStringLn(ctime(getTimestamp()) + "    Finished creating " + 
						(rstring) totalFilesCreated + " daily transaction files.");
				} // End of onTuple AccountActivityRecord
		} // End of AccountActivityFileWriter = Custom(AccountActivityRecord)	
} // End of composite Main
~~~~~~
