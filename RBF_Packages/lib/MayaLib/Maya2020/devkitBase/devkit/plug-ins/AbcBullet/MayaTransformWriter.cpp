//-*****************************************************************************
//
// Copyright (c) 2009-2012,
//  Sony Pictures Imageworks Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *	   Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *	   Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *	   Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic, nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#include "MayaTransformWriter.h"
#include "MayaUtility.h"

void addChannel( 
	std::vector < AnimChan > & oAnimChanList,
	Alembic::AbcGeom::XformSample & oSample,
	AnimChan& chan
	)
{
	chan.opNum = oSample.getNumOps();
	oAnimChanList.push_back(chan);
}

void addTranslate(const MFnDependencyNode & iTrans,
	MString parentName, MString xName, MString yName, MString zName,
	Alembic::Util::uint8_t iHint, bool inverse, bool forceStatic,
	bool forceAnimated, Alembic::AbcGeom::XformSample & oSample,
	std::vector < AnimChan > & oAnimChanList)
{
	Alembic::AbcGeom::XformOp op(Alembic::AbcGeom::kTranslateOperation, iHint);

	MPlug xPlug = iTrans.findPlug(xName);
	int xSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			xSamp = util::getSampledType(xPlug);
		else
			xSamp = 1;
	}
	double xVal = xPlug.asDouble();

	MPlug yPlug = iTrans.findPlug(yName);
	int ySamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			ySamp = util::getSampledType(yPlug);
		else
			ySamp = 1;
	}
	double yVal = yPlug.asDouble();

	MPlug zPlug = iTrans.findPlug(zName);
	int zSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			zSamp = util::getSampledType(zPlug);
		else
			zSamp = 1;
	}
	double zVal = zPlug.asDouble();

	// this is to handle the case where there is a connection to the parent
	// plug but not to the child plugs, if the connection is there then all
	// of the children are considered animated
	MPlug parentPlug = iTrans.findPlug(parentName);
	int parentSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			parentSamp = util::getSampledType(parentPlug);
		else
			parentSamp = 1;
	}

	if (parentSamp != 0)
	{
		xSamp = 1;
		ySamp = 1;
		zSamp = 1;
	}

	// something is sampled or not identity, add it to the stack
	if (xSamp != 0 || ySamp != 0 || zSamp != 0 || xVal != 0.0 || yVal != 0.0 ||
		zVal != 0.0)
	{
		if (inverse)
		{
			xVal = -xVal;
			yVal = -yVal;
			zVal = -zVal;
		}

		op.setChannelValue(0, xVal);
		op.setChannelValue(1, yVal);
		op.setChannelValue(2, zVal);

		if (xSamp != 0)
		{
			AnimChan chan;
			chan.plug = xPlug;
			chan.scale = 1.0;
			if (inverse)
				chan.scale = -1.0;
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 0;
			oAnimChanList.push_back(chan);
		}

		if (ySamp != 0)
		{
			AnimChan chan;
			chan.plug = yPlug;
			chan.scale = 1.0;
			if (inverse)
				chan.scale = -1.0;
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 1;
			oAnimChanList.push_back(chan);
		}

		if (zSamp != 0)
		{
			AnimChan chan;
			chan.plug = zPlug;
			chan.scale = 1.0;
			if (inverse)
				chan.scale = -1.0;
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 2;
			oAnimChanList.push_back(chan);
		}

		oSample.addOp(op);
	}
}

// names need to be passed in x,y,z order, iOrder is the order to
// use these indices
void addRotate(const MFnDependencyNode & iTrans,
	MString parentName, const MString* iNames, const unsigned int* iOrder,
	Alembic::Util::uint8_t iHint, bool forceStatic, bool forceAnimated,
	Alembic::AbcGeom::XformSample & oSample,
	std::vector < AnimChan > & oAnimChanList,
	size_t oOpIndex[3])
{
	// for each possible rotation axis
	static const Alembic::AbcGeom::XformOperationType rots[3] = {
		 Alembic::AbcGeom::kRotateXOperation,
		 Alembic::AbcGeom::kRotateYOperation,
		 Alembic::AbcGeom::kRotateZOperation
	};

	// this is to handle the case where there is a connection to the parent
	// plug but not to the child plugs, if the connection is there then all
	// of the children are considered animated
	MPlug parentPlug = iTrans.findPlug(parentName);
	int parentSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			parentSamp = util::getSampledType(parentPlug);
		else
			parentSamp = 1;
	}

	// whether we are using the XYZ rotation order
	bool isXYZ = ((iOrder[0] == 0) && (iOrder[1] == 1) && (iOrder[2] == 2));

	// add them in backwards since we are dealing with a stack
	int i = 2;
	for (; i > -1; i--)
	{
		unsigned int index = iOrder[i];
		MPlug plug = iTrans.findPlug(iNames[index]);
		int samp = 0;
		if (!forceStatic)
		{
			if (!forceAnimated)
				samp = util::getSampledType(plug);
			else
				samp = 1;

			if (samp == 0)
				samp = parentSamp;
		}

		double plugVal = plug.asDouble();


		Alembic::AbcGeom::XformOp op(rots[index], iHint);
		op.setChannelValue(0, Alembic::AbcGeom::RadiansToDegrees(plugVal));

		// the sampled case
		if (samp != 0)
		{
			AnimChan chan;
			chan.plug = plug;
			chan.scale = Alembic::AbcGeom::RadiansToDegrees(1.0);
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 0;
			oAnimChanList.push_back(chan);
		}
		// non sampled, XYZ axis and the angle is 0, do not add to the stack
		else if (isXYZ && plugVal == 0.0)
			continue;

		oOpIndex[index] = oSample.addOp(op);
	}
}

