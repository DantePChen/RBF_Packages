#include <iostream>
#include <maya/MStatus.h>
#include <maya/MIOStream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnCompoundAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnGeometryData.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnMesh.h>
#include <maya/MFnLatticeData.h>
#include <maya/MFnNurbsSurfaceData.h>
#include <maya/MFnNurbsSurface.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnLattice.h>
#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MMatrix.h>
#include <maya/MFnMatrixData.h>
#include <maya/MFnNumericData.h>
#include <cmath>
#include <algorithm>
constexpr double  kFloatEpsilon		{1.0e-4};


/////////////////////////////////
// Plugin Compare Shader Class //
/////////////////////////////////

class Compare : public MPxNode
{
public:
                      Compare();
             ~Compare() override;

    MStatus   compute( const MPlug&, MDataBlock& ) override;
    SchedulingType schedulingType() const override { return SchedulingType::kParallel; }


    bool checkNurbs(const MFnNurbsSurface& a, const MFnNurbsSurface& b);
    bool checkNurbs(const MFnNurbsCurve& a, const MFnNurbsCurve& b);
    bool checkMesh(const MFnMesh& a, const MFnMesh& b);
    bool checkLattice(const MFnLattice& a, const MFnLattice& b);

    static  void *    creator();
    static  MStatus   initialize();
    static  MTypeId   id;

protected:

   static MObject  aInputA;
   static MObject  aInputB;

   static MObject  aOutValue;
};

//
// DESCRIPTION: node classification
///////////////////////////////////////////////////////

MTypeId Compare::id( 0x58000880 );

//
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////

MObject  Compare::aInputA;
MObject  Compare::aInputB;
MObject  Compare::aOutValue;


