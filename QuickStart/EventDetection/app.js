/*jshint node:true*/

// **************************************
// * (C) Copyright 2015   IBM Corp.     *
// * All Rights Reserved.               *
// **************************************
//
// app.js
// This file contains the server side JavaScript code for the Event Detection sample application.
// This starter application uses express as its web application framework and jade as its template engine.
var https = require('https');
var express = require('express');
var async = require('async');
var fs = require('fs');
var FormData = require('form-data');
var errorhandler = require('errorhandler');

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
      var ss = req.body.toString();
      console.info("POST object is: " + ss);
      var jsonString = req.body.jsonString;
      console.info("POST message is: " + jsonString);
      var payload = JSON.parse(jsonString);
    
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
          finalCancel(jobNumber.toString());
        }
      }
    }
    res.send();
});
// Post handler - end


// Start the app using some of the values from process.env, i.e. the IP address // and port of the Cloud Foundry DEA (Droplet Execution Agent) that hosts this 
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
// Step 1:  Extract the environment information required to use the Streaming
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

// Authorization information for all REST calls
var authbuf = 'Basic ' + new Buffer(sa_props.userid + ':' + sa_props.password).toString('base64');

// Event variables
var events = [];
var eventCount = 0;
var eventTarget = 3000;
var cancelling = false;
var Event = function(number, evt) {  
    this.Number = number;  
    this.EventString = evt;  
};

// Temperature max/min object
var maxmin = {};

// Streams variables
var instanceStatus = {};
var jobNumber = -1;
var oldJob = -1;


// Steps 2, 3 and 4 are run inside an async series.  Since Node.js is
// asynchronous by default, the series is needed to keep the steps from
// running at the same time.  The series also makes the execution of the
// app easier to follow on its web UI.

async.series(
  [
    //
    // Step 2a - check if Streams instance is running
    // 

    function(callback) {
      //
      // Begin - GET status
      //
      status_step[1] = "Starting";
      console.info("\nUsing the Streaming Analytics REST API to get the instance status and jobs information.");
      console.info("Preparing to call Jobs GET API");  

      // prepare the header
      var getheaders = {
        'Content-Type' : 'application/json',
        'Authorization' : authbuf
      };

      // set the get options
      var optionsget = {
        host : sa_props.rest_host,
        path : sa_props.jobs_path,
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
    // Step 2b - Start the instance if necessary
    // 

    function(callback) {
  
      if (instanceStatus.state === 'STARTED') {
        // Make callback for async series signaling completion
        callback(null, null);  
      }
      else {
        // Start the instance with a PUT 
        console.info("Using the Streaming Analytics REST API to start the Streams instance.");

        // create the JSON object
        var jsonPut = JSON.stringify({});

        // prepare the header
        var putheaders = {
          'Content-Type' : 'application/json',
          'Content-Length' : Buffer.byteLength(jsonPut, 'utf8'),
          'Authorization' : authbuf
        };
 
        // set the put options
        var optionsput = {
          host : sa_props.rest_host,
          path : sa_props.start_path,
          method : 'PUT',
          headers : putheaders
        }; 

        console.info('Options prepared:');
        console.info(optionsput);

        console.info('Perform the PUT');
        var reqPut = https.request(optionsput, function(res) {

          res.on('data', function(d) {
            console.info('PUT-start result: ' + d);
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
        reqPut.write(jsonPut);
        reqPut.end();
        reqPut.on('error', function(e) {
          console.error(e);
        });
      } // end else

    },


    //
    // Step 3 - Check if the Streams job is running because of some earlier run   
    // of the sample
    //     

    function(callback) {
      // If the instance was not running, no jobs earlier jobs could exist
      if (instanceStatus.state != 'STARTED') {
        status_step[2] = "Completed - No earlier job running";
        // Make callback for async series signaling completion
        callback(null, null);  
      }
      // Check for an existing job
      else {
        var jobFound = false;
        for (var j=0; j < instanceStatus.job_count; j++) {
          if (instanceStatus.jobs[j].name === 'EventDetectionSample') {
            jobFound = true;
            oldJob = instanceStatus.jobs[j].jobId;
            console.info('JOB ID is ' + oldJob);
            break;
          }
        }
        if (!jobFound) {
          status_step[2] = "Completed - No earlier job running";
          // Make callback for async series signaling completion
          callback(null, null);  
        }
        else {
          console.info("About to cancel old job.  Job number is: " + oldJob);

          // Cancel the job via a DELETE-Job 
          console.info("Using the Streaming Analytics REST API to cancel the Streams job.");

          // prepare the header
          var deleteheaders = {
            'Authorization' : authbuf
          };
 
          uri_string = "";
          uri_string = sa_props.jobs_path + '?job_id=' + oldJob;

          console.info('DELETE URI string is: ' + uri_string);

          // set the put options
          var optionsdelete = {
            host : sa_props.rest_host,
            path : uri_string,
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
        } // end-else
      } // end-else
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

      // -----POST Params-----
      var uri_string = sa_props.jobs_path + '?bundle_id=EventDetection.sab';

      // -----SUBMIT POST-----
      var jsonPostRes = {};

      form.submit({ 
        protocol: 'https:',
        host: sa_props.rest_host,
        path: uri_string,
        headers: {'Authorization' : authbuf}
      }, function(err, res) {


         res.on('data', function(data) {
           console.info('GET-status response data:' + data);
           jsonPostRes = JSON.parse(data);
           jobNumber = jsonPostRes.jobId;
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
            // response is ['Node.js', 'JavaScript']
        }
    ); // End of async series



// Helper functions

function finalCancel(jobNumberToCancel) {
  // Cancel the job via a DELETE-Job 
  console.info("Using the Streaming Analytics REST API to cancel the Streams job.");

  // prepare the header
  var deleteheaders = {
    'Authorization' : authbuf
  };
 
  uri_string = "";
  uri_string = sa_props.jobs_path + '?job_id=' + jobNumberToCancel;
  console.info('DELETE URI string is: ' + uri_string);


  // set the put options
  var optionsdelete = {
    host : sa_props.rest_host,
    path : uri_string,
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
        status_step[4] = "Completed";
        status_step[5] = "Completed";
      }
      else {
        status_step[5] = "Failed - Could not cancel the Streams job";
      }
    });   
  });

  reqDelete.end();
  reqDelete.on('error', function(e) {
    console.error(e);
  });
}