// the test on whether or not to add it is very similiar to addTranslate
// but the operation it creates is very different
void addShear(const MFnDependencyNode & iTrans, bool forceStatic,
	Alembic::AbcGeom::XformSample & oSample,
	std::vector < AnimChan > & oAnimChanList)
{
	Alembic::AbcGeom::XformOp op(Alembic::AbcGeom::kMatrixOperation,
		Alembic::AbcGeom::kMayaShearHint);

	MString str = "shearXY";
	MPlug xyPlug = iTrans.findPlug(str);
	int xySamp = 0;
	if (!forceStatic)
	{
		xySamp = util::getSampledType(xyPlug);
	}
	double xyVal = xyPlug.asDouble();

	str = "shearXZ";
	MPlug xzPlug = iTrans.findPlug(str);
	int xzSamp = 0;
	if (!forceStatic)
	{
		xzSamp = util::getSampledType(xzPlug);
	}
	double xzVal = xzPlug.asDouble();

	str = "shearYZ";
	MPlug yzPlug = iTrans.findPlug(str);
	int yzSamp = 0;
	if (!forceStatic)
	{
		yzSamp = util::getSampledType(yzPlug);
	}
	double yzVal = yzPlug.asDouble();

	// this is to handle the case where there is a connection to the parent
	// plug but not to the child plugs, if the connection is there then all
	// of the children are considered animated
	str = "shear";
	MPlug parentPlug = iTrans.findPlug(str);
	if (!forceStatic && util::getSampledType(parentPlug) != 0)
	{
		xySamp = 1;
		xzSamp = 1;
		yzSamp = 1;
	}

	// something is sampled or not identity, add it to the stack
	if (xySamp != 0 || xzSamp != 0 || yzSamp != 0 ||
		xyVal != 0.0 || xzVal != 0.0 || yzVal != 0.0)
	{
		Alembic::Abc::M44d m;
		m.makeIdentity();
		op.setMatrix(m);
		op.setChannelValue(4, xyVal);
		op.setChannelValue(8, xzVal);
		op.setChannelValue(9, yzVal);

		if (xySamp != 0)
		{
			AnimChan chan;
			chan.plug = xyPlug;
			chan.scale = 1.0;
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 4;
			oAnimChanList.push_back(chan);
		}

		if (xzSamp != 0)
		{
			AnimChan chan;
			chan.plug = xzPlug;
			chan.scale = 1.0;
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 8;
			oAnimChanList.push_back(chan);
		}

		if (yzSamp != 0)
		{
			AnimChan chan;
			chan.plug = yzPlug;
			chan.scale = 1.0;
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 9;
			oAnimChanList.push_back(chan);
		}

		oSample.addOp(op);
	}
}

// this test is very similiar to addTranslate, except that it doesn't add it
// to the stack if x,y, and z are 1.0
void addScale(const MFnDependencyNode & iTrans,
	MString parentName, MString xName, MString yName, MString zName, bool inverse,
	bool forceStatic, bool forceAnimated, Alembic::AbcGeom::XformSample & oSample,
	std::vector < AnimChan > & oAnimChanList)
{

	Alembic::AbcGeom::XformOp op(Alembic::AbcGeom::kScaleOperation,
		Alembic::AbcGeom::kScaleHint);

	MPlug xPlug = iTrans.findPlug(xName);
	int xSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			xSamp = util::getSampledType(xPlug);
		else
			xSamp = 1;
	}
	double xVal = xPlug.asDouble();

	MPlug yPlug = iTrans.findPlug(yName);
	int ySamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			ySamp = util::getSampledType(yPlug);
		else
			ySamp = 1;
	}
	double yVal = yPlug.asDouble();

	MPlug zPlug = iTrans.findPlug(zName);
	int zSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			zSamp = util::getSampledType(zPlug);
		else
			zSamp = 1;
	}
	double zVal = zPlug.asDouble();

	// this is to handle the case where there is a connection to the parent
	// plug but not to the child plugs, if the connection is there then all
	// of the children are considered animated
	MPlug parentPlug = iTrans.findPlug(parentName);
	int parentSamp = 0;
	if (!forceStatic)
	{
		if (!forceAnimated)
			parentSamp = util::getSampledType(parentPlug);
		else
			parentSamp = 1;
	}

	if (parentSamp != 0)
	{
		xSamp = 1;
		ySamp = 1;
		zSamp = 1;
	}

	// something is sampled or not identity, add it to the stack
	if (xSamp != 0 || ySamp != 0 || zSamp != 0 || xVal != 1.0 || yVal != 1.0 ||
		zVal != 1.0)
	{
		if (inverse)
		{
			xVal = util::inverseScale(xVal);
			yVal = util::inverseScale(yVal);
			zVal = util::inverseScale(zVal);
		}

		op.setChannelValue(0, xVal);
		op.setChannelValue(1, yVal);
		op.setChannelValue(2, zVal);

		if (xSamp != 0)
		{
			AnimChan chan;
			chan.plug = xPlug;
			chan.scale = 1.0;
			if (inverse)
				chan.scale = -std::numeric_limits<double>::infinity();
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 0;
			oAnimChanList.push_back(chan);
		}

		if (ySamp != 0)
		{
			AnimChan chan;
			chan.plug = yPlug;
			chan.scale = 1.0;
			if (inverse)
				chan.scale = -std::numeric_limits<double>::infinity();
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 1;
			oAnimChanList.push_back(chan);
		}

		if (zSamp != 0)
		{
			AnimChan chan;
			chan.plug = zPlug;
			chan.scale = 1.0;
			if (inverse)
				chan.scale = -std::numeric_limits<double>::infinity();
			chan.opNum = oSample.getNumOps();
			chan.channelNum = 2;
			oAnimChanList.push_back(chan);
		}

		oSample.addOp(op);
	}
}