//
// DESCRIPTION:
///////////////////////////////////////////////////////
Compare::Compare()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
Compare::~Compare()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void* Compare::creator()
{
    return new Compare();
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus Compare::initialize()
{
    MFnNumericAttribute nAttr;
    MFnCompoundAttribute   cAttr;
	MFnTypedAttribute tAttr;
    MStatus stat;
	aInputA = tAttr.create( "inputA", "inA", MFnData::kAny, MObject::kNullObj, &stat );
	if (!stat) { stat.perror("create attribute inputA"); return stat; }


    aInputB = tAttr.create( "inputB", "inB", MFnData::kAny, MObject::kNullObj, &stat );
	if (!stat) { stat.perror("create attribute inputB"); return stat; }

// Outputs

    aOutValue = nAttr.create( "outValue", "ov", MFnNumericData::kBoolean);
    CHECK_MSTATUS ( nAttr.setHidden(false) );
    CHECK_MSTATUS ( nAttr.setReadable(true) );
    CHECK_MSTATUS ( nAttr.setWritable(false) );

    CHECK_MSTATUS ( addAttribute(aInputA) );
    CHECK_MSTATUS ( addAttribute(aInputB) );
    CHECK_MSTATUS ( addAttribute(aOutValue) );

    CHECK_MSTATUS ( attributeAffects (aInputA, aOutValue) );
    CHECK_MSTATUS ( attributeAffects (aInputB, aOutValue) );
    return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus Compare::compute(
const MPlug&      plug,
      MDataBlock& block )
{
  if( plug == aOutValue )
  {
    auto handleA = block.inputValue( aInputA );
    auto handleB = block.inputValue( aInputB );
    bool same {false};

    MObject curveA = handleA.asNurbsCurve();

    if(handleA.type() == handleB.type())
    {
        if(handleA.type() == MFnData::kMesh)
        {
            MFnMesh a(block.inputValue( aInputA ).asMesh());
            MFnMesh b(block.inputValue( aInputB ).asMesh());
            same = checkMesh(a, b);
        }
        else if(handleA.type() == MFnData::kLattice)
        {
                MFnLatticeData a(block.inputValue(aInputA).data());
                MFnLatticeData b(block.inputValue(aInputB).data());
                if (!a.lattice().isNull() && !b.lattice().isNull())
                    same = checkLattice(a.lattice(), b.lattice());

        }
        else if(!curveA.isNull())
        {
                MFnNurbsCurve a(block.inputValue(aInputA).data());
                MFnNurbsCurve b(block.inputValue(aInputB).data());
                same = checkNurbs(a, b);
        }
        else if(handleA.type() == MFnData::kNurbsSurface)
        {
                MFnNurbsSurface a(block.inputValue(aInputA).data());
                MFnNurbsSurface b(block.inputValue(aInputB).data());
                same = checkNurbs(a, b);
        }
        else if(handleA.type() == MFnData::kMatrix)
        {
            MFnMatrixData a(block.inputValue(aInputA).data());
            MFnMatrixData b(block.inputValue(aInputB).data());
            if(!a.object().isNull() && !b.object().isNull())
                same = a.matrix().isEquivalent(b.matrix(), kFloatEpsilon);
        }
    }

    // set ouput color attribute
    MDataHandle outHandle = block.outputValue( aOutValue );
	outHandle.set(same);

  } else
	  return MS::kUnknownParameter;

  return MS::kSuccess;
}


bool Compare::checkNurbs(const MFnNurbsCurve& aCurve, const MFnNurbsCurve& bCurve)
{
    if (aCurve.numCVs() != bCurve.numCVs()) return false;
    MPointArray aCVs, bCVs;
    aCurve.getCVs(aCVs, MSpace::kWorld);
    bCurve.getCVs(bCVs, MSpace::kWorld);
    if(aCVs.length() != bCVs.length())return false;
    for(int i = 0; i < aCVs.length(); ++i){
        if(!aCVs[i].isEquivalent(bCVs[i], kFloatEpsilon)) return false;
    }
    MDoubleArray aKnots, bKnots;
    aCurve.getKnots(aKnots);
    bCurve.getKnots(bKnots);
    if(aKnots.length() != bKnots.length())return false;
    for(int i = 0; i < aKnots.length(); ++i){
        if(std::fabs(aKnots[i] - bKnots[i]) > kFloatEpsilon) return false;
    }
    return true;
}


bool Compare::checkNurbs(const MFnNurbsSurface& aSurface, const MFnNurbsSurface& bSurface)
{
    if(aSurface.numPatchesInU() != bSurface.numPatchesInU()) return false;
    if(aSurface.numPatchesInV() != bSurface.numPatchesInV()) return false;
    MPointArray aCVs, bCVs;
    aSurface.getCVs(aCVs);
    bSurface.getCVs(bCVs);
    if(aCVs.length() != bCVs.length())return false;
    for(int i = 0; i < aCVs.length(); ++i){
        if(!aCVs[i].isEquivalent(bCVs[i], kFloatEpsilon)) return false;
    }
    MDoubleArray aKnots, bKnots;
    aSurface.getKnotsInU(aKnots);
    bSurface.getKnotsInU(bKnots);
    if(aKnots.length() != bKnots.length())return false;
    for(int i = 0; i < aKnots.length(); ++i){
        if(std::fabs(aKnots[i] - bKnots[i]) > kFloatEpsilon) return false;
    }
    return true;
}

bool Compare::checkLattice(const MFnLattice& aLattice, const MFnLattice& bLattice)
{
    unsigned int sa,ta,ua;
    unsigned int sb,tb,ub;
    const_cast<MFnLattice&>(aLattice).getDivisions(sa,ta,ua);
    const_cast<MFnLattice&>(bLattice).getDivisions(sb,tb,ub);
    if(sa != sb || ta != tb || ua != ub)return false;

    for(unsigned int i = 0; i < sa; ++i )
        for(unsigned int j = 0; j < ta; ++j )
            for(unsigned int k = 0; k < ua; ++k ){
                const auto& pta = const_cast<MFnLattice&>(aLattice).point(i,j,k);
                const auto& ptb = const_cast<MFnLattice&>(bLattice).point(i,j,k);
                auto eps = std::max({pta[0]*kFloatEpsilon, ptb[0]*kFloatEpsilon, kFloatEpsilon});
                if(!pta.isEquivalent(ptb, eps))return false;
            }
    return true;
}

bool Compare::checkMesh(const MFnMesh& aMesh, const MFnMesh& bMesh)
{
    MPointArray a,b;
    aMesh.getPoints(a, MSpace::kWorld);
    bMesh.getPoints(b, MSpace::kWorld);
    auto c = a.length();
    if (c != b.length()) return false;

    for(unsigned int i = 0; i < c; ++i){
        auto pta = a[i];
        auto ptb = b[i];
        auto eps = std::max({pta[0]*kFloatEpsilon, ptb[0]*kFloatEpsilon, kFloatEpsilon});
        if(std::fabs(pta[0] - ptb[0]) > eps) return false;

        eps = std::max({pta[1]*kFloatEpsilon, ptb[1]*kFloatEpsilon, kFloatEpsilon});
        if(std::fabs(pta[1] - ptb[1]) > eps) return false;

        eps = std::max({pta[2]*kFloatEpsilon, ptb[2]*kFloatEpsilon, kFloatEpsilon});
        if(std::fabs(pta[2] - ptb[2]) > eps) return false;
    }

    return true;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{
   MFnPlugin plugin( obj, "Autodesk", "1.0", "Any");


   CHECK_MSTATUS ( plugin.registerNode( "compare", Compare::id,
                         Compare::creator, Compare::initialize,
                         MPxNode::kDependNode) );

   return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
   MFnPlugin plugin( obj );
   CHECK_MSTATUS ( plugin.deregisterNode( Compare::id ) );

   return MS::kSuccess;
}
// =====================================================================
// Copyright 2018 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// =====================================================================
