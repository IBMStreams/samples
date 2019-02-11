## Streams flows Aggregation Examples

This application is discussed in [this article on Streamsdev](https://developer.ibm.com/streamsdev/2018/01/12/calculate-moving-averages-real-time-data-streams-designer/).  It demonstrates how you can use the `Aggregation` operator in Streams flows. Output of the aggregation is sent to Cloud object storage, and sample result files are in the `output` folder in this project.

To get started, first download `Aggregation_examples.stp`, then import it into Streams flows.

# How to import the flow

- From Watson Data Platform, click **Tools** > **Streams flows**
  - If you don't already have a project, create one, then under Streams flow, Click **New Streams flow**.
- From the New Streams Flow page, Click **From file** and then select the flow you just downloaded.
- Click **Create**.
- After the flow is created, you need to configure it to send the result files to your Cloud Object Storage service:
  - Click **Edit**
  - For each `Cloud Object Storage` operator, edit it to specify the connection to the Cloud Object Storage service (you must have created one before importing the flow), and the file path.
- Run the flow by clicking "Run".

# Viewing Output
Once the flow is running, you can view the result files:
- Go to the [IBM Cloud dashboard](https://console.bluemix.net/dashboard/apps)
- Under **Services** click your *Cloud Object Storage* service. The name might be anything you chose at creation time, but the **Service Offering** will be *Cloud Object Storage*.
- Look for files `TotalSales.csv`, `customers_per_hr.csv`, e.t.c in the bucket you specified when running the flow.