bool getSampledRotation(const Alembic::AbcGeom::XformSample& sample,
	const size_t opIndex[3], double& xx, double& yy, double& zz)
{
	bool success = false;

	xx = 0.0;
	if (opIndex[0] < sample.getNumOps())
	{
		double angleX = sample[opIndex[0]].getChannelValue(0);
		xx = Alembic::AbcGeom::DegreesToRadians(angleX);
		success = true;
	}

	yy = 0.0;
	if (opIndex[1] < sample.getNumOps())
	{
		double angleY = sample[opIndex[1]].getChannelValue(0);
		yy = Alembic::AbcGeom::DegreesToRadians(angleY);
		success = true;
	}

	zz = 0.0;
	if (opIndex[2] < sample.getNumOps())
	{
		double angleZ = sample[opIndex[2]].getChannelValue(0);
		zz = Alembic::AbcGeom::DegreesToRadians(angleZ);
		success = true;
	}

	return success;
}

bool setSampledRotation(Alembic::AbcGeom::XformSample& sample,
	const size_t opIndex[3], double xx, double yy, double zz)
{
	bool success = false;

	if (opIndex[0] < sample.getNumOps())
	{
		sample[opIndex[0]].setChannelValue(0, Alembic::AbcGeom::RadiansToDegrees(xx));
		success = true;
	}

	if (opIndex[1] < sample.getNumOps())
	{
		sample[opIndex[1]].setChannelValue(0, Alembic::AbcGeom::RadiansToDegrees(yy));
		success = true;
	}

	if (opIndex[2] < sample.getNumOps())
	{
		sample[opIndex[2]].setChannelValue(0, Alembic::AbcGeom::RadiansToDegrees(zz));
		success = true;
	}

	return success;
}

