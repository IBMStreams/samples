## Streaming Analytics Event Detection Starter Application - For use only with Streaming Analytics V2 service plans
Welcome to the Streaming Analytics service powered by IBM Streams! This starter application demonstrates how to configure and control the Streaming Analytics service through its V2 REST API. The application is written in Node.js using the SDK for Node.js runtime in Bluemix.

This sample should only be used with an instance created under a Streaming Analytics V2 service plan, because the Streams application included in this sample is compiled for RHEL/CentOS 7.x.  If you are using an instance created with a Streaming Analytics V1 service plan, please use [the V1 Event Detection sample](https://github.com/IBMStreams/samples/tree/main/QuickStart/EventDetection).

#### Preparing to run the starter application

To get ready to run the Event Detection V2 starter application, you need to:
1. Sign up for [IBM Cloud](https://ace.ng.bluemix.net/) and log in.

- [Download and install the IBM Cloud command-line tool](https://console.bluemix.net/docs/cli/reference/bluemix_cli/get_started.html).

- Create an application in IBM Cloud using the **SDK for Node.js** runtime. Remember the name you give your application, you will need it later on. 

- Connect your Node.js application to an instance of the Streaming Analytics service (that you created using one of the V2 service plans).


#### Pushing the Event Detection V2 starter application to IBM Cloud

After you meet these prerequisites, you are ready to download and push the starter application to IBM Cloud:

1. Download the [EventDetectionV2.zip](https://streams-github-samples.mybluemix.net/?get=QuickStart%2FEventDetectionV2) file.

- After the download completes, extract the .zip file.

- Rename the root directory of the extracted zip to match the name you gave your Node.js application in IBM Cloud.
		
- On the command line, `cd` to the renamed directory. For example:
		cd myapp
		
- Connect to IBM Cloud:

		bx api https://api.ng.bluemix.net

- Log into IBM Cloud:

		bx login

- Set your target org and space to match where you created your Node.js application.

		bx target -o myorg -s myspace

- Deploy your app. For example:

		bx cf push myapp

- Access your Node.js application by clicking on the route displayed near the top of your application's overview page in IBM Cloud.


