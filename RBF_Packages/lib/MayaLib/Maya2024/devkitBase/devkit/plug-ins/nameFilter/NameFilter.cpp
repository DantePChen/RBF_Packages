#include "NameFilter.h"

#include <maya/MCacheConfigRuleRegistry.h>
#include <maya/MEvaluationNode.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MStringArray.h>

MStatus initializePlugin( MObject obj )
//
//	Description:
//		Registers the NameFilter cache configuration rule filter
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{ 
    return MCacheConfigRuleRegistry::registerFilter( "NameFilter", NameFilterCreator );
}

MStatus uninitializePlugin( MObject obj )
//
//	Description:
//		Unregisters the NameFilter cache configuration rule filter
//
//	Arguments:
//		obj - a handle to the plug-in object (use MFnPlugin to access it)
//
{
    return MCacheConfigRuleRegistry::unregisterFilter( "NameFilter" );
}

NameFilter::NameFilter(bool reverseLogic, const MString &pattern)
    : fReverseLogic( reverseLogic )
    , fPattern( pattern )
{
}

bool NameFilter::isMatch(const MEvaluationNode& evalNode)
{
    MStatus status;
    MFnDependencyNode fnNode( evalNode.dependencyNode(&status) );

    if ( MStatus::kSuccess != status ) return false;

    MString absName = fnNode.absoluteName( &status );
    if ( MStatus::kSuccess != status ) return false;

    // Simple name-matching logic, using the reverse logic if specified
    bool nodeNameMatchesPattern = ( -1 != absName.indexW(fPattern) );
    return fReverseLogic ? !nodeNameMatchesPattern : nodeNameMatchesPattern;
}

MPxCacheConfigRuleFilter* NameFilterCreator(const MString &param)
{
    // Parse the creation parameters here 
    if ( 0 == param.length() ) {
        MGlobal::displayError( "Cannot construct NameFilter object: empty parameter string" );
        return nullptr;
    }

    // Split the string 
    MStringArray tokens;
    param.split( ',', tokens );

    if ( tokens.length() > 2 ) {
        MGlobal::displayError( "Cannot construct NameFilter object: the parameter string has too many tokens" );
        return nullptr;
    }

    MString pattern = tokens[0];

    // Get the option
    bool reverseLogic = false;
    if ( tokens.length() > 1 ) {
        if ( tokens[1] == "reverse" ) {
            reverseLogic = true;
        } 
        else {
            MString displayString = "Cannot construct NameFilter object: expected \"reverse\" for the second token in the parameter string, got \"" + tokens[1] + "\"";
            MGlobal::displayError( displayString );
            return nullptr;
        }
    }

    return new NameFilter( reverseLogic, pattern );
}