MayaTransformWriter::MayaTransformWriter(Alembic::AbcGeom::OObject & iParent,
	MDagPath & iDag, Alembic::Util::uint32_t iTimeIndex, const JobArgs & iArgs)
{
	mVerbose = iArgs.verbose;
	mFilterEulerRotations = iArgs.filterEulerRotations;
	mJointOrientOpIndex[0] = mJointOrientOpIndex[1] = mJointOrientOpIndex[2] =
	mRotateOpIndex[0]	  = mRotateOpIndex[1]	  = mRotateOpIndex[2]	  =
	mRotateAxisOpIndex[0]  = mRotateAxisOpIndex[1]  = mRotateAxisOpIndex[2]  = ~size_t(0);

	if (iDag.hasFn(MFn::kJoint))
	{
		MFnIkJoint joint(iDag);
		MString jointName = joint.name();

		mName = util::stripNamespaces(jointName, iArgs.stripNamespace);

		Alembic::AbcGeom::OXform obj(iParent, mName.asChar(),
			iTimeIndex);
		mSchema = obj.getSchema();

		Alembic::Abc::OCompoundProperty cp;
		Alembic::Abc::OCompoundProperty up;
		if (AttributesWriter::hasAnyAttr(joint, iArgs))
		{
			cp = mSchema.getArbGeomParams();
			up = mSchema.getUserProperties();
		}

		mAttrs = AttributesWriterPtr(new AttributesWriter(cp, up, obj, joint,
			iTimeIndex, iArgs));

		if (!iArgs.worldSpace)
		{
			pushTransformStack(joint, iTimeIndex == 0);

			// need to look at inheritsTransform
			MFnDagNode dagNode(iDag);
			MPlug inheritPlug = dagNode.findPlug("inheritsTransform");
			if (!inheritPlug.isNull())
			{
				if (util::getSampledType(inheritPlug) != 0)
					mInheritsPlug = inheritPlug;

				mSample.setInheritsXforms(inheritPlug.asBool());
			}

			// everything is default, don't write anything
			if (mSample.getNumOps() == 0 && mSample.getInheritsXforms())
				return;

			mSchema.set(mSample);

			return;
		}
	}
	else
	{
		MFnTransform trans(iDag);
		MString transName = trans.name();

		mName = util::stripNamespaces(transName, iArgs.stripNamespace);

		Alembic::AbcGeom::OXform obj(iParent, mName.asChar(),
			iTimeIndex);
		mSchema = obj.getSchema();

		Alembic::Abc::OCompoundProperty cp;
		Alembic::Abc::OCompoundProperty up;
		if (AttributesWriter::hasAnyAttr(trans, iArgs))
		{
			cp = mSchema.getArbGeomParams();
			up = mSchema.getUserProperties();
		}

		mAttrs = AttributesWriterPtr(new AttributesWriter(cp, up, obj, trans,
			iTimeIndex, iArgs));

		if (!iArgs.worldSpace)
		{
			pushTransformStack(trans, iTimeIndex == 0);

			// need to look at inheritsTransform
			MFnDagNode dagNode(iDag);
			MPlug inheritPlug = dagNode.findPlug("inheritsTransform");
			if (!inheritPlug.isNull())
			{
				if (util::getSampledType(inheritPlug) != 0)
					mInheritsPlug = inheritPlug;

				mSample.setInheritsXforms(inheritPlug.asBool());
			}


			// everything is default, don't write anything
			if (mSample.getNumOps() == 0 && mSample.getInheritsXforms())
				return;

			mSchema.set(mSample);
			return;
		}
	}

	// if we didn't bail early then we need to add all the transform
	// information at the current node and above

	// copy the dag path because we'll be popping from it
	MDagPath dag(iDag);

	int i;
	int numPaths = dag.length();
	std::vector< MDagPath > dagList;
	for (i = numPaths - 1; i > -1; i--, dag.pop())
	{
		dagList.push_back(dag);

		// inheritsTransform exists on both joints and transforms
		MFnDagNode dagNode(dag);
		MPlug inheritPlug = dagNode.findPlug("inheritsTransform");

		// if inheritsTransform exists and is set to false, then we
		// don't need to worry about ancestor nodes above this one
		if (!inheritPlug.isNull() && !inheritPlug.asBool())
			break;
	}


	std::vector< MDagPath >::iterator iStart = dagList.begin();

	std::vector< MDagPath >::iterator iCur = dagList.end();
	iCur--;

	// now loop backwards over our dagpath list so we push ancestor nodes
	// first, all the way down to the current node
	for (; iCur != iStart; iCur--)
	{
		// only add it to the stack don't write it yet!

		if (iCur->hasFn(MFn::kJoint))
		{
			MFnIkJoint joint(*iCur);
			pushTransformStack(joint, iTimeIndex == 0);
		}
		else
		{
			MFnTransform trans(*iCur);
			pushTransformStack(trans, iTimeIndex == 0);
		}
	}

	// finally add any transform info on the final node and write it
	if (iCur->hasFn(MFn::kJoint))
	{
		MFnIkJoint joint(*iCur);
		pushTransformStack(joint, iTimeIndex == 0);
	}
	else
	{
		MFnTransform trans(*iCur);
		pushTransformStack(trans, iTimeIndex == 0);
	}

	// need to look at inheritsTransform
	MFnDagNode dagNode(iDag);
	MPlug inheritPlug = dagNode.findPlug("inheritsTransform");
	if (!inheritPlug.isNull())
	{
		if (util::getSampledType(inheritPlug) != 0)
			mInheritsPlug = inheritPlug;

		mSample.setInheritsXforms(inheritPlug.asBool());
	}


	// everything is default, don't write anything
	if (mSample.getNumOps() == 0 && mSample.getInheritsXforms())
		return;

	mSchema.set(mSample);

}

