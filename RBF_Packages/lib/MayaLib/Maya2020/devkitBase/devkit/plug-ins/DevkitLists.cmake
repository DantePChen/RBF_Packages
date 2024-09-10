# The plugin list for devkit

add_subdirectory(autoLoader)
add_subdirectory(simpleEvaluationDraw)
add_subdirectory(simpleEvaluationNode)
add_subdirectory(XmlGeometryCacheDesc)
add_subdirectory(iffInfoCmd)
add_subdirectory(iffPixelCmd)
add_subdirectory(iffPpmCmd)
add_subdirectory(testNucleusNode)
add_subdirectory(testNsolverNode)
add_subdirectory(testNobjectNode)
add_subdirectory(testNpassiveNode)

add_subdirectory(polyRawExporter)
add_subdirectory(polyX3DExporter)

add_subdirectory(richMoveCmd)
add_subdirectory(listRichSelectionCmd)

add_subdirectory(stringFormatNode)
add_subdirectory(threadTestCmd)
add_subdirectory(threadTestWithLocksCmd)
add_subdirectory(splatDeformer)
add_subdirectory(threadingLockTests)
add_subdirectory(threadedBoundingBox)
add_subdirectory(sseDeformer)

add_subdirectory(randomizerDevice)
add_subdirectory(udpDevice)
add_subdirectory(gameInputDevice)

add_subdirectory(geometrySurfaceConstraint)

add_subdirectory(squareScaleManip)
add_subdirectory(squareScaleManipContext)
add_subdirectory(lineManip)
add_subdirectory(pointManip)
add_subdirectory(lineManipContainer)

add_subdirectory(MetadataSample)
add_subdirectory(MetadataXML)
add_subdirectory(nameFilter)

# The list from SuperConductor, used in standalone devkit
if (NOT INSIDE_MAYA)
	add_subdirectory(animImportExport)
	add_subdirectory(atomImportExport)
	add_subdirectory(glslShader)
    if (NOT APPLE)
        add_subdirectory(objExport)
    endif()
	add_subdirectory(testMTopologyEvaluator)
    if (WIN32)
        add_subdirectory(hlslShader)
    endif()
endif()

    if (NOT APPLE)
	    add_subdirectory(cgFx)
    endif()
	if (WIN32)
		add_subdirectory(dx11Shader)
	endif()
