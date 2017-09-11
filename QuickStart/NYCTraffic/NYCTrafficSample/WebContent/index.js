// index.js
// set the doc link based on where we are running
var myUrl = window.location.host;
var newUrl = "";
if (myUrl.indexOf("stage1") == -1) {
  newUrl = "https://www.ng.bluemix.net/docs/services/StreamingAnalytics/index.html";
} else { 
  newUrl = "https://www.stage1.ng.bluemix.net/docs/services/StreamingAnalytics/index.html";
}
var a = document.getElementById('docLink'); 
a.href = newUrl;

refreshStatus(); // run once immediately 
var refreshInterval1 = window.setInterval(refreshStatus, 60000); // Refresh status every 60 seconds

function refreshStatus()
{
    xhrGet("api/jax-rs/refreshStatus", function(responseText){
	var statusDate = new Date();
	var lastStatusLine = getLastStatusLine(responseText);
	document.getElementById("lastStatusLine").innerHTML = lastStatusLine;
	topTens();
	document.getElementById("FullAppStat").innerHTML = "<h2>Full Application Status - " +  statusDate + "</h2>";
	var outstat = statusBuilder(responseText);
    document.getElementById("id00").innerHTML = outstat;
    }, function(err){
	  console.log(err);
    });
}

function topTens()
{
	xhrGet("api/jax-rs/topTens", function(responseText){
	var tableDate = new Date();
	document.getElementById("NTCTables").innerHTML = "<h3>New York City Traffic Information - " +  tableDate + "</h3>";
	tablesBuilder(responseText);
	}, function(err){
		console.log(err);
	});
}


//utilities

function statusBuilder(jsonInput){
	// take a json array of status lines and build a scrollable list.
	var myArr = JSON.parse(jsonInput).refreshStatus;
    var myi;
    var myout = '<textarea rows="5" cols="130">';

    for(myi = 0; myi < myArr.length; myi++) {
        myout += myArr[myi].statusLine+"\n";
    }
    myout += '</textarea>';
	return myout;
}	

function getLastStatusLine(jsonInput){
	// take a json array of status lines and return the most recent line
	var myArr1 = JSON.parse(jsonInput).refreshStatus;
    var myi1;
    var myout1 = '<p>Current Application Status - ';
    if (myArr1.length >0) {
      myout1 += myArr1[0].statusLine;
    }  
    myout1 += '</p>';
	return myout1;
}	


function tablesBuilder(jsonIn) {
	var myTables = JSON.parse(jsonIn).TopTens;
	if (myTables.length == 0) {
	  document.getElementById("InitMessage").innerHTML = "<p>Initializing... The New York City Traffic Information is updated about every minute. The Streams application aggregates data across a 5 minute period (outputting each minute) so it may take up to 2 minutes for the first table data to appear.</p>";
	} else 
	{ 
	  document.getElementById("InitMessage").innerHTML = "";
	  for(i = 0; i < myTables.length; i++) {
	    var name = myTables[i].attributeName;
	    var tableData = myTables[i].data;
	    var outcurrent = tableBuilder(tableData, "Top Ten "+name, name);
	 
        document.getElementById(name).innerHTML = outcurrent; 
      }
	}
}

function tableBuilder(jsonInput, caption, attribute){
	var myArr = jsonInput;
    var myi;
    var myout = '<table id="t01" style="margin-right:10px;">';
	myout += "<caption style=\"font-size:150%; color:white\"  >" + caption + "</caption>";
	myout += "<tr><th>Id</th><th>" + attribute + "</th><th>Link Description</th></tr>";

    for(myi = 0; myi < myArr.length; myi++) {
        myout += "<tr><td>" +
		 myArr[myi]['Id'] +
        "</td><td>" +
        myArr[myi][attribute] +
        "</td><td>" +
        myArr[myi].linkName +
        "</td></tr>";
    }
    myout += "</table>"
	return myout;
}

function createXHR(){
	if(typeof XMLHttpRequest != 'undefined'){
		return new XMLHttpRequest();
	}else{
		try{
			return new ActiveXObject('Msxml2.XMLHTTP');
		}catch(e){
			try{
				return new ActiveXObject('Microsoft.XMLHTTP');
			}catch(e){}
		}
	}
	return null;
}
function xhrGet(url, callback, errback){
	var xhr = new createXHR();
	xhr.open("GET", url, true);
	xhr.onreadystatechange = function(){
		if(xhr.readyState == 4){
			if(xhr.status == 200){
				callback(xhr.responseText);
			}else{
				errback('service not available');
			}
		}
	};
	xhr.timeout = 60000; // set 1 minute timeout
	xhr.ontimeout = errback;
	xhr.send();
}
function parseJson(str){
	return window.JSON ? JSON.parse(str) : eval('(' + str + ')');
}
function prettyJson(str){
	// If browser does not have JSON utilities, just print the raw string value.
	return window.JSON ? JSON.stringify(JSON.parse(str), null, '  ') : str;
}