MayaTransformWriter::MayaTransformWriter(MayaTransformWriter & iParent,
	MDagPath & iDag, Alembic::Util::uint32_t iTimeIndex, const JobArgs & iArgs)
{
	mVerbose = iArgs.verbose;
	mFilterEulerRotations = iArgs.filterEulerRotations;
	mJointOrientOpIndex[0] = mJointOrientOpIndex[1] = mJointOrientOpIndex[2] =
	mRotateOpIndex[0]	  = mRotateOpIndex[1]	  = mRotateOpIndex[2]	  =
	mRotateAxisOpIndex[0]  = mRotateAxisOpIndex[1]  = mRotateAxisOpIndex[2]  = ~size_t(0);

	if (iDag.hasFn(MFn::kJoint))
	{
		MFnIkJoint joint(iDag);
		MString jointName = joint.name();

		mName = util::stripNamespaces(jointName, iArgs.stripNamespace);

		Alembic::AbcGeom::OXform obj(iParent.getObject(), mName.asChar(),
			iTimeIndex);
		mSchema = obj.getSchema();

		Alembic::Abc::OCompoundProperty cp;
		Alembic::Abc::OCompoundProperty up;
		if (AttributesWriter::hasAnyAttr(joint, iArgs))
		{
			cp = mSchema.getArbGeomParams();
			up = mSchema.getUserProperties();
		}

		mAttrs = AttributesWriterPtr(new AttributesWriter(cp, up, obj, joint,
			iTimeIndex, iArgs));

		pushTransformStack(joint, iTimeIndex == 0);
	}
	else
	{
		MFnTransform trans(iDag);
		MString transName = trans.name();

		mName = util::stripNamespaces(transName, iArgs.stripNamespace);

		Alembic::AbcGeom::OXform obj(iParent.getObject(), mName.asChar(),
			iTimeIndex);
		mSchema = obj.getSchema();

		Alembic::Abc::OCompoundProperty cp;
		Alembic::Abc::OCompoundProperty up;
		if (AttributesWriter::hasAnyAttr(trans, iArgs))
		{
			cp = mSchema.getArbGeomParams();
			up = mSchema.getUserProperties();
		}

		mAttrs = AttributesWriterPtr(new AttributesWriter(cp, up, obj, trans,
			iTimeIndex, iArgs));

		pushTransformStack(trans, iTimeIndex == 0);
	}


	// need to look at inheritsTransform
	MFnDagNode dagNode(iDag);
	MPlug inheritPlug = dagNode.findPlug("inheritsTransform");
	if (!inheritPlug.isNull())
	{
		if (util::getSampledType(inheritPlug) != 0)
			mInheritsPlug = inheritPlug;

		mSample.setInheritsXforms(inheritPlug.asBool());
	}

	// everything is default, don't write anything
	if (mSample.getNumOps() == 0 && mSample.getInheritsXforms())
		return;

	mSchema.set(mSample);
}

MayaTransformWriter::MayaTransformWriter(Alembic::Abc::OObject & iParent, 
	const AnimSampler& sampler, 
	Alembic::Util::uint32_t iTimeIndex, const JobArgs & iArgs)
{
	mVerbose = iArgs.verbose;
	mFilterEulerRotations = iArgs.filterEulerRotations;
	mJointOrientOpIndex[0] = mJointOrientOpIndex[1] = mJointOrientOpIndex[2] =
	mRotateOpIndex[0]	  = mRotateOpIndex[1]	  = mRotateOpIndex[2]	  =
	mRotateAxisOpIndex[0]  = mRotateAxisOpIndex[1]  = mRotateAxisOpIndex[2]  = ~size_t(0);

	MString name = sampler.name();

	mName = util::stripNamespaces(name, iArgs.stripNamespace);

	Alembic::AbcGeom::OXform obj(iParent, mName.asChar(),
		iTimeIndex);
	mSchema = obj.getSchema();

	// no custom or user attributes
	
	// determine channels to write
	pushTransformStack(sampler, iTimeIndex == 0);

	// everything is default, don't write anything
	if (mSample.getNumOps() == 0 && mSample.getInheritsXforms())
		return;

	mSchema.set(mSample);
}

MayaTransformWriter::~MayaTransformWriter()
{
}

void MayaTransformWriter::write()
{
	size_t numSamples = mAnimChanList.size();
	if (numSamples > 0)
	{
		std::vector < AnimChan >::iterator it, itEnd;

		for (it = mAnimChanList.begin(), itEnd = mAnimChanList.end();
			it != itEnd; ++it)
		{
			double val = it->asDouble();

			if (it->scale == -std::numeric_limits<double>::infinity())
				val = util::inverseScale(val);
			else if (it->scale != 1.0)
				val *= it->scale;

			mSample[it->opNum].setChannelValue(it->channelNum, val);
		}

		if (!mInheritsPlug.isNull())
		{
			mSample.setInheritsXforms(mInheritsPlug.asBool());
		}

		if (mFilterEulerRotations)
		{
			double xx(0), yy(0), zz(0);

			if (getSampledRotation(mSample, mJointOrientOpIndex, xx, yy, zz))
			{
				MEulerRotation filteredEuler(xx, yy, zz, mPrevJointOrientSolution.order);
				filteredEuler.setToClosestSolution(mPrevJointOrientSolution);

				// update sample with new solution
				setSampledRotation(mSample, mJointOrientOpIndex, filteredEuler.x, filteredEuler.y, filteredEuler.z);
				mPrevJointOrientSolution = filteredEuler;
			}

			if (getSampledRotation(mSample, mRotateOpIndex, xx, yy, zz))
			{
				MEulerRotation filteredEuler(xx, yy, zz, mPrevRotateSolution.order);
				filteredEuler.setToClosestSolution(mPrevRotateSolution);

#ifdef _DEBUG
				if (mVerbose)
				{
					MString str;
					MStringArray args(7,"");

					unsigned int i = 0;

					i=0;
					args[i++] = mName;
					args[i++].set(mPrevRotateSolution[0]);
					args[i++].set(mPrevRotateSolution[1]);
					args[i++].set(mPrevRotateSolution[2]);

					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(mPrevRotateSolution[0]));
					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(mPrevRotateSolution[1]));
					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(mPrevRotateSolution[2]));

					str.format("^1s: previous rotation radians(^2s, ^3s, ^4s) degrees(^5s, ^6s, ^7s)",args);
					MGlobal::displayInfo( str );

					MEulerRotation currentEuler(xx, yy, zz, mPrevRotateSolution.order);

					i=0;
					args[i++] = mName;
					args[i++].set(currentEuler[0]);
					args[i++].set(currentEuler[1]);
					args[i++].set(currentEuler[2]);

					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(currentEuler[0]));
					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(currentEuler[1]));
					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(currentEuler[2]));

					str.format("^1s: current rotation radians(^2s, ^3s, ^4s) degrees(^5s, ^6s, ^7s)",args);
					MGlobal::displayInfo( str );

					i=0;
					args[i++] = mName;
					args[i++].set(filteredEuler[0]);
					args[i++].set(filteredEuler[1]);
					args[i++].set(filteredEuler[2]);

					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(filteredEuler[0]));
					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(filteredEuler[1]));
					args[i++].set(Alembic::AbcGeom::RadiansToDegrees(filteredEuler[2]));

					str.format("^1s: filtered rotation radians(^2s, ^3s, ^4s) degrees(^5s, ^6s, ^7s)",args);
					MGlobal::displayInfo( str );

				}
