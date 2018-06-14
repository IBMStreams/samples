# LibertyStreamsProxy Servlet - readme

Dynamically build a bridge between the open Web and a Streams application running in IBM Cloud.
[This article provides more background information](https://developer.ibm.com/streamsdev/2018/04/01/connecting-internet-streams-ibm-cloud/).
The servlet, running on the IBM Cloud *Liberty for Java Foundry App*, locates the connected Streams *Streaming Analytics* server and
binds it Liberty's web address via an embedded proxy. The proxy configuration occurs on the first access from the web.

The servlet works in conjunction with Streams' [INET](http://ibmstreams.github.io/streamsx.inet/) toolkit, transparently.
Streams applications that are accessed via HTTP or REST in a development environment with no changes to the proxy server.

The goal of this servlet, to easily move a Streams applications to the web, for demonstration purposes.


## Demonstration walk through.

Assumed:
 * You have a [Bluemix account](https://console.ng.bluemix.net/registration/).
 * You have downloaded and installed [cf](https://github.com/cloudfoundry/cli#downloads) and [Bluemix](https://console.bluemix.net/docs/starters/install_cli.html) command-line tools.

Provided:
 * UpperRestRH6.sab : The Streams application bundle file, with the demonstration Streams application, source below.
 * A [video](https://youtu.be/v0upqnUWz74) walking through the process outlined here.


Overview of Steps.

1. Allocate and attach resources in Bluemix.
2. Start the Streams application.
3. Build, install, start  the Liberty application.
4. Access the Streams application running in Bluemix.


### The Streams application.

Below is the source of the Streams application used in the walk through. The code responds with a upper case version of the input string.

```
namespace application ;

use com.ibm.streamsx.inet.rest::HTTPRequestProcess ;
use com.ibm.streamsx.inet.http::HTTPRequest ;

composite UpperRest
{
	graph
		(stream<HTTPRequest> httpRequest) as rest =
			HTTPRequestProcess(httpResponse)
		{
			param
				context : "myStreams" ;
				contextResourceBase : "/dev/null" ;
				port : 8080 ;
		}

		(stream<HTTPRequest> httpResponse as O) as doWork = Functor(httpRequest as I)
		{
			output
				O : response = upper(I.request) ;
		}
}
```
Invoking the URL, "http://localhost:8080/myStreams/rest/ports/analyze/0?to_upper_case" in your Streams development environment looks like...

```
$ curl http://localhost:8080/myStreams/rest/ports/analyze/0?to_upper_case
TO_UPPER_CASE
```

The application uses the **HTTPRequestProcess()** operator of the [INET](http://ibmstreams.github.io/streamsx.inet/) toolkit.


A built version of the program, UpperRestRH6.sab is included in this repository. 

Login into your IBM Cloud account (https://console.bluemix.net/catalog)

### Create Streams Service
1. Enter the 'Data & Analytics' catagory section. (https://console.bluemix.net/catalog/?category=data)
2. Select 'Streaming Analytics'.
3. Set the the 'Service Name', I'll use 'LibertyRiverAnalytics' and create.
4. The service will start.

### Create a Liberty for Java Cloud Foundry App on IBM Cloud.
1. Enter the 'Cloud Foundry Apps' category section (https://console.bluemix.net/catalog/?category=cf-apps)
2. Select 'Liberty for Java' and create.
3. Provide a 'App name:', for this walkthorough I'll use 'LibertyRiver' and create.

### Connect Liberty App to Streams service.
1. Select 'Connections' in the left panel.
2. Select 'Create connecctions +' button.
3. Locate the Streams Analytics service you created above (LiberyRiverAnalytics), select the row and select the 'Connect' button when it appears.
4. Select 'Restage' when prompted.

### Upload the Streams application.
1. When the restaging completes.
2. In the 'CONNECTION NAME' column select the Streaming Analytics service (LibertyRiverAnalytics).
3. Select the 'Alias of' link, it will be the name of the service (LibertyRiverAnalytics).
4. Select 'LAUNCH'.
5. Select the "Play" button to the right of 'Jobs'.
6. Browse to the location of of the Streams application bundle file (UpperRestRH6.sab)
7. Select 'Submit'.

Liberty is connected to a 'Streams Analytics' service which is running UpperRest application. Now
we need to build an install the proxy server on Liberty.

### Build and upload the LibertyStreamsProxy server.

From a terminal, login to your bluemix account and set your target. Using my account, it looks like...

```bash
bluemix login  -o siegenth@us.ibm.com -sso
bluemix target -o siegenth@us.ibm.com -s dev
````
When prompted, follow directions for password.

Change to the General/LibertyStreamsProxy directory of the downloaded git repostitory and invoke maven to install componets and build the proxy server, output will be in 'target' directory.

```bash
cd General/LibertyStreamsProxy
mvn -U initialize
mvn clean install
```

Upload the war file to IBM Cloud, using *Liberty App* name (LibertyRiver) and the war file (LibertyStreamsProxy-v1.0.war).
```bash
bluemix cf push LibertyRiver -p target/LibertyStreamsProxy-v1.0.war
```
On completion the application is ready to be accessed.

### Test the application .
Build build and invoke the tests.

```bash
 mvn clean test -DappUrl=LibertyRiver.mybluemix.net -DskipTests=false
```
Execute the application using curl, swap in your *Liberty App* name before executing
```bash
curl http://libertyriver.mybluemix.net/myStreams/rest/ports/analyze/0?upper_case
```
The execution looks like...

```bash
curl http://libertyriver.mybluemix.net/myStreams/rest/ports/analyze/0?upper_case
UPPER_CASE
```

Shut down the Liberty application ...

```bash
bluemix cf stop LibertyRiver
```
## Appendix : Notes :

### Precaution
Leaving the proxy up may result in excessive charges being incurred by your application if internet 'trolls' discovery your proxy's url. Shutting down the proxy server is advisable.

### Debug
The process of deriving the proxy mapping is a multistep process, a log of
this processing can be retrieved using the following urls.

The current active log:
```www
http://libertyriver.mybluemix.net/proxyDebug
```

Rederive and fetch the log (good for debugging)...
```web
http://libertyriver.mybluemix.net/TopoDisplay
```

### Resources

[Video](https://youtu.be/v0upqnUWz74) walkthrough.
