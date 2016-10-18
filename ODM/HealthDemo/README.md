This sample provides an application that demonstrates the use of ODM rules for managing patient vital statistics. It is the sample that is used in the StreamsDev article located at: https://developer.ibm.com/streamsdev/docs/rules-based-processing-in-real-time-streaming-applications

The sample consists of 3 projects:

1) health-xom: This project contains the Java model that is used in the ODM rules. The Patient class within this project represents a patient and their vital stats.

2) HealthRules: This project contains the ODM rules that monitor the health of the patient and issue alerts when the stats are outside the "normal" range.

3) HealthApplication: This SPL project contains the main SPL application that uses the rules written in the HealthRules project against input patient data flowing through the system as tuples.

To see this sample in action, compile and submit the main application to a streams instance.
 
To compile the application, you need:
- Streams 4.2 - for the new com.ibm.streams.rulescompiler toolkit
- Streams Studio: For editing the rules located in the HealthRules project.
  