#endif
				// update sample with new solution
				setSampledRotation(mSample, mRotateOpIndex, filteredEuler.x, filteredEuler.y, filteredEuler.z);
				mPrevRotateSolution = filteredEuler;
			}

			if (getSampledRotation(mSample, mRotateAxisOpIndex, xx, yy, zz))
			{
				MEulerRotation filteredEuler(xx, yy, zz, mPrevRotateAxisSolution.order);
				filteredEuler.setToClosestSolution(mPrevRotateAxisSolution);

				// update sample with new solution
				setSampledRotation(mSample, mRotateAxisOpIndex, filteredEuler.x, filteredEuler.y, filteredEuler.z);
				mPrevRotateAxisSolution = filteredEuler;
			}
		}

		mSchema.set(mSample);
	}
}

bool MayaTransformWriter::isAnimated() const
{
	return mAnimChanList.size() > 0 || !mInheritsPlug.isNull();
}

void MayaTransformWriter::pushTransformStack(const MFnTransform & iTrans,
	bool iForceStatic)
{

	// inspect the translate
	addTranslate(iTrans, "translate", "translateX", "translateY", "translateZ",
		Alembic::AbcGeom::kTranslateHint, false, iForceStatic, false, mSample,
		mAnimChanList);


	// inspect the rotate pivot translate
	addTranslate(iTrans, "rotatePivotTranslate", "rotatePivotTranslateX",
		"rotatePivotTranslateY", "rotatePivotTranslateZ",
		Alembic::AbcGeom::kRotatePivotTranslationHint, false,
			iForceStatic, false, mSample, mAnimChanList);

	// inspect the rotate pivot
	addTranslate(iTrans, "rotatePivot", "rotatePivotX", "rotatePivotY",
		"rotatePivotZ",  Alembic::AbcGeom::kRotatePivotPointHint,
		false, iForceStatic, false, mSample, mAnimChanList);

	// inspect rotate names
	MString rotateNames[3];
	rotateNames[0] = "rotateX";
	rotateNames[1] = "rotateY";
	rotateNames[2] = "rotateZ";

	unsigned int rotOrder[3];

	// if this returns false then the rotation order was kInvalid or kLast
	MTransformationMatrix::RotationOrder eRotOrder(iTrans.rotationOrder());
	if (util::getRotOrder(eRotOrder, rotOrder[0], rotOrder[1],
		rotOrder[2]))
	{
		addRotate(iTrans, "rotate", rotateNames, rotOrder,
			Alembic::AbcGeom::kRotateHint, iForceStatic, false,
			mSample, mAnimChanList, mRotateOpIndex);
	}

	// now look at the rotation orientation, aka rotate axis
	rotateNames[0] = "rotateAxisX";
	rotateNames[1] = "rotateAxisY";
	rotateNames[2] = "rotateAxisZ";
	rotOrder[0] = 0;
	rotOrder[1] = 1;
	rotOrder[2] = 2;
	addRotate(iTrans, "rotateAxis", rotateNames, rotOrder,
		Alembic::AbcGeom::kRotateOrientationHint, iForceStatic, false,
		mSample, mAnimChanList, mRotateAxisOpIndex);

	// invert the rotate pivot if necessary
	addTranslate(iTrans, "rotatePivot", "rotatePivotX", "rotatePivotY",
		"rotatePivotZ", Alembic::AbcGeom::kRotatePivotPointHint,
		true, iForceStatic, false, mSample, mAnimChanList);

	// inspect the scale pivot translation
	addTranslate(iTrans, "scalePivotTranslate", "scalePivotTranslateX",
		"scalePivotTranslateY", "scalePivotTranslateZ",
		Alembic::AbcGeom::kScalePivotTranslationHint, false, iForceStatic,
		false, mSample, mAnimChanList);

	// inspect the scale pivot point
	addTranslate(iTrans, "scalePivot", "scalePivotX", "scalePivotY",
		"scalePivotZ", Alembic::AbcGeom::kScalePivotPointHint, false,
		iForceStatic, false, mSample, mAnimChanList);

	// inspect the shear
	addShear(iTrans, iForceStatic, mSample, mAnimChanList);

	// add the scale
	addScale(iTrans, "scale", "scaleX", "scaleY", "scaleZ", false,
		iForceStatic, false, mSample, mAnimChanList);

	// inverse the scale pivot point if necessary
	addTranslate(iTrans, "scalePivot", "scalePivotX", "scalePivotY",
		"scalePivotZ", Alembic::AbcGeom::kScalePivotPointHint, true,
		iForceStatic, false, mSample, mAnimChanList);

	// remember current rotation
	if (mFilterEulerRotations)
	{
		double xx(0), yy(0), zz(0);

		// there are 2 rotation order enum definitions:
		//	 MEulerRotation::RotationOrder = MTransformationMatrix::RotationOrder-1
		if (getSampledRotation( mSample, mRotateOpIndex, xx, yy, zz ))
		{
			mPrevRotateSolution.setValue(xx, yy, zz, (MEulerRotation::RotationOrder)(eRotOrder-1));
		}

		if (getSampledRotation( mSample, mRotateAxisOpIndex, xx, yy, zz ))
		{
			mPrevRotateAxisSolution.setValue(xx, yy, zz, MEulerRotation::kXYZ);
		}
	}

}

