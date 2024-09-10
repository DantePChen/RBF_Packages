// ==========================================================================
// Copyright 2019 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================

// This plug-in demonstrates how to implement a simple simulation system with full support for Cached Playback.
//   - Using MPxNode::getCacheSetup() to trigger the 'Dynamics Cache Layer' behavior
//   - Using MPxNode::configCache() to setup attributes to cache
//   - Using MPxNode::transformInvalidationRange() to setup the correct invalidation range for dynamics system
//   - The correct way to readback cached dynamics status and support 'Simulation resumption from arbitrary time'
//
// To run this example, execute the MEL code below.

/*
```MEL
file -new -force;
loadPlugin simplePhysicsEngine;

createNode physicsEngine -name "solver";
polySphere -r 1 -name "sphere" -constructionHistory 1;
connectAttr solver.position sphere.translate;

// Create a plane for collision, and start to tilt it
polyPlane -w 50 -h 50 -sx 1 -sy 1 -name "ground";
setKeyframe -v 0 -t 1 "ground.rz";
setKeyframe -v -20 -t 120 "ground.rz";

// Convert the transform of this plane into the Ax+By+Cz+D=0 form that solver node accepts
createNode -parent "ground" -name "o" locator;
createNode -parent "ground" -name "y" locator;
setAttr "y.localPositionY" 1;
setAttr "o.visibility" 0;
setAttr "y.visibility" 0;
// ground.normal = (0,1,0,0) * matrix
createNode plusMinusAverage -n "normal";
setAttr "normal.operation" 2; // subtract
connectAttr y.worldPosition[0] normal.input3D[0];
connectAttr o.worldPosition[0] normal.input3D[1];
connectAttr -force normal.output3Dx solver.collisionPlane0;
connectAttr -force normal.output3Dy solver.collisionPlane1;
connectAttr -force normal.output3Dz solver.collisionPlane2;
// ground.offset = -dot((0,0,0,1) * matrix, ground.normal) - sphere.radius
createNode multiplyDivide -n "offset_mul";
setAttr "normal.operation" 1; // multiply
connectAttr o.worldPosition[0] offset_mul.input1;
connectAttr normal.output3D    offset_mul.input2;
createNode plusMinusAverage -n "offset";
setAttr "offset.operation" 2; // subtract
setAttr "offset.input1D[0]" 0;
connectAttr offset_mul.outputX offset.input1D[1];
connectAttr offset_mul.outputY offset.input1D[2];
connectAttr offset_mul.outputZ offset.input1D[3];
connectAttr polySphere1.r offset.input1D[4];
connectAttr offset.output1D solver.collisionPlane3;
```
*/

#include <type_traits>
#include <utility>
#include <cassert>
#include <limits>
#include <cmath>
#include <thread>

#include <maya/MPxNode.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnCompoundAttribute.h>

#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>

#include <maya/MFnPlugin.h>
#include <maya/MNodeCacheDisablingInfoHelper.h>

#include <maya/MDGMessage.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFn.h>
#include <maya/MDGModifier.h>

