function toColor(value) 
{
	return "#" + (Math.round(value * 0XFFFFFF)).toString(16);
}

window.onload = function() 
{
		var fileInput = document.getElementById('fileInput');
		var eventList = document.getElementById('elementList');
		var eventCount = document.getElementById("eventCount");
		var canvas = document.getElementById("canvas");
		
		fileInput.addEventListener('change', function(e) {
			var file = fileInput.files[0];
			var textType = /text.*/;

			var reader = new FileReader();

			reader.onload = function(e)
			{
				var jsonText = reader.result;
				var jsonObject = JSON.parse(jsonText);

				// Write header
				eventHeader.innerHTML = 'Profiler Version: ' + jsonObject.version + "<br>";
			 	eventHeader.innerHTML += 'Total Event Count: ' + jsonObject.eventCount + "<br>";
				eventHeader.innerHTML += 'Events Written: ' + jsonObject.eventsWritten + "<br>";
				eventHeader.innerHTML += 'CPU Count: ' + jsonObject.cpuCount + "<br>";
				if (jsonObject.categories)
				{
					eventHeader.innerHTML += 'Categories:';
					eventHeader.innerHTML += '<ol>';
					for (i=0; i<jsonObject.categories.length; i++)
					{
						eventHeader.innerHTML += '<li>[' + i + '] = ' + jsonObject.categories[i];
					}
					eventHeader.innerHTML += '</ol>';
				}
				if (jsonObject.eventNames)
				{
					eventHeader.innerHTML += '<br>Event Names:';
					eventHeader.innerHTML += '<ol>';
					for (i=0; i<jsonObject.eventNames.length; i++)
					{
						eventHeader.innerHTML += '<li>[' + i + '] = ' + jsonObject.eventNames[i];
					}
					eventHeader.innerHTML += '</ol>';
				}

				var i;
				var eventTableInput = jsonObject.events;
      			var eventTable = "<table border=1>";

				// Table header items are the key labels
				eventTable += "<tr>";

				var haveDuration = false;
				var haveColorId = false;
				var categoriesArray = [];
				var categoriesColors = [];
				var categoryKey = "";
				if (eventTableInput.length)
				{
					eventRow =  eventTableInput[0];
					// Example to sort by duration descending 
					if ("duration" in eventRow)
					{
						haveDuration = true;
						eventTableInput.sort(
							function(a,b)
							{
								return b.duration-a.duration;
							});
					}
					if ("colorId" in eventRow)
					{
						haveColorId = true;
					}
					
					for (var key in eventRow)
					{
						eventTable += "<td><b>" + key.toUpperCase() + "</b></td>";
					}
			
					var categories = new Set();
					for(i = 0; i < eventTableInput.length; i++)		
					{
						eventRow =  eventTableInput[i];
						if ("catIdx" in eventRow)
						{
							categories.add(eventRow['catIdx'])
							categoryKey = "catIdx";
						}
						else if ("category" in eventRow)
						{
							categories.add(eventRow['category'])
							categoryKey = "category";
						}
					}
					categories.forEach(v => categoriesArray.push(v));
					categories.forEach(v => categoriesColors.push(Math.random()));
					console.log("Categories: " + categoriesArray);
					console.log("Categories: " + categoriesColors);					
				}
				eventTable += "</tr>";

				// Row items are key data
				var durationData = [];	
				var durationTotal = 0.0;
				var durationColors = [];
				for(i = 0; i < eventTableInput.length; i++)
				{
					eventRow =  eventTableInput[i];

					if (haveDuration)
					{
						duration = eventRow['duration'];
						durationTotal += duration;
						if (categoriesArray.length && categoryKey.length)
						{
							catIdx = categoriesArray.indexOf(eventRow[categoryKey]);
							console.log('cat: ' + eventRow[categoryKey] + '. cat idx: ' + catIdx + "/" + categoriesArray.length + 'cat color: ' + categoriesColors[catIdx]);
							//durationColor = toColor(categoriesColors[catIdx] * (catIdx / categoriesArray.length));
							durationColor = toColor((catIdx+1) / (categoriesArray.length+1));
						}
						else
						{
							durationColor = toColor(Math.random());
						}
						durationData.push(duration);
						durationColors.push(durationColor);
					}
					
					eventTable += "<tr>"
					for (var key in eventRow)
					{
						if (key == 'duration')
						{
							var x = "<td bgcolor=\"" + durationColor + "\"><font color=\"#FFFFFF\">" + eventRow[key] + "</font></td>";	
							console.log(x);
							eventTable += x;							
						}
						else
						{
							eventTable += "<td>" + eventRow[key] + "</td>";
						}
					}
					eventTable += "</tr>"
				}
      			eventTable += "</table>";
      			eventList.innerHTML = eventTable;
				
				// Draw a pie chart
				if (canvas)
				{
					var ctx = canvas.getContext("2d");
					
					ctx.clearRect(0, 0, canvas.width, canvas.height);
					if (durationData.length)
					{				
						var center = [canvas.width / 2, canvas.height / 2];
						var radius = Math.min(canvas.width, canvas.height) / 2;
						var lastPosition = 0, total = 0;
						
						for (var i = 0; i < durationData.length; i++) 
						{
							ctx.fillStyle = durationColors[i];
							ctx.beginPath();
							ctx.moveTo(center[0],center[1]);
							ctx.arc(center[0],center[1],
									radius,
									lastPosition,lastPosition+(Math.PI*2*(durationData[i]/durationTotal)),
									false);
							ctx.lineTo(center[0],center[1]);
							ctx.fill();
							lastPosition += Math.PI*2*(durationData[i]/durationTotal);
						}
					}
				}
			}

			reader.readAsText(file);
		});
}
