from builtins import range
import maya.OpenMaya as om
import json
import csv

__all__ = [ 'profilerToJSON', 'profilerToCSV', 'profilerFormatJSON']

#
# The following is sample code which uses the OpenMaya API to show how
# data from Maya's profiler can be output to file on disk in a custom text format.
#
# Note that references to Maya's profiler output format are valid at the time of writing.
#
# The format's chosen to illustrate this are JSON and CSV. For each sample output is provided.

# Sample 1: Profiler to JSON output
# ---------------------------------
#
def profilerToJSON(fileName, useIndex, durationMin):
	"""
	 	fileName : name of file to write to disk
		useIndex : write events using index lookup to category and name lists
		durationMin : only write out events which have at least this minimum time duration

		Description:
			Sample code to extract profiler information and write to file in JSON format

		Example usage:
			> profilerToJSON('profiler_indexed.json', True, 0.0) # Index without a duration clamp
	 		> profilerToJSON('profiler_nonIndexed.json', False, 10.0) # Non-Indexed with duration clamp
	"""
	# Below is some sample output from after running the JSON export. Note that here we try to mimic
	# the data and ordering of Maya's internal profiler save by storing category and event name lists
	# and specifying lookups for each event. e.g. the first event stores a name index (nameIdx) of 59
	# which would reference element 59 in "eventNames".
	#
	# Note that the total available events and the number of events actually written out is included.
	#
	# {
		# "version": 1,
		# "eventCount": 11276,
		# "cpuCount": 8,
		# "categories": ["Main", "Change", "Dirty Propagation", "Evaluation", "VP1 Evaluation", "Poly", "Mudbox", "VP2 Evaluation", "Qt", "Render Setup", "Anim UI Refresh"],
		# "eventNames": ["QtDynamicPropertyChange", "PreRefreshTopLevel", "QtCreate", "Vp2ShaderDoDG", "QtShortcutOverride", "Vp2SceneRender", "Vp2ShaderItemCompileShader", "QtShowToParent", "QtMove", "Vp2AssignShaderInstance", "opaqueUIList", "Vp2ParallelEvaluationTask", "QtResize", "colorPass", "pConeShape1", "QtWindowDeactivate", "QtZeroTimerEvent", "QtTimer", "QtNonClientAreaMouseButtonPress", "QtMouseButtonRelease", "QtChildRemoved", "QtApplicationDeactivate", "Vp2ShaderWrite", "QtChildAdded", "QtWindowActivate", "Vp2PrepareToUpdate", "QtMetaCall", "QtActivationChange", "QtUpdateLater", "QtFocusIn", "QtShow", "QtUserEvent", "QtWindowStateChange", "QtModifiedChange", "Vp2UpdateGeometryBuffer", "setDirty", "Vp2AcquireOffScreenTarget", "QtKeyRelease", "uiGeometry", "Vp2UpdateUI", "QtUpdateRequest", "QtToolTipChange", "QtApplicationPaletteChange", "QtDragMove", "preUIGeometry", "Vp2WaitForEvaluation", "QtKeyPress", "QtNonClientAreaMouseButtonRelease", "QtLeave", "opaqueGeometry", "initialShadingGroup", "QtDrop", "Vp2BuildRenderLists", "Vp2UpdateGeometry", "Compute", "Vp2ClearRenderLists", "Vp2UpdateScene", "postUIGeometry", "QtMouseButtonPress", "QtIdleTimer", "QtDragEnter", "QtDestroy", "QtPaint", "Vp2BuildShadowMaps", "Vp2UpdateDagObject", "QtEnter", "pPyramidShape1", "QtMouseMove", "Vp2TranslateGeometry", "QtWindowTitleChange", "QtZOrderChange", "ScheduleRefreshAllViews", "QtDeferredDelete", "QtPaletteChange", "QtNonClientAreaMouseMove", "shadedBeautyGraphSemantic", "QtApplicationActivate", "QtCursorChange", "Vp2WaitForTranslation", "sendAttributeChangedMsg", "QtLayoutRequest", "QtStatusTip", "QtDragLeave", "QtFocusOut", "Vp2Draw3dBeautyPass", "Vp2HUD", "Vp2ConstructFragmentGraph", "QtHide"],
		# "events": [{
			# "time": 25610841341,
			# "nameIdx": 59,
			# "desc": "",
			# "catIdx": 8,
			# "duration": 39014,
			# "tDuration": 39724089,
			# "tId": 6972,
			# "cpuId": 4,
			# "colorId": 6
		# }, {
			# "time": 25620057925,
			# "nameIdx": 79,
			# "desc": "",
			# "catIdx": 1,
			# "duration": 228,
			# "tDuration": 212774,
			# "tId": 6972,
			# "cpuId": 0,
			# "colorId": 12
		# }
		# .... more events ...
		# ]
		# "eventsWritten": 364
	# }
	#
	# Here is the same data without using indexing.
	# {
    # "cpuCount": 8,
    # "eventCount": 11276,
    # "events": [
        # {
            # "category": "Qt",
            # "colorId": 6,
            # "cpuId": 4,
            # "desc": "",
            # "duration": 39014,
            # "name": "QtIdleTimer",
            # "tDuration": 39724089,
            # "tId": 6972,
            # "time": 25610841341
        # },
        # {
            # "category": "Change",
            # "colorId": 12,
            # "cpuId": 0,
            # "desc": "",
            # "duration": 228,
            # "name": "sendAttributeChangedMsg",
            # "tDuration": 212774,
            # "tId": 6972,
            # "time": 25620057925
     	# }
		# .... more events ...
		# ]
		# "eventsWritten": 364
	#
	stripped = lambda s: "".join(i for i in s if 31 < ord(i) < 127)

	eventCount = om.MProfiler.getEventCount()
	if eventCount == 0:
		return

	file = open(fileName, "w")
	if not file:
		return


	file.write("{\n")

	# Output version
	file.write("\t\"version\": 1,\n")
	# Output event count
	file.write("\t\"eventCount\": " + str(eventCount) + ",\n")
	# Output number of CPUS. Missing from Python API
	file.write("\t\"cpuCount\": " + str(om.MProfiler.getNumberOfCPUs()) + ",\n")

	# Output event categories if using indexing lookup
	categories = []
	om.MProfiler.getAllCategories(categories)
	asciiString = json.dumps(categories, True, True)
	if useIndex:
		file.write("\t\"categories\": " + asciiString + ",\n")

	# Output event name list if using indexing
	nameDict = {}
	for i in range(0, eventCount, 1):
		eventName = om.MProfiler.getEventName(i)
		eventName = eventName.decode('ascii', 'replace')
		eventName = stripped(eventName)
		if eventName not in nameDict:
			nameDict[eventName] = len(nameDict)
	if useIndex:
		nameString = json.dumps(list(nameDict.keys()), True, True)
		file.write('\t\"eventNames\" : ' + nameString + ",\n")

	# Write out each event:
	# Event time, Event Name / Event Index, Description , Category / Category index, Duration, Thread Duration, Thread id, Cpu id, Color id
	file.write('\t\"events\": [\n')
	dumped = False
	eventsWritten = 0
	for i in range(0, eventCount):

		duration = om.MProfiler.getEventDuration(i)
		if duration > durationMin:
			eventsWritten = eventsWritten + 1

			eventTime = om.MProfiler.getEventTime(i)
			eventName = om.MProfiler.getEventName(i)
			eventName = eventName.decode('ascii', 'replace')
			eventName = stripped(eventName)
			if useIndex:
				eventNameIndex = list(nameDict.keys()).index(eventName)

			description = ''
			if om.MProfiler.getDescription(i):
				description = om.MProfiler.getDescription(i)

			eventCategory = om.MProfiler.getEventCategory(i)
			eventCategoryName = om.MProfiler.getCategoryName(eventCategory)
			if useIndex:
				eventCatagoryIndex = categories.index(eventCategoryName)

			threadDuration = om.MProfiler.getThreadDuration(i)

			threadId = om.MProfiler.getThreadId(i)

			cpuId = om.MProfiler.getCPUId(i)

			colorId = om.MProfiler.getColor(i)

			# Instead of using json library, the code just writes on the fly
			if dumped:
				file.write('\t,{ ')
			else:
				file.write('\t{ ')
			dumped = True
			file.write('\"time\" : ' + str(eventTime) + ', ')
			if useIndex:
				file.write('\"nameIdx\" : ' + str(eventNameIndex) + ', ')
			else:
				file.write('\"name\" : \"' + eventName + '\", ')
			file.write('\"desc\" : \"' + str(description) + '\", ')
			if useIndex:
				file.write('\"catIdx\" : ' + str(eventCatagoryIndex) + ', ')
			else:
				file.write('\"category\" : \"' + eventCategoryName + '\", ')
			file.write('\"duration\" : ' + str(duration) + ', ')
			file.write('\"tDuration\" : ' + str(threadDuration) + ', ')
			file.write('\"tId\" : ' + str(threadId) + ', ')
			file.write('\"cpuId\" : ' + str(cpuId) + ', ')
			file.write('\"colorId\" : ' + str(colorId) + '')
			file.write('\t}\n')

	file.write("\t],\n")
	file.write("\t\"eventsWritten\": " + str(eventsWritten) + "\n")
	file.write("}\n")
	file.close()

