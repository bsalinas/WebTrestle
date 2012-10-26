$(document).ready(function() {
	var source = new EventSource('/stream');
	var station_identifier = $('h1').attr("data-station_identifier");
	var sensor_data ={};
	var sensor_chart = {};


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
			$('#sensor_table_'+data.sensor_id+' tbody').append('<tr><td>'+data.value+'</td><td>'+data.created_at+'</td></tr>');
			if(e.type === "measurement"){
				drawChart(data.sensor_id);
			}
		}
	},
	changeState = function(e){
		console.log(e);
		var data = JSON.parse(e.data);
		if(data.state_id && data.time){
			$('#state_id_'+data.state_id+' div').html(data.value);
		}
	},
	performedAction = function(e){
		var data = JSON.parse(e.data);
		if(data.action_identifier){
			$('#action_display_'+data.action_identifier).text("Last Performed at "+(new Date()));
			$('#badge_action_'+data.action_id).text("Performed");
		}
	},
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
	drawChart = function(sensor_id){
		var svg_selector = '.chart#sensor_chart_'+sensor_id+' svg';
		nv.addGraph(function() {
			var chart = nv.models.scatterChart();
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
	};

	$('.sensor_header').each(function(){
		var sensor_id = $(this).attr('data-sensor_id');
		getInitialMeasurements(sensor_id);

	});

	

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

	//Now, setup the action buttons
	$('.action_button').each(function(){
		$(this).click(function(e){
			e.preventDefault();
			var action_identifier = $(this).attr("data-action_identifier"), action_id = $(this).attr("data-action_id");
			$.ajax({type: "POST",
				url: '/perform_action',
				data : { identifier : action_identifier, station_identifier: station_identifier}
			}).done(function(msg){
				$('#action_display_'+action_identifier).text("Pending as of "+(new Date()));
				$('#badge_action_'+action_id).text("Pending");
			});

		});
	});

	$('.nav-tabs a').click(function(e){
		e.preventDefault();
		$(this).tab('show');
	});



// 		 $('.sensor_table').dataTable({
// 			"sPaginationType": "bootstrap",
// 			"sDom": "<'row'<'span6'l><'span6'f>r>t<'row'<'span6'i><'span6'p>>",
// 			"bFilter": false, // Disable searching
// 			"bPaginate": true, // Enable pagination
// 			"bInfo": true,    // Enable info texts,
// 			"bSort": false,
// 			"iDisplayLength": 100
// 			} );
		// $('body').scrollspy({target:"#navigation"});
		// $('body').scrollspy("refresh");

 
 
 
 /**************************************
  * Simple test data generator
  */
 
 function generateData(sensor_id){
	var data = [];
	for(var point in sensor_data[sensor_id].data){
		data.push({x: Date.parse(sensor_data[sensor_id].data[point].created_at), y: parseFloat(sensor_data[sensor_id].data[point].value), size: 50});
	}
	return [
	{
		values : data,
		key : "Label",
		color: 'ff7f0e'

	}];
 }
});