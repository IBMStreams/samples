/*jshint node:true*/

// **************************************
// * (C) Copyright 2018   IBM Corp.     *
// * All Rights Reserved.               *
// **************************************
//
// app.js
// This file contains the server side JavaScript code for the Event Detection V2 sample application.
// This starter application uses express as its web application framework and jade as its template engine.
var https = require('https');
var express = require('express');
var async = require('async');
var fs = require('fs');
var FormData = require('form-data');
var errorhandler = require('errorhandler');
var url = require('url');
var querystring = require('querystring');   

// Setup middleware
var app = express();
app.use(errorhandler());
app.use(express.static(__dirname + '/public')); //setup static public directory
app.set('view engine', 'jade');
app.set('views', __dirname + '/views'); //optional since express defaults to CWD/views
app.use(require('body-parser').json());

// Array used to reflect status of the 6 majors steps of this app on its web UI.
var status_step = ["Pending", "Pending", "Pending", "Pending", "Pending", "Pending"];

// Render index page of the app's single page web UI
app.get('/', function(req, res){
    res.render('index', {'events':events, 'maxmin':maxmin, 'status':status_step, 'eventTarget':eventTarget});
});

// POST handler for the events being sent back from the Streams application
app.post('/', function(req, res){
    status_step[4] = "Processing Events";

    if (!cancelling) {
      console.info("In POST function");
      console.log(req.body);
      var payload = req.body;
 
      if (payload.eventType == 'MaxMin Temp') {
        // Max or min temperature change
        maxmin = payload;
      }
      else {
        // Regular event
        eventCount++;
        console.info("Event total = " + eventCount);

        // Add event to the array used by the web user interface
        events.push(new Event(eventCount, payload));
     
        // Cancel the Streams job if we've reached the event target
        if (eventCount == eventTarget) {
          cancelling = true;
          console.info("EVENT TARGET REACHED...");
          console.info("STREAMS JOB WILL BE CANCELLED.");
          finalCancel();
        }
      }
    }
    res.send();
});
// Post handler - end


// Start the app using some of the values from process.env 
// application
var host = "0.0.0.0";
var port = (process.env.PORT || 3000);

// Start server
app.listen(port, host);
console.info('App started on port ' + port + '\n');

// Get the route for this application so it can be passed as a parameter to the Streams application.
var vcap_app = JSON.parse(process.env.VCAP_APPLICATION);
var app_uri = 'http://' + vcap_app.application_uris[0];
console.info('App URI is: ' + app_uri);

//
// Step 1.a - Extract the environment information required to use the Streaming
// Analytics REST API.
//

// VCAP_SERVICES contains all the credentials of services bound to this application.

// Declare variables for portions of the VCAP_SERVICES we will save and use.
var sa_props = {};
var service_name = {};

// Parse VCAP_SERVICES
if (process.env.VCAP_SERVICES) {
    var env = JSON.parse(process.env.VCAP_SERVICES);
	
    // For reference, echo names and values for each bound service to the log
    for (var svcName in env) 	{
        console.info(svcName);
    }
    console.info(env);

    // Find the Streaming Analytics service
    if (env['streaming-analytics']) {
        sa_props = env['streaming-analytics'][0]['credentials'];
        console.info('streaming-analytics credentials: ');
        console.info(sa_props);
        service_name = env['streaming-analytics'][0]['name'];
        status_step[0] = "Completed";
    }
    else {
        console.info('You must bind the streaming-analytics service to this application');
        status_step[0] = "Failed - Application must be bound to streaming-analytics service.";
        process.exit(1);
    }
}
else {
    process.exit(1);
}

// Define variables that will be used in the remaining steps

var btok;					// IAM bearer token
var restURL = url.parse(sa_props.v2_rest_url);	// Base URL for V2 REST API

// Authorization information for all REST calls
var authbuf; 
var mykey = sa_props.apikey;

// Event variables
var events = [];
var eventCount = 0;
var eventTarget = 1500;
eventTarget = eventTarget.MAX_VALUE;

var cancelling = false;
var Event = function(number, evt) {  
    this.Number = number;  
    this.EventString = evt;  
};

// Temperature max/min object
var maxmin = {};

// Streams-related variables
var instanceStatus = {};
var jobNumber = -1;
var jobExists = false;

// The next steps are run inside an async series.  Since Node.js is
// asynchronous by default, the series is needed to keep the steps from
// running at the same time.  The series also makes the execution of the
// app easier to follow on its web UI.