// Helpers for vector math
namespace {
struct MDouble3 { double x, y, z; };
struct MDouble4 { double x, y, z, w; };
MDouble3 operator+ (MDouble3 lhs, MDouble3 rhs) noexcept { return { lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z }; }
MDouble3 operator- (MDouble3 lhs, MDouble3 rhs) noexcept { return { lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z }; }
MDouble3 operator* (MDouble3 lhs, MDouble3 rhs) noexcept { return { lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z }; }
MDouble3 operator* (MDouble3 lhs, double rhs)   noexcept { return { lhs.x * rhs, lhs.y * rhs, lhs.z * rhs }; }
MDouble3 operator/ (MDouble3 lhs, MDouble3 rhs) noexcept { return { lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z }; }
MDouble3 operator/ (MDouble3 lhs, double rhs)   noexcept { return { lhs.x / rhs, lhs.y / rhs, lhs.z / rhs }; }

MDouble4 to4(MDouble3 v, double w) { return { v.x,v.y,v.z,w }; }
MDouble3 xyz(MDouble4 v) { return { v.x,v.y,v.z }; }
double   dot(MDouble4 lhs, MDouble4 rhs) noexcept { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }
double   dot(MDouble3 lhs, MDouble3 rhs) noexcept { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
MDouble3 normalized(MDouble3 v) { return v / sqrt(dot(v,v)); }
/*!
\brief Get an arbitrary point on the plane given by Ax+By+Cz+D=0
\param[in] p The plane equation coefficients (A,B,C,D)
*/
MDouble3 point_on_plane(MDouble4 p) {
	if (p.x != .0) {
		return { -p.w / p.x, .0, .0 };
	} else if (p.y != .0) {
		return { .0, -p.w / p.y, .0 };
	} else if (p.z != .0) {
		return { .0, .0, -p.w / p.z };
	}
	return { .0,.0,.0 };
};
}

// helpers for Maya attributes access
namespace{
template <typename T>
struct MAttributeOf : MObject {
	using type = T;
	using MObject::MObject;
	template <typename U>
	MAttributeOf& operator=(U&& arg) { MObject::operator=(std::forward<U>(arg)); return *this; }
};

namespace detail {
	struct error_type {}; // Placeholder type to detect usage of as<T> with not specialization 

	template <typename T>
	decltype(auto) as(const MDataHandle& handle) { return error_type{}; }

	template <> decltype(auto) as<bool>(const MDataHandle& handle) { return handle.asBool(); }
	template <> decltype(auto) as<double>(const MDataHandle& handle) { return handle.asDouble(); }
	template <> decltype(auto) as<MTime>(const MDataHandle& handle) { return handle.asTime(); }
	template <>
	decltype(auto) as<MDouble3>(const MDataHandle& handle) {
		return reinterpret_cast<MDouble3&>(handle.asDouble3());
	}
	template <>
	decltype(auto) as<MDouble4>(const MDataHandle& handle) {
		return reinterpret_cast<MDouble4&>(handle.asDouble4());
	}
	// Add more specializations for all the asXXX methods
}

template <typename T>
decltype(auto) get_input(MDataBlock& data, const MAttributeOf<T>& attr) {
	MStatus status;
	MDataHandle handle = data.inputValue(static_cast<const MObject&>(attr), &status);
	assert(status == MStatus::kSuccess);
	return ::detail::as<T>(handle);
}

template <typename T>
decltype(auto) get_as_is(MDataBlock& data, const MAttributeOf<T>& attr) {
	MStatus status;
	MDataHandle handle = data.outputValue(static_cast<const MObject&>(attr), &status);
	assert(status == MStatus::kSuccess);
	return ::detail::as<T>(handle);
}

template <typename T>
void set(MDataBlock& data, const MAttributeOf<T>& attr, const T& value) {
	MStatus status;
	MDataHandle handle = data.outputValue(static_cast<const MObject&>(attr), &status);
	assert(status == MStatus::kSuccess);
	::detail::as<T>(handle) = value; // MDataHandle.set(...) cannot handle 4 double...
	handle.setClean();
}
}

// Helpers for physics simulation
namespace physics {
	struct ObjectStatus {
		MTime    Time;
		MDouble3 Position;
		MDouble3 Velocity;
	};

	/*!
		\brief Compute a particle's new status after time `dt` with Newton's second law of motion
		\param[in] prev The existing status of the particle, i.e. (Position(t), Velocity(t))
		\param[in] dt   The time delta
		\param[in] acceleration The average acceleration in time [t, t+dt]
		\return The new status
			Position(t+dt) = Position(t) + (Velocity(t-1) + Velocity(t)) *dt / 2 ;
			Velocity(t+dt) = Velocity(t) + Acceleration(t) * dt ;
	*/
	ObjectStatus apply_velocity(const ObjectStatus& prev, MTime dt, MDouble3 acceleration) {
		double dts = dt.as(MTime::kSeconds);
		ObjectStatus now;
		now.Time = prev.Time + dt;
		now.Velocity = prev.Velocity + acceleration * dts;
		now.Position = prev.Position + (now.Velocity + prev.Velocity) * (0.5 * dts);
		return now;
	}

