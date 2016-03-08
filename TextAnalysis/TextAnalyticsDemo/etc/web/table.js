function updatePage(response) {
			try {
				// HTTPTupleView retuns tuples information as JSON, parse the JSON into a list of objects to process
						
				var rows  ="";
				console.log(response);					
				for (var i=0; i<response.length; i++) {
						var company = response[i].Organizations;
						var product = response[i].Offerings;
						
						
						var text = response[i].match;
						
						var row = "<tr><td>"+company+"</td><td>"+product+"</td><td";
						row +=">"+text+"</td></tr>";
						rows += row;
					
				}
				 $('#mentions').append(rows);
				 setupCall();
			}
			catch (e) {
				console.log(e)
			}
		}
		function getPeriod() {
			return 4500;
		}
		
		function setupCall() {
			setTimeout(load, getPeriod());
		}
		function load(){
			var url = window.location.protocol + "//" + window.location.host + "/textAnalytics/data/ports/input/0/tuples";
			$.ajax({ url: url, success: updatePage, dataType: "json", timeout: 30000 });
		}