async.series(
  [

    //
    // Step 1.b - IAM authentication step - get a bearer token to use in Streaming
    // Analytics REST calls
    // 

    function(callback) {
      //
      // Begin - POST get token
      //
    console.info("Preparing to call POST to get a bearer token");  

    var options = {
      "method": "POST",
      "hostname": "iam.bluemix.net",
      "port": "443",
      "path": "/oidc/token",
      "headers": {
        "accept": "application/json",
        "content-type": "application/x-www-form-urlencoded"
      }
    };

    postData = {
        grant_type: "urn:ibm:params:oauth:grant-type:apikey",
        apikey: mykey
    };

    postBody = querystring.stringify(postData);

    console.info('Options prepared:');
    console.info(options);

    var req = https.request(options, function (res) {
      var chunks = [];

      res.on("data", function (chunk) {
        chunks.push(chunk);
      });

      res.on("end", function () {
        console.info('Response code: ' + res.statusCode);
        var body = Buffer.concat(chunks);
        console.info('Response body: ' + body.toString());

        var d = body.toString();
        var jsonObj =JSON.parse(d);
        console.info("Token is: " + jsonObj.access_token);
        btok = jsonObj.access_token;
        authbuf = 'Bearer ' + btok; 

        // Make callback for async series after the http response
        if (res.statusCode == 200){
          // Status of 200 means ok, anything else means the status call failed
          callback(null, null);  
        }
        else {
          console.info("POST create token failed.");
          callback(res.statusCode, null);
        }        
      });
    });
    req.write(postBody);
    req.end();
    },
      
    //
    // Step 2.a - check if Streams instance is running
    // 

    function(callback) {
      //
      // Begin - GET status
      //
      status_step[1] = "Starting";
      console.info("\nUsing the Streaming Analytics REST API to get the instance status information.");
      console.info("Preparing to call Streaming Analytics GET API");  

      // prepare the header
      var getheaders = {
        'Content-Type' : 'application/json',
        'Authorization' : authbuf
      };

      // set the get options
      var optionsget = {
        host : restURL.host,
        path : restURL.pathname,
        method : 'GET',
        headers : getheaders
      }; 

      console.info('Options prepared:');
      console.info(optionsget);
      
      console.info('Perform the GET-status call');
      var reqGet = https.request(optionsget, function(res) {
     
        res.on('data', function(data) {
          console.info('GET-status response data:' + data);
          instanceStatus = JSON.parse(data);
          if (instanceStatus.state === 'STARTED') {
            status_step[1] = "Completed - Instance was already running";
          }
          else {
            status_step[1] = "Starting instance...";
          }
         
          // Make callback for async series after the http response
          if (res.statusCode == 200){
            // Status of 200 means ok, anything else means the status call failed
            callback(null, null);  
          }
          else {
            status_step[1] = "Failed";
            callback(res.statusCode, null);
          }        
        });
      });
 
      reqGet.end();
      reqGet.on('error', function(e) {
        console.error(e);
      });

      //
      // Get-status end
      //

    },


    //
    // Step 2.b - Start the instance if necessary
    // 

    function(callback) {
  
      if (instanceStatus.state === 'STARTED') {
        // Make callback for async series signaling completion
        callback(null, null);  
      }
      else {
        // Start the instance with a PATCH 
        console.info("Using the Streaming Analytics REST API to start the Streams instance.");

        // create the JSON object
        var jsonPatch = JSON.stringify({"state":"STARTED"});

        // prepare the header
        var patchheaders = {
          'Content-Type' : 'application/json',
          'Content-Length' : Buffer.byteLength(jsonPatch, 'utf8'),
          'Authorization' : authbuf
        };
 
        // set the put options
        var optionspatch = {
          host : restURL.host,
          path : restURL.pathname,
          method : 'PATCH',
          headers : patchheaders
        }; 

        console.info('Options prepared:');
        console.info(optionspatch);

        console.info('Perform the PATCH');
        var reqPatch = https.request(optionspatch, function(res) {

          res.on('data', function(d) {
            console.info('PATCH-start result: ' + d);
            console.info('statusCode: ', res.statusCode + '\n\n');
            // Make callback for async series after the http response
            if (res.statusCode == 200) {
              status_step[1] = "Completed - Started Instance";
              callback(null, null);  
            }
            else {
              status_step[1] = "Failed - Could not start the instance";
              callback(res.statusCode, null);
            }
          });   
        });

        // write the json data
        reqPatch.write(jsonPatch);
        reqPatch.end();
        reqPatch.on('error', function(e) {
          console.error(e);
        });
      } // end else

    },


    //
    // Step 3.a - Check if the Streams job is running because of some earlier run   
    // of the sample
    //     

    function(callback) {
      // If the instance was not running, no jobs earlier jobs could exist
      if (instanceStatus.state != 'STARTED') {
        status_step[2] = "Completed - No earlier job running";
        // Make callback for async series signaling completion
        callback(null, null);  
      }

      //
      // Begin - GET jobs
      //
      console.info("\nUsing the Streaming Analytics REST API to get jobs information.");
      console.info("Preparing to call Streaming Analytics GET Jobs API");  

      // prepare the header
      var getheaders = {
        'Content-Type' : 'application/json',
        'Authorization' : authbuf
      };

      // set the get options
      var optionsget = {
        host : restURL.host,
        path : restURL.pathname + '/jobs',
        method : 'GET',
        headers : getheaders
      }; 

      console.info('Options prepared:');
      console.info(optionsget);
      
      console.info('Perform the GET-jobs call');
      var reqGet = https.request(optionsget, function(res) {
     
        res.on('data', function(data) {
          console.info('GET-jobs response data:' + data);
          var jobInfo = JSON.parse(data);
          for (var j=0; j < instanceStatus.job_count; j++) {
            if (jobInfo.resources[j].name === 'EventDetectionSample') {
              jobExists = true;
              break;
            }
          }
          // Make callback for async series after the http response
          if (res.statusCode == 200){
            // Status of 200 means ok, anything else means the status call failed
            callback(null, null);  
          }
          else {
            status_step[2] = "Failed";
            callback(res.statusCode, null);
          }        
        });
      });
 
      reqGet.end();
      reqGet.on('error', function(e) {
        console.error(e);
      });

      //
      // Get-jobs end
      //
    },

    //
    // Step 3.b - Cancel the job if it already exists   
    //     

    function(callback) {
          if (!jobExists) {
            status_step[2] = "Completed - No earlier job running";
            // Make callback for async series signaling completion
            callback(null, null);  
          }
          else {
            // Cancel the job via a DELETE-Job 
            console.info("Using the Streaming Analytics REST API to cancel the Streams job.");

            // prepare the header
            var deleteheaders = {
              'Authorization' : authbuf
            };
 
            // set the put options
            var optionsdelete = {
              host : restURL.host,
              path : restURL.pathname + '/jobs/' + 'EventDetectionSample',
              method : 'DELETE',
              headers : deleteheaders
            }; 

            console.info('Options prepared:');
            console.info(optionsdelete);

            console.info('Perform the DELETE');
            var reqDelete = https.request(optionsdelete, function(res) {

              res.on('data', function(d) {
                console.info('DELETE-jobs result: ' + d);
                console.info('statusCode: ', res.statusCode + '\n\n');
                if (res.statusCode == 200) {
                  status_step[2] = "Completed - Existing job cancelled";
                  callback(null, null);  
                }
                else {
                  status_step[2] = "Failed - Could not cancel the Streams job";
                  callback(res.statusCode, null);
                }
              });   
            });

            reqDelete.end();
            reqDelete.on('error', function(e) {
              console.error(e);
            });
          }
    },

    //
    // Step 4 - submit the Streams application bundle
    // 

    function(callback) {
      //
      // Begin - POST submit
      //
      status_step[3] = "Starting";
      console.info("\nUsing the Streaming Analytics REST API to submit an application bundle.");
      console.info("Preparing to call submit API");     

      // -----New form-----
      var form = new FormData();

      // -----File part-----
      form.append('file', fs.createReadStream('EventDetection.sab'), {
        contentType: 'application/octet-stream'
      });

      // -----JSON Part-----
      jsonObject = JSON.stringify({
        "jobName" :  "EventDetectionSample",
        "submissionParameters" :
        {
            "route" : app_uri,
        },
      });
      console.info('JSON object part: ' + jsonObject);

      var buffer = new Buffer(jsonObject);
      form.append('my_buffer', buffer, {
        contentType: 'application/json',
        // The line below is not an actual file.  The name with the .json 
        // extension is needed for the data in the buffer to be recognized
        // as json.
        "filename": "jparams.json"
      });

      // -----SUBMIT POST-----
      var jsonPostRes = {};

      form.submit({ 
        protocol: 'https:',
        host : restURL.host,
        path : restURL.pathname + '/jobs',
        headers: {'Authorization' : authbuf}
      }, function(err, res) {

         res.on('data', function(data) {
           console.info('GET-status response data:' + data);
           jsonPostRes = JSON.parse(data);
           jobNumber = jsonPostRes.id;
           console.info('Streams job id is: ' + jobNumber);
         
           console.info('In form submit callback.');
           if (err) {
             console.info('err: ' + err);
             console.info('POST failed - Submission of the bundle failed.');
             status_step[3] = "Failed";
           }
           else {
             console.info('POST complete - Submission of the bundle complete.');
             status_step[3] = "Completed";
             status_step[4] = "Started";
           }
         });
      });
      
      console.info('After the POST');
      callback(null, null);
      

      //
      // Post-submit end
      //
    },

    //
    // Step 5 - Process events and display them on the 
    //          application's web page
    // 
    // (No code in async series for processing events.  See the
    //  POST handler elsewhere in this file for event handling.)

    //
    // Step 6 - Cancel the Streams job after the event target is reached.
    //
    // (No code in async series for cancelling the job.  See the
    //  POST handler elsewhere in this file that performs the cancel.)

      ],
        function(err, response) {
          // no-op
        }
    ); // End of async series