	/*!
		\brief  Compute the collision between the given `particle` now and the plane `ground`
		\return The particle's new position and velocity after collision with `ground`
		\param[in] particle   The active particle to collided
		\param[in] ground     The passive plane to collided with
		\param[in] elasticity The elasticity of the collision, speed_out = speed_in * elasticity
	*/
	ObjectStatus resolve_collision(ObjectStatus particle, MDouble4 ground, double elasticity) {
		assert(elasticity >= 0);
		MDouble4 p = to4(particle.Position, 1.0);
		double   v = dot(p, ground);
		if (v > .0) {
			// no collision
		} else {
			// The the object have collide with the ground plane
			MDouble3 gp = point_on_plane(ground); // A point on the ground
			MDouble3 n  = normalized(xyz(ground)); // Normal of the ground plane

			MDouble3 dp = particle.Position - gp;

			// Reflect the position and velocity with the ground plain
			dp = n * (dot(dp, n) * (1 + elasticity));
			particle.Position = particle.Position - dp;

			dp = n * (dot(particle.Velocity, n) * (1 + elasticity));
			particle.Velocity = particle.Velocity - dp;
		}
		return particle;
	}
}

/*!
\brief Dependency node for running a simple physical simulation of one active mass-point and a passive collision plane.

This node contains the interface attributes:

	\param [out] position			p
	\param [out] velocity			v

	\param [in] initialTime			t0
	\param [in] initialPosition		p0
	\param [in] initialVelocity		v0

	\param [in] mass				m
	\param [in] force				f
	\param [in] collisionPlane		cp
	\param [in] collisionElasticity	ce

	The math in this node is described by the following equation:
		p_t		= (p_{t-dt} + v_t) * collision_{cp,ce}
		v_t		= (v_{t-dt} + f_t/m_t) * collision_{cp,ce}
		p_{t_0} = p_0
		v_{t_0} = v_0
*/
class physicsEngine final : public MPxNode
{
public:
	MStatus    compute(const MPlug& plug, MDataBlock& data) override;

	void       getCacheSetup(const MEvaluationNode&, MNodeCacheDisablingInfo&, MNodeCacheSetupInfo&, MObjectArray&) const override;
	void       configCache(const MEvaluationNode&, MCacheSchema&) const override;

	MTimeRange transformInvalidationRange(const MPlug& source, const MTimeRange& input) const override;

	static void* creator();
	static MStatus initialize();

public:
	// Attributes

	static MAttributeOf<bool>  aSimulationEnabled; // [in] When disabled, the current status is frozen as is

	static MAttributeOf<MTime> aInputTime;         // [in] Always connected to the global time node

	// Simulation status, aCurrentStatus(t) is a function of aCurrentStatus(t-1)
	// This attribute must capture all the data of the 'previous status' a solver can reply on
	// In order to support 'simulation resumption', this attribute must be cached
	static  MObject     aCurrentStatus;
		static  MAttributeOf<MTime>    aCurrentTime;     // [out] The time where the current status is computed
		static  MAttributeOf<MDouble3> aCurrentPosition; // [out]
		static  MAttributeOf<MDouble3> aCurrentVelocity; // [out]

	// Initial status
	static  MObject     aInitialStatus;
		static  MAttributeOf<MTime>    aInitialTime;       // [in]
		static  MAttributeOf<MDouble3> aInitialPosition;   // [in]
		static  MAttributeOf<MDouble3> aInitialVelocity;   // [in]

	// Other simulation parameters, can be evaluated at any time
	static  MAttributeOf<double>   aMass;                  // [in] The mass of this object to decide acceleration
	static  MAttributeOf<MDouble3> aForce;                 // [in] External force applied to this object

	/*static MAttributeOf<std::function<MDouble3(MDouble3 position, MDouble3 velocity)> aForceField;*/
	// If a force field is needed as external force
	// It is best to passed it as a 'functor' to make sure it does not depend on aCurrentStatus
	//
	// If you added the force field as the result of evaluating the field at the current status
	// E.g. `aFieldForce(t) = f(aCurrentStatus(t), t)`
	// Then it must be added to aCurrentStatus, because it is depend on the previous status of the obejct

