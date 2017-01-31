This sample is used in the application dashboard tutorial (https://developer.ibm.com/streamsdev/docs/getting-started-with-application-dashboards/)

The sample is a slight modification of the Vwap sample and consist of the following three applications:
<ul>
<li>TradeSource: Periodically exports trade quotes from a file.
<li>IndexCalculator-{stock}: Imports trade quotes from the TradeSource application and calculates the bargain index for a particular stock. The ingest operators in this job have a tag ‘ingest’.
<li>IndexAnalyzer: Imports bargain indexes from the IndexCalculator-{stock} applications and calculates the average bargain index for all stocks. The operator which calculates the averages has an annotation which defines a view
</ul>
These application do not have any toolkit dependencies. The IndexCalculator application requires a submission time value for 'ticker'. Examples of valid values are IBM, EVG and BK. 
