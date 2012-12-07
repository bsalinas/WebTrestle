$(document).ready(function() {
	//The EventSource variable.
	var source;
	//Setup the eventsource if it exists.
	if(typeof(EventSource)!=="undefined"){
	 	source = new EventSource('/stream');
	 	source.addEventListener('measurement', addMeasurement, false);
		source.addEventListener('state', changeState, false);
		source.addEventListener('action_performed', performedAction, false);
		source.addEventListener('open', function(e) {
		  // Connection was opened.
		  console.log("open");
		}, false);
		source.addEventListener('error', function(e) {
			if (e.readyState == EventSource.CLOSED) {
			// Connection was closed
			console.log("Closed");
			}
		}, false);
	  }
	else{
	  // Sorry! No server-sent events support..
	  }
	
	//Grep the Station Identifier from the header element.
	var station_identifier = $('h1').attr("data-station_identifier"),
	//Initialize the sensor_data object
	sensor_data ={},
	//Initialize the arrays of charts
	sensor_chart = {};

	//Called when a new measurement is added to a sensor.
	//Called from the EventStream for measurement
	var addMeasurement = function(e){
		var data = e.data;
		if(typeof data === 'string'){
			data = JSON.parse(data);
		}
		if(data.sensor_id && data.created_at){
			if(!sensor_data[data.sensor_id]){
				sensor_data[data.sensor_id]={data: []};
			}
			sensor_data[data.sensor_id].data.push({value:data.value, created_at:data.created_at});
			$('#sensor_table_'+data.sensor_id+' tbody').append('<tr><td>'+parseFloat(data.value)+'</td><td>'+data.created_at+'</td></tr>');
			if(e.type === "measurement"){
				drawChart(data.sensor_id);
			}
		}
	},
	//Called when a state is changed by the EventStream
	changeState = function(e){
		console.log(e);
		var data = JSON.parse(e.data);
		if(data.state_id && data.time){
			$('#state_id_'+data.state_id+' div').html(data.value);
			$('#badge_state_'+data.state_id).text("Updated").show();
		}
	},
	//Called when an action is performed by the Arduino
	performedAction = function(e){
		var data = JSON.parse(e.data);
		if(data.action_identifier){
			$('#action_display_'+data.action_identifier).text("Last Performed at "+(new Date()));
			$('#badge_action_'+data.action_id).text("Performed");
		}
	},
	//Makes an ajax request to get all the measurements for the specified sensor and draws the chart
	getInitialMeasurements = function(sensorid){
			//Get the initial data
		$.ajax({type: "GET",
			url: "/get_measurements",
			data:{sensor_id : sensorid}
		}).done(function(data){
			$.each(data.measurements, function(){
				addMeasurement({data:this});
			});
			sensor_chart[sensorid] = drawChart(sensorid);
		});
	},
	generateData = function(sensor_id){
		var data = [];
		for(var point in sensor_data[sensor_id].data){
			var arr = sensor_data[sensor_id].data[point].created_at.split(/[- :]/);
			var date = new Date(arr[0], arr[1]-1, arr[2], arr[3], arr[4], arr[5]);
			data.push({x: date, y: parseFloat(sensor_data[sensor_id].data[point].value), size: 50});
		}
		return [
		{
			values : data,
			key : "Label",
			color: '3A87AD'
		}];
	 },
	//Draws the chart for the specified sensor_id
	drawChart = function(sensor_id){
		//First, let's update the CSV
		$('#badge_sensor_'+sensor_id).text(parseInt($('#badge_sensor_'+sensor_id).text())||0 + 1).show();
		$('a#download_link_'+sensor_id).attr('href',createDownloadHref(sensor_id));
		var now = new Date();
		var formattedDate = (now.getMonth()+1)+'-'+now.getDate()+'-'+now.getFullYear()+'_'+now.getHours()+':'+now.getMinutes();
		$('a#download_link_'+sensor_id).attr('download',$('a#download_link_'+sensor_id).attr('data-name')+"_"+formattedDate+'.csv');


		var svg_selector = '.chart#sensor_chart_'+sensor_id+' svg';
		nv.addGraph(function() {
			var chart = nv.models.lineWithFocusChart();
		    chart.xAxis
		        .axisLabel('Time')
		        .tickFormat(function(d){
				return d3.time.format('%x %X')(new Date(d));
		        });
		  
		    chart.yAxis
		        .axisLabel('Value');
		    $(svg_selector).empty();
		   d3.select(svg_selector)
		       .datum(generateData(sensor_id))
		     .transition().duration(500)
		       .call(chart);
		       nv.utils.windowResize(function() { d3.select(svg_selector).call(chart);});
		     return chart;
	   });
	},
	//text = the file content, name = the name of the file
	//Watch out for odd characters like '-' or other things... it chokes.
	createDownloadHref = function(sensor_id){
		var text = "Time,Value\n";
		for( var point in sensor_data[sensor_id].data){
			text= text+'"'+sensor_data[sensor_id].data[point].created_at+'","'+sensor_data[sensor_id].data[point].value+'"\n';
		}
		var html = 'data:application/octet-stream;base64,'+window.btoa(text);
		return html;
	};
	//Gets the initial measurements based on the stubbed out initial page.
	$('.sensor_header').each(function(){
		var sensor_id = $(this).attr('data-sensor_id');
		getInitialMeasurements(sensor_id);

	});

	//Now, setup the action buttons
	//When the button is pressed, we need to send an AJAX request to /perform_action
	//Also, change the badge on the navigation.
	$('.action_button').each(function(){
		$(this).click(function(e){
			e.preventDefault();
			var action_identifier = $(this).attr("data-action_identifier"), action_id = $(this).attr("data-action_id");
			$.ajax({type: "POST",
				url: '/perform_action',
				data : { identifier : action_identifier, station_identifier: station_identifier}
			}).done(function(msg){
				$('#action_display_'+action_identifier).text("Pending as of "+(new Date()));
				$('#badge_action_'+action_id).text("Pending").show();
			});

		});
	});

	//Handles the switching of tabs.
	$('.nav-tabs a').click(function(e){
		e.preventDefault();
		$(this).tab('show');
	});

	//When you scroll over a new data badge, hide it.
	$('[data-spy="scroll"]').on('activate', function(e){
		$(e.target).find('.badge').text('').hide();
	});
});