	// Passive colliders, can be evaluated at any time
	static  MAttributeOf<MDouble4> aCollisionPlane;        // [in] A plane to collide with the particle: (A,B,C,D) in Ax+By+Cz+D>=0 
	static  MAttributeOf<double>   aCollisionElasticity;   // [in] The elasticity of the collision plane

public:
	static  MTypeId     id;
	static  MString     nodeName;
};

MTypeId physicsEngine::id = { 0x00081165 };
MString physicsEngine::nodeName = { "physicsEngine" };

MAttributeOf<MTime>    physicsEngine::aInputTime;
MObject                physicsEngine::aCurrentStatus;
MAttributeOf<MTime>    physicsEngine::aCurrentTime;
MAttributeOf<MDouble3> physicsEngine::aCurrentPosition;
MAttributeOf<MDouble3> physicsEngine::aCurrentVelocity;
MObject                physicsEngine::aInitialStatus;
MAttributeOf<MTime>    physicsEngine::aInitialTime;
MAttributeOf<MDouble3> physicsEngine::aInitialPosition;
MAttributeOf<MDouble3> physicsEngine::aInitialVelocity;
MAttributeOf<MDouble3> physicsEngine::aForce;
MAttributeOf<double>   physicsEngine::aMass;
MAttributeOf<MDouble4> physicsEngine::aCollisionPlane;
MAttributeOf<double>   physicsEngine::aCollisionElasticity;
MAttributeOf<bool>     physicsEngine::aSimulationEnabled;

// When setting infinite values, do not use max()/min() directly, could overflow easily.
static constexpr MTime::MTick kMaximumTimeTick = std::numeric_limits<MTime::MTick>::max() / 2;
static constexpr MTime::MTick kMinimumTimeTick = std::numeric_limits<MTime::MTick>::min() / 2 + 1;
static const MTime kMaximumTime = { kMaximumTimeTick / static_cast<double>(MTime::ticksPerSecond()), MTime::kSeconds };
static const MTime kMinimumTime = { kMinimumTimeTick / static_cast<double>(MTime::ticksPerSecond()), MTime::kSeconds };

MStatus physicsEngine::compute(const MPlug& plug, MDataBlock& data)
{
	using namespace physics;

	// Only aCurrentStatus is the computable
	if (plug == aCurrentStatus || plug.parent() == aCurrentStatus)
	{
		MTime time  = get_input(data, aInputTime);
		MTime start = get_input(data, aInitialTime);

		ObjectStatus status;
		status.Time     = get_as_is(data, aCurrentTime);
		status.Position = get_as_is(data, aCurrentPosition);
		status.Velocity = get_as_is(data, aCurrentVelocity);

		if (status.Time <= start || time <= start) {
			// Setup initial status
			status.Time     = start;
			status.Position = get_input(data, aInitialPosition);
			status.Velocity = get_input(data, aInitialVelocity);
		}
		assert(status.Time >= start);

		bool simulation = get_input(data, aSimulationEnabled);

		if (simulation && time > status.Time) {
			MTime dt = time - status.Time;
			MDouble3 force  = get_input(data, aForce);
			double   mass   = get_input(data, aMass);
			MDouble4 ground = get_input(data, aCollisionPlane);
			double   damp   = get_input(data, aCollisionElasticity);

			status = apply_velocity(status, dt, force / mass);
			status = resolve_collision(status, ground, damp);

			// Emulate the slow computation for real world solver
			using namespace std::chrono;
			std::this_thread::sleep_for(0.01s);
		}

		set(data, aCurrentTime,     status.Time);
		set(data, aCurrentPosition, status.Position);
		set(data, aCurrentVelocity, status.Velocity);

		return MS::kSuccess;
	}

	return MS::kUnknownParameter;
}

void physicsEngine::getCacheSetup(const MEvaluationNode& evalNode, MNodeCacheDisablingInfo&, MNodeCacheSetupInfo& setupInfo, MObjectArray&) const
{
	bool simulation;
	if (evalNode.dirtyPlugExists(aSimulationEnabled)) {
		// This attribute is not designed to be animated...
		// It is safer to treat it as running simulation if it is animated
		assert(!"'physicsEngine.simulation' cannot not be animated");
		simulation = true;
	} else {
		auto data = const_cast<physicsEngine*>(this)->forceCache();
		simulation = get_input(data, aSimulationEnabled);
	}

	// evalNode.dirtyPlugExists(aCurrentStatus) checks if `this.position` or `this.velocity` is connected
	// Otherwise compute(aCurrentStatus) will not be called during evaluation
	// There is no simulation to do in that case
	if (simulation && evalNode.dirtyPlugExists(aCurrentStatus)) {
		setupInfo.setRequirement(MNodeCacheSetupInfo::kSimulationSupport,   true);
		setupInfo.setPreference(MNodeCacheSetupInfo::kWantToCacheByDefault, true);
	}
}

void physicsEngine::configCache(const MEvaluationNode& evalNode, MCacheSchema& schema) const
{
	// Always cache the aCurrentStatus(t)
	// It is needed to compute aCurrentStatus(t+1)
	// Other input attributes must be computable at any time
	//
	// Interactions with Maya while background simulation is running may interrupt the simulation and destroy the status data stored in the datablock
	// When the interaction finishes and background simulation is resumed
	// Maya will restore all cached attributes on this node into the datablock
	// For example, the background evaluation are resumed to compute the frame of [5,6,7,...]
	// Before the start of evaluation for frame 5
	// The cached data for frame 4 will be restored into the datablock
	// And thus, one can use `datablock.outpuValue(aCurrentStatus)` in MPxNode::compute() to read the previous-status
	// If aCurrentStatus is not cached and restored, the simulation on frames [5,6,7,...] maybe incorrect
	if (evalNode.dirtyPlugExists(aCurrentStatus))
	{
		schema.add(aCurrentStatus);
	}
}


MTimeRange physicsEngine::transformInvalidationRange(const MPlug& source, const MTimeRange& input) const
{
	// This is the logic for builds with simulation resumption support
	return input | MTimeRange{ input.bounds().min, kMaximumTime };

	// This is for nodes does not cache internal status and thus cannot resume simulation
	// Have to restart from the beginning
	//auto data = const_cast<physicsEngine*>(this)->forceCache();
	//auto start = get_as_is(data, aInitialTime);
	//return input | MTimeRange{ start, kMaximumTime };
}

void* physicsEngine::creator()
{
	return new physicsEngine();
}

MStatus physicsEngine::initialize()
{
	MFnNumericAttribute nAttr;
	MFnUnitAttribute    uAttr;
	MFnCompoundAttribute cAttr;
	MStatus             status;

	aSimulationEnabled = nAttr.create("simulation", "se", MFnNumericData::kBoolean); nAttr.setDefault(true); nAttr.setKeyable(false); nAttr.setWritable(true); nAttr.setReadable(true); nAttr.setStorable(true); nAttr.setChannelBox(true);

	aInputTime = uAttr.create("inputTime", "ipt", MFnUnitAttribute::kTime, 0.0); uAttr.setReadable(false); uAttr.setWritable(true); uAttr.setStorable(false); uAttr.setHidden(true);

	aCurrentStatus = cAttr.create("status", "s");
		aCurrentTime     = uAttr.create("time", "t", MFnUnitAttribute::kTime); uAttr.setDefault(kMinimumTime); uAttr.setWritable(false); uAttr.setConnectable(false); uAttr.setHidden(true);
		aCurrentPosition = nAttr.create("position", "p", MFnNumericData::k3Double);
		aCurrentVelocity = nAttr.create("velocity", "v", MFnNumericData::k3Double);
		cAttr.addChild(aCurrentTime);
		cAttr.addChild(aCurrentPosition);
		cAttr.addChild(aCurrentVelocity);
	cAttr.setReadable(true);
	cAttr.setWritable(false);
	cAttr.setKeyable(false);
	cAttr.setStorable(true);

	aInitialStatus = cAttr.create("initialStatus", "is");
		aInitialTime     = uAttr.create("initialTime", "it", MFnUnitAttribute::kTime, 1.0); uAttr.setKeyable(false); nAttr.setChannelBox(true);
		aInitialPosition = nAttr.create("initialPosition", "ip", MFnNumericData::k3Double); nAttr.setDefault(0.0, 10.0, 0.0); nAttr.setKeyable(false); nAttr.setChannelBox(true);
		aInitialVelocity = nAttr.create("initialVelocity", "iv", MFnNumericData::k3Double); nAttr.setDefault(1.0, 0.0, 0.0); nAttr.setKeyable(false); nAttr.setChannelBox(true);
		cAttr.addChild(aInitialTime);
		cAttr.addChild(aInitialPosition);
		cAttr.addChild(aInitialVelocity);
	cAttr.setWritable(true);
	cAttr.setReadable(true);
	cAttr.setStorable(true);

	aForce               = nAttr.create("force", "f", MFnNumericData::k3Double); nAttr.setDefault(0.0, -9.8, 0.0); nAttr.setWritable(true); nAttr.setReadable(true); nAttr.setStorable(true); nAttr.setChannelBox(true);
	aMass                = nAttr.create("mass", "m", MFnNumericData::kDouble, 1.0); nAttr.setWritable(true); nAttr.setReadable(true); nAttr.setStorable(true); nAttr.setChannelBox(true);

	aCollisionPlane      = nAttr.create("collisionPlane", "cp", MFnNumericData::k4Double); nAttr.setDefault(0.0, 1.0, 0.0, 0.0); nAttr.setWritable(true); nAttr.setReadable(true); nAttr.setStorable(true); nAttr.setChannelBox(true);
	aCollisionElasticity = nAttr.create("collisionElasticity", "ce", MFnNumericData::kDouble, 0.8); nAttr.setWritable(true); nAttr.setReadable(true); nAttr.setStorable(true); nAttr.setChannelBox(true);

	status = addAttribute(aSimulationEnabled);
	status = addAttribute(aInputTime);
	status = addAttribute(aCurrentStatus);
	status = addAttribute(aInitialStatus);
	status = addAttribute(aForce);
	status = addAttribute(aMass);
	status = addAttribute(aCollisionPlane);
	status = addAttribute(aCollisionElasticity);

	status = attributeAffects(aSimulationEnabled,   aCurrentStatus);
	status = attributeAffects(aInputTime,           aCurrentStatus);
	status = attributeAffects(aInitialStatus,       aCurrentStatus);
	status = attributeAffects(aForce,               aCurrentStatus);
	status = attributeAffects(aMass,                aCurrentStatus);
	status = attributeAffects(aCollisionPlane,      aCurrentStatus);
	status = attributeAffects(aCollisionElasticity, aCurrentStatus);

	return MS::kSuccess;
}

MCallbackId gNodeAddedCallbackId;
// Plug-in entry points
//
MStatus initializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj, PLUGIN_COMPANY, "1.0", "Any");

	status = plugin.registerNode(
		physicsEngine::nodeName,
		physicsEngine::id,
		physicsEngine::creator,
		physicsEngine::initialize
	);
	if (!status) {
		status.perror("registerNode");
		return status;
	}

	// Ensure physicsEngine.inputTime is always connected to time node
	gNodeAddedCallbackId = MDGMessage::addNodeAddedCallback([](MObject& node, void*) {
		static MPlug timeOutPlug;
		if (timeOutPlug.isNull())
		{
			MItDependencyNodes itr { MFn::kTime };
			MObject timeNode = itr.thisNode();
			MFnDependencyNode fDN(timeNode);
			timeOutPlug = { timeNode, fDN.attribute("outTime") };
		}

		MFnDependencyNode fDN(node);
		MPlug timeInPlug { node, physicsEngine::aInputTime };

		if (!timeOutPlug.isNull() && !timeInPlug.isNull()) {
			MDGModifier modifier;
			modifier.connect(timeOutPlug, timeInPlug);
			modifier.doIt();
		}
	}, physicsEngine::nodeName);

	return status;
}

MStatus uninitializePlugin(MObject obj)
{
	MStatus   status;
	MFnPlugin plugin(obj);

	MDGMessage::removeCallback(gNodeAddedCallbackId);

	status = plugin.deregisterNode(physicsEngine::id);
	if (!status) {
		status.perror("deregisterNode");
		return status;
	}

	return status;
}