// Helper functions

function finalCancel() {
  // Another async series to 
  //    1. Get a new bearer token (if the app has run for more than an hour, then the
  //       token we have been using will have expired).
  //    2. Submit a job cancel request using the new token.
  //

  async.series(
    [
      function(callback) {
        //
        // Begin - POST get token
        //
        console.info("Preparing to call POST to get a new bearer token");  
        var options = {
          "method": "POST",
          "hostname": "iam.bluemix.net",
          "port": "443",
          "path": "/oidc/token",
          "headers": {
            "accept": "application/json",
            "content-type": "application/x-www-form-urlencoded"
          }
        };

        postData = {
            grant_type: "urn:ibm:params:oauth:grant-type:apikey",
            apikey: mykey
        };
        postBody = querystring.stringify(postData);

        console.info('Options prepared:');
        console.info(options);

        var req = https.request(options, function (res) {
          var chunks = [];

          res.on("data", function (chunk) {
            chunks.push(chunk);
          });

          res.on("end", function () {
            console.info('Response code: ' + res.statusCode);
            var body = Buffer.concat(chunks);
            console.info('Response body: ' + body.toString());

            var d = body.toString();
            var jsonObj =JSON.parse(d);
            console.info("Token is: " + jsonObj.access_token);
            btok = jsonObj.access_token;
            authbuf = 'Bearer ' + btok; 

            if (res.statusCode == 200) {
              // Status of 200 means ok, anything else means the request failed
              console.info("POST create token succeeded.");
              callback(null, null)
            }
            else {
              console.info("POST create token failed.");
              callback(res.statusCode, null);
            }        
          });
        });
        req.write(postBody);
        req.end();
      },

      function(callback) {
        // Cancel the job via a DELETE-Job 
        console.info("Using the Streaming Analytics REST API to cancel the Streams job.");

        // prepare the header
        var deleteheaders = {
          'Authorization' : authbuf
        };
 
        // set the put options
        var optionsdelete = {
          host : restURL.host,
          path : restURL.pathname + '/jobs/' + 'EventDetectionSample',
          method : 'DELETE',
          headers : deleteheaders
        }; 

        console.info('Options prepared:');
        console.info(optionsdelete);

        console.info('Perform the DELETE');
        var reqDelete = https.request(optionsdelete, function(res) {

          res.on('data', function(d) {
            console.info('DELETE-jobs result: ' + d);
            console.info('statusCode: ', res.statusCode + '\n\n');
            if (res.statusCode == 200) {
              console.info('Success - Job Cancelled');
              status_step[4] = "Completed";
              status_step[5] = "Completed";
              callback(null, null);
            }
            else {
              console.info('Failed - Job was not cancelled');
              status_step[5] = "Failed - Could not cancel the Streams job";
              callback(res.statusCode, null);
            }
          });   
        });

        reqDelete.end();
        reqDelete.on('error', function(e) {
          console.error(e);
        });
      },

    ],
      function(err, response) {
          // no-op
      }
    ); // End of async series


}


