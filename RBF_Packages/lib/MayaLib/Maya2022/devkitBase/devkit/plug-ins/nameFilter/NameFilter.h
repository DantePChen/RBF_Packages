#ifndef __NameFilter_h
#define __NameFilter_h

#include <maya/MPxCacheConfigRuleFilter.h>
#include <maya/MString.h>

// The NameFilter class is a cache configuration filter that looks at the 
// dependency graph (DG) nodes relating to caching in order to match their 
// name with a pattern.
//
// This filter requires a param string 
// (newFilterParam from the cacheEvaluator command) formatted like so:
//      "pattern,option"
// where
//      pattern is the substring to search for in all the dependency nodes 
//              relating to the cache configuration
//      option is optional. Can be one of the following string values:
//          reverse: reverse logic match behavior: the filter will match on 
//                   node names NOT containing the pattern
//
// It can be used to include or exclude nodes from caching modes based on 
// their names when used with the cacheEvaluator command.
//
// Example:
//  Use default evaluation cache configuration mode but exclude cones from 
//  the cache (Python):
//
//      import maya.cmds as cmds
//      cmds.loadPlugin( 'nameFilter.mll' )
//      
//      # Reset Rules
//      cmds.cacheEvaluator(resetRules = True);
//      
//      # Add Eval Cache action/filter
//      cmds.cacheEvaluator(newFilter = 'evaluationCacheNodes', newAction = 'enableEvaluationCache' )
//      
//      # Add Custom Filter
//      cmds.cacheEvaluator(newFilter = 'NameFilter', newFilterParam = 'Cone', newAction = 'disableAllCache' )
//
// See also the OpenMaya API documentation for the MPxCacheConfigRuleFilter 
// proxy class
class NameFilter : public MPxCacheConfigRuleFilter
{
public:
    NameFilter( bool reverseLogic, const MString &pattern );
    bool isMatch(const MEvaluationNode& evalNode) override;


private:
    bool    fReverseLogic;
    MString fPattern;
};

// Creator function for the filter
MPxCacheConfigRuleFilter* NameFilterCreator(const MString &param);

#endif // __NameFilter_h