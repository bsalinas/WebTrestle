$(document).ready(function() {
	var source = new EventSource('/stream');
	var station_identifier = $('h1').attr("data-station_identifier");

	var addMeasurement = function(e){
		var data = JSON.parse(e.data);
		if(data.sensor_id && data.time){
			$('#sensor_table_'+data.sensor_id+' tbody').append('<tr><td>'+data.value+'</td><td>'+data.time+'</td></tr>');
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
		}
	};

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
			var action_identifier = $(this).attr("data-action_identifier");
			$.ajax({type: "POST",
				url: '/perform_action',
				data : { identifier : action_identifier, station_identifier: station_identifier}
			}).done(function(msg){
				$('#action_display_'+action_identifier).text("Pending as of "+(new Date()));
			});

		});
	});

	$('.tabs').tabs();

	$('.chart').each(function(){
		var sensor_id = $(this).attr('data-sensor_id'),
		svg_selector = '.chart#sensor_chart_'+sensor_id+' svg';
		nv.addGraph(function() {
			var chart = nv.models.lineWithFocusChart();
		    chart.xAxis
		        .axisLabel('Time')
		        .tickFormat(function(d){
				return d3.time.format('%x %X')(new Date(d));
		        });
		  
		    chart.yAxis
		        .axisLabel('Value');
		 
		   d3.select(svg_selector)
		       .datum(generateData(sensor_id))
		     .transition().duration(500)
		       .call(chart);
		       nv.utils.windowResize(function() { d3.select(svg_selector).call(chart);});
		     return chart;
	   });
});
 
 
 
 
 /**************************************
  * Simple test data generator
  */
 
 function generateData(sensor_id){
	var data = [];
	$('#sensor_table_'+sensor_id+' .measurement_value').each(function(i,d){
		data.push({x: Date.parse($(this).attr('data-time')), y: parseFloat($(this).attr('data-value')), size: 50});
	});
	return [
	{
		values : data,
		key : "Label",
		color: 'ff7f0e'

	}];
 }
});