def profilerFormatJSON(fileName, fileName2):
	"""
	 	fileName : name of file to read
		fileName2 : name of file to write to

		Description:
			Simple utility code to read a JSON file sort and format it before
			writing to a secondary file.

		Example:
			> profilerFormatJSON('profilerIn.json', 'profilerFormatted.json')

	"""
	file = open(fileName, "r")
	if not file:
		return

	result = json.load(file)
	file.close()

	dump = json.dumps(result, sort_keys=True, indent=4, separators=(',', ': '))

	file2 = open(fileName2, "w")
	if not file2:
		return

	file2.write(dump)
	file2.close()




# Sample 1: Profiler to CSV output
# ---------------------------------
#
def profilerToCSV(fileName, durationMin):
	"""
	 	fileName : name of file to write to disk
		useIndex : write events using index lookup to category and name lists
		durationMin : only write out events which have at least this minimum time duration

		Description:
			Sample to output profiler event information only to CSV format.
		Example:
			> profilerToCSV('profiler.csv', 0.0)
	"""
	#
	# Sample output:
	#
	# Event Time","Event Name","Description","Event Category","Duration","Thread Duration","Thread Id","CPU Id","Color Id"
	# 25610841341,"QtIdleTimer","","Qt",39014,39724089,6972,4,6
	# 25620057925,"sendAttributeChangedMsg","","Change",228,212774,6972,0,12
	# 25620058186,"setDirty","","Dirty Propagation",8,7806,6972,0,1
	# 25620058276,"sendAttributeChangedMsg","","Change",11,10633,6972,0,12
	# 25620058310,"sendAttributeChangedMsg","","Change",8,7732,6972,0,12
	# 25620058332,"sendAttributeChangedMsg","","Change",7,6844,6972,0,12
	# ... <more events> ...
	#
	stripped = lambda s: "".join(i for i in s if 31 < ord(i) < 127)
	
	eventCount = om.MProfiler.getEventCount()
	if eventCount == 0:
		return

	file = open(fileName, "w")
	if not file:
		return

	csvWriter = csv.writer(file, quoting=csv.QUOTE_NONNUMERIC)

	# Write out each event:
	# Event time, Event Name / Event Index, Description , Category / Category index, Duration, Thread Duration, Thread id, Cpu id, Color id

	head = ( 'Event Time', 'Event Name', 'Description', 'Event Category', 'Duration', 'Thread Duration', 'Thread Id', 'CPU Id', 'Color Id' )
	csvWriter.writerow(head)

	for i in range(0, eventCount):

		duration = om.MProfiler.getEventDuration(i)
		if duration > durationMin:

			eventTime = om.MProfiler.getEventTime(i)
			eventName = om.MProfiler.getEventName(i)
			eventName = eventName.decode('ascii', 'replace')
			eventName = stripped(eventName)
			
			description = ''
			if om.MProfiler.getDescription(i):
				description = om.MProfiler.getDescription(i)

			eventCategory = om.MProfiler.getEventCategory(i)
			eventCategoryName = om.MProfiler.getCategoryName(eventCategory)

			threadDuration = om.MProfiler.getThreadDuration(i)

			threadId = om.MProfiler.getThreadId(i)

			cpuId = om.MProfiler.getCPUId(i)

			colorId = om.MProfiler.getColor(i)

			row = ( eventTime, eventName, description, eventCategoryName, duration, threadDuration, threadId, cpuId, colorId )

			csvWriter.writerow(row)

	file.close()


# Nothing run on initialize for now
def initializePlugin(obj):
	obj

def uninitializePlugin(obj):
    obj