void MayaTransformWriter::pushTransformStack(const MFnIkJoint & iJoint,
	bool iForceStatic)
{
	// check joints that are driven by Maya FBIK
	// Maya FBIK has no connection to joints' TRS plugs
	// but TRS of joints are driven by FBIK, they are not static
	// Maya 2012's new HumanIK has connections to joints.
	// FBIK is a special case.
	bool forceAnimated = false;
	MStatus status = MS::kSuccess;
	if (iJoint.hikJointName(&status).length() > 0 && status) {
		forceAnimated = true;
	}

	// inspect the translate
	addTranslate(iJoint, "translate", "translateX", "translateY", "translateZ",
		Alembic::AbcGeom::kTranslateHint, false, iForceStatic, forceAnimated,
		mSample, mAnimChanList);

	// inspect the inverseParent scale
	// [IS] is ignored when Segment Scale Compensate is false
	MPlug scaleCompensatePlug = iJoint.findPlug("segmentScaleCompensate");
	if (scaleCompensatePlug.asBool())
	{
		addScale(iJoint, "inverseScale", "inverseScaleX", "inverseScaleY",
			"inverseScaleZ", true, iForceStatic, forceAnimated, mSample, mAnimChanList);
	}

	MTransformationMatrix::RotationOrder eJointOrientOrder, eRotOrder, eRotateAxisOrder;
	double vals[3];

	// for reordering rotate names
	MString rotateNames[3];
	unsigned int rotOrder[3];

	// now look at the joint orientation
	rotateNames[0] = "jointOrientX";
	rotateNames[1] = "jointOrientY";
	rotateNames[2] = "jointOrientZ";

	iJoint.getOrientation(vals, eJointOrientOrder);
	if (util::getRotOrder(eJointOrientOrder, rotOrder[0], rotOrder[1], rotOrder[2]))
	{
		addRotate(iJoint, "jointOrient", rotateNames, rotOrder,
			Alembic::AbcGeom::kRotateHint, iForceStatic, true,
			mSample, mAnimChanList, mJointOrientOpIndex);
	}

	rotateNames[0] = "rotateX";
	rotateNames[1] = "rotateY";
	rotateNames[2] = "rotateZ";

	// if this returns false then the rotation order was kInvalid or kLast
	eRotOrder = iJoint.rotationOrder();
	if (util::getRotOrder(eRotOrder, rotOrder[0], rotOrder[1],
		rotOrder[2]))
	{
		addRotate(iJoint, "rotate", rotateNames, rotOrder,
			Alembic::AbcGeom::kRotateHint, iForceStatic, true,
			mSample, mAnimChanList, mRotateOpIndex);
	}

	// now look at the rotation orientation, aka rotate axis
	rotateNames[0] = "rotateAxisX";
	rotateNames[1] = "rotateAxisY";
	rotateNames[2] = "rotateAxisZ";

	iJoint.getScaleOrientation(vals, eRotateAxisOrder);
	if (util::getRotOrder(eRotateAxisOrder, rotOrder[0], rotOrder[1], rotOrder[2]))
	{
		addRotate(iJoint, "rotateAxis", rotateNames, rotOrder,
			Alembic::AbcGeom::kRotateOrientationHint, iForceStatic, true,
			mSample, mAnimChanList, mRotateAxisOpIndex);
	}

	// inspect the scale
	addScale(iJoint, "scale", "scaleX", "scaleY", "scaleZ", false,
		iForceStatic, forceAnimated, mSample, mAnimChanList);

	// remember current rotation
	if (mFilterEulerRotations)
	{
		double xx(0), yy(0), zz(0);

		// there are 2 rotation order enum definitions:
		//	 MEulerRotation::RotationOrder = MTransformationMatrix::RotationOrder-1
		if (getSampledRotation( mSample, mJointOrientOpIndex, xx, yy, zz ))
		{
			mPrevJointOrientSolution.setValue(xx, yy, zz, (MEulerRotation::RotationOrder)(eJointOrientOrder-1));
		}

		if (getSampledRotation( mSample, mRotateOpIndex, xx, yy, zz ))
		{
			mPrevRotateSolution.setValue(xx, yy, zz, (MEulerRotation::RotationOrder)(eRotOrder-1));
		}

		if (getSampledRotation( mSample, mRotateAxisOpIndex, xx, yy, zz ))
		{
			mPrevRotateAxisSolution.setValue(xx, yy, zz, (MEulerRotation::RotationOrder)(eRotateAxisOrder-1));
		}
	}
}
void MayaTransformWriter::pushTransformStack(const AnimSampler & sampler,
	bool iForceStatic)
{
	// inspect the translate
	{
		Alembic::Util::uint8_t iHint = Alembic::AbcGeom::kTranslateHint;

		Alembic::AbcGeom::XformOp op(Alembic::AbcGeom::kTranslateOperation, iHint);

		AnimChan xChan = sampler.animChan(Alembic::AbcGeom::kTranslateOperation,kChannel0);
		AnimChan yChan = sampler.animChan(Alembic::AbcGeom::kTranslateOperation,kChannel1);
		AnimChan zChan = sampler.animChan(Alembic::AbcGeom::kTranslateOperation,kChannel2);

		op.setChannelValue(kChannel0, xChan.asDouble());
		op.setChannelValue(kChannel1, yChan.asDouble());
		op.setChannelValue(kChannel2, zChan.asDouble());

		addChannel( mAnimChanList, mSample, xChan );
		addChannel( mAnimChanList, mSample, yChan );
		addChannel( mAnimChanList, mSample, zChan );

		mSample.addOp(op);
	}

	// TODO: inspect the rotate pivot translate

	// TODO: inspect the rotate pivot

	// TODO: inspect rotate names
	MTransformationMatrix::RotationOrder eRotOrder = MTransformationMatrix::kXYZ;
	unsigned int rotOrder[3];

	if (util::getRotOrder(eRotOrder, rotOrder[0], rotOrder[1], rotOrder[2]))
	{
		Alembic::Util::uint8_t iHint = Alembic::AbcGeom::kRotateHint;

		Alembic::AbcGeom::XformOp opZ(Alembic::AbcGeom::kRotateZOperation, iHint);
		Alembic::AbcGeom::XformOp opY(Alembic::AbcGeom::kRotateYOperation, iHint);
		Alembic::AbcGeom::XformOp opX(Alembic::AbcGeom::kRotateXOperation, iHint);

		double scale = Alembic::AbcGeom::RadiansToDegrees(1.0);

		AnimChan zChan = sampler.animChan(Alembic::AbcGeom::kRotateZOperation, kChannel0, scale);
		AnimChan yChan = sampler.animChan(Alembic::AbcGeom::kRotateYOperation, kChannel0, scale);
		AnimChan xChan = sampler.animChan(Alembic::AbcGeom::kRotateXOperation, kChannel0, scale);

		// assume we're getting radians
		double valZ = zChan.asDouble();
		double valY = yChan.asDouble();
		double valX = xChan.asDouble();

		opZ.setChannelValue(kChannel0, valZ);
		opY.setChannelValue(kChannel0, valY);
		opX.setChannelValue(kChannel0, valX);

		// we are using the XYZ rotation order
		addChannel( mAnimChanList, mSample, zChan );
		mRotateOpIndex[rotOrder[2]] = mSample.addOp(opZ);

		addChannel( mAnimChanList, mSample, yChan );
		mRotateOpIndex[rotOrder[1]] = mSample.addOp(opY);

		addChannel( mAnimChanList, mSample, xChan );
		mRotateOpIndex[rotOrder[0]] = mSample.addOp(opX);
	}

	// TODO: now look at the rotation orientation, aka rotate axis

	// TODO: invert the rotate pivot if necessary

	// TODO: inspect the scale pivot translation

	// TODO: inspect the scale pivot point

	// TODO: inspect the shear

	// TODO: add the scale

	// TODO: inverse the scale pivot point if necessary

	// remember current rotation
	if (mFilterEulerRotations)
	{
		double xx(0), yy(0), zz(0);

		// there are 2 rotation order enum definitions:
		//	 MEulerRotation::RotationOrder = MTransformationMatrix::RotationOrder-1
		if (getSampledRotation( mSample, mRotateOpIndex, xx, yy, zz ))
		{
			mPrevRotateSolution.setValue(xx, yy, zz, (MEulerRotation::RotationOrder)(eRotOrder-1));
		}

		if (getSampledRotation( mSample, mRotateAxisOpIndex, xx, yy, zz ))
		{
			mPrevRotateAxisSolution.setValue(xx, yy, zz, MEulerRotation::kXYZ);
		}
	}
}
