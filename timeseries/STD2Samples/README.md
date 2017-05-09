#STD2 Operator Samples

These samples demonstrate how the **STD2** operator can be used. The **STD2** operator is capable of decomposing a time series signal into _seasonal_, _trend_ and _residual_ components. This project contains the following samples:

  * **STD2Basic.spl** - This sample demonstrates a simple demonstration of how to use the **STD2** operator with a continuous time series.
  * **STD2FiniteLength.spl** - This samples demonstrates how to configure the operator in order to analyze finite-length time series (signal segments)
  * **STD2Random.spl** - This samples demonstrates how to generate random time series data and analyze it using the STD2 operator. The generated time series data contain _seasonal_, _trend_ and _residual_ components.
  * **STD2Anomaly.spl** - This samples demonstrates how to use the STD2 operator in conjunction with the **AnomalyDetector** operator in order to perform anomaly detection on seasonal data. More information about how this is done can be found here: [Detecting Anomalies in Seasonal Data](https://developer.ibm.com/streamsdev/2016/05/03/detecting-anomalies-in-seasonal-data/)

## Running the samples
To compile all of the applications, run this `ant` command:

`ant compileAll`


To compile an individual application, run one of the following `ant` commands:

 * `ant compileBasic`
 * `ant compileFiniteLength`
 * `ant compileRandom`
 * `ant compileAnomaly`

To clean the project, run this ant `ant` command:

`ant clean`
