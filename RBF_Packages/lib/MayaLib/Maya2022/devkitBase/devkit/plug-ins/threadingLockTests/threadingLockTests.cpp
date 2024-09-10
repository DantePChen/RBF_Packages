//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <maya/MSimple.h>
#include <maya/MGlobal.h>
#include <maya/MIOStream.h>
#include <maya/MTimer.h>

#include <maya/MSpinLock.h>
#include <maya/MThreadUtils.h>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_reduce.h>

#include <mutex>
#include <atomic>

DeclareSimpleCommand( threadingLockTests, PLUGIN_COMPANY, "3.0");


int doMutexCalc(int iterations, int val)
{
	int sum = 0;

	std::mutex mutex;
    tbb::parallel_for(tbb::blocked_range<int>(0, iterations),
        [&](const tbb::blocked_range<int> &br) {
    for (int i = br.begin(); i < br.end(); i++) {
		mutex.lock();
		sum += val;
		mutex.unlock();
	}
    });
	return sum;
}

int doSpinCalc(int iterations, int val)
{
	int sum = 0;

	MSpinLock mutex;
    tbb::parallel_for(tbb::blocked_range<int>(0, iterations),
        [&](const tbb::blocked_range<int> &br) {
    for (int i = br.begin(); i < br.end(); i++) {
		mutex.lock();
		sum += val;
		mutex.unlock();
	}
	});
	return sum;
}

int doAtomicCalc(int iterations, int val)
{
    std::atomic<int> sum{ 0 };

    tbb::parallel_for(tbb::blocked_range<int>(0, iterations),
        [&](const tbb::blocked_range<int> &br) {
    for (int i = br.begin(); i < br.end(); i++) {
        sum += val;
    }
    });
	return sum;
}

int doReductionCalc(int iterations, int val)
{
	int sum = 0;

    sum =
        tbb::parallel_reduce(
            tbb::blocked_range<unsigned int>(0, iterations, 8196),
            0.0,  //initial value
            [=](const tbb::blocked_range<unsigned int> &br, int init)->int {
                for (unsigned int i = br.begin(); i != br.end(); i++) {
                    init += val;
                }
                return init;
            },
            [](int x, int y)->int { return x + y; }
    );

	return sum;
}

int doUnsafeCalc(int iterations, int val)
{
	int sum = 0;

    tbb::parallel_for(tbb::blocked_range<int>(0, iterations),
        [&](const tbb::blocked_range<int> &br) {
    for (int i = br.begin(); i < br.end(); i++) {
		sum += val;
	}
    });
	return sum;
}

int doSerialCalc(int iterations, int val)
{
	int sum = 0;
	for(int i=0; i<iterations; i++) {
		sum += val;
	}
	return sum;
}

MStatus threadingLockTests::doIt( const MArgList& args )
{
	MStatus stat;

	if ( MS::kSuccess != stat )
		cout<<"Error creating curve."<<endl;

	if(args.length() != 1) {
		MString str = MString("Invalid number of arguments, usage: threadingLockTests 1000000");
		MGlobal::displayError(str);
		return MStatus::kFailure;
	}

	printf("In threadedLockTests, numthreads %d\n", MThreadUtils::getNumThreads());
	int iterations = args.asInt( 0, &stat );
	int increment = 2;
	int sum = 0;
	int repeat = 1;
	MTimer timer;
	MString out;


	// Mutex lock calculation
 	timer.beginTimer();
	for(int i=0; i<repeat; i++) {
		sum = doMutexCalc(iterations, increment);
		if(sum != iterations*increment) {
			printf("doMutexCalc failed, sum %d\n", sum);
			break;
		}
	}
	timer.endTimer(); 
	out = MString("Runtime doMutexCalc ") + timer.elapsedTime();
	MGlobal::displayInfo(out);


	// Spin mutex lock calculation
 	timer.beginTimer();
	for(int i=0; i<repeat; i++) {
		sum = doSpinCalc(iterations, increment);
		if(sum != iterations*increment) {
			printf("doSpinCalc failed, sum %d\n", sum);
			break;
		}
	}
	timer.endTimer(); 
	out = MString("Runtime doSpinCalc ") + timer.elapsedTime();
	MGlobal::displayInfo(out);


	// Atomic calculation
 	timer.beginTimer();
	for(int i=0; i<repeat; i++) {
		sum = doAtomicCalc(iterations, increment);
		if(sum != iterations*increment) {
			printf("doAtomicCalc failed, sum %d\n", sum);
			break;
		}
	}
	timer.endTimer(); 
	out = MString("Runtime doAtomicCalc ") + timer.elapsedTime();
	MGlobal::displayInfo(out);


	// Reduction calculation
 	timer.beginTimer();
	for(int i=0; i<repeat; i++) {
		sum = doReductionCalc(iterations, increment);
		if(sum != iterations*increment) {
			printf("doReductionCalc failed, sum %d\n", sum);
			break;
		}
	}
	timer.endTimer(); 
	out = MString("Runtime doReductionCalc ") + timer.elapsedTime();
	MGlobal::displayInfo(out);


	// Unsafe calculation (race condition)
 	timer.beginTimer();
	for(int i=0; i<repeat; i++) {
		sum = doUnsafeCalc(iterations, increment);
		if(sum != iterations*increment) {
			printf("doUnsafeCalc failed, sum %d\n", sum);
			break;
		}
	}
	timer.endTimer(); 
	out = MString("Runtime doUnsafeCalc ") + timer.elapsedTime();
	MGlobal::displayInfo(out);


	// Unthreaded calculation
 	timer.beginTimer();
	for(int i=0; i<repeat; i++) {
		sum = doSerialCalc(iterations, increment);
		if(sum != iterations*increment) {
			printf("doUnthreadedCalc failed, sum %d\n", sum);
			break;
		}
	}
	timer.endTimer(); 
	out = MString("Runtime doUnthreadedCalc ") + timer.elapsedTime();
	MGlobal::displayInfo(out);


	return stat;
}
