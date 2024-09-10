//-
// ==========================================================================
// Copyright 2020 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

////////////////////////////////////////////////////////////////////////
// DESCRIPTION:
// 
// Produces the MEL command "threadTestWithLocksCmd".
//
// This is similar to the threadTestCmd, but incorporates thread spin locking
// with the MSpinLock class. 
//
// To use this plug-in, first load it and then execute the following:
//
//    threadTestWithLocksCmd 100000
//
// Here, the argument specifies the iteration count. 
//
////////////////////////////////////////////////////////////////////////

#include <memory>
#include <math.h>

#include <maya/MIOStream.h>
#include <maya/MSimple.h>
#include <maya/MTimer.h>
#include <maya/MGlobal.h>
#include <maya/MThreadAsync.h>
#include <maya/MThreadPool.h>
#include <maya/MSpinLock.h>

#include <mutex>
#include <thread>
#include <chrono>

DeclareSimpleCommand( threadTestWithLocksCmd, PLUGIN_COMPANY, "2008");


#define NUM_TASKS	        32
#define NUM_ASYNC_TASKS	13

MThreadRetVal Pi(void *data);


typedef struct _threadDataTag
{
	int threadNo;
	double pi, step, *globalPi;
	int start, end;
	MSpinLock *spinLock;
	std::mutex *mutexLock;

}threadData;

typedef struct _taskDataTag
{
	int      iterations;
	double    totalPi;

}taskData;

// Pi computation. This function is called from a single thread
double CalcPiSingleThreaded(int iterations)
{
	double pi = 0;
	double step = 1.0/double(iterations);
	for( int i = 0; i < iterations; i++ )
	{
		double x = step * ((double)i-0.5);
		pi += 4.0 / (1.0 + x*x);
	}
	pi /= double(iterations);
	return pi;
}

// Pi computation. This function is called from multiple threads
MThreadRetVal Pi(void *data)
{
	threadData *myData = (threadData *)data;
	double pi = 0;

	for( int i = myData->start; i < myData->end; i++ )
	{
		double x = myData->step * ((double)i-0.5);
		pi += 4.0 / (1.0 + x*x);
	}
	myData->pi = pi;

	return (MThreadRetVal)0;
}

// Function to create thread tasks
void DecomposePi(void *data, MThreadRootTask *root)
{
	taskData *taskD = (taskData *)data;

	int iterationsPerTask = taskD->iterations/NUM_TASKS;
	int limit;

	threadData tdata[NUM_TASKS];

	double step = 1.0f/taskD->iterations;

	for( int i = 0; i < NUM_TASKS; ++i )
	{
		limit                = (i+1)*iterationsPerTask;
		tdata[i].threadNo    = i;
		tdata[i].pi          = 0;
		tdata[i].start       = i*iterationsPerTask;
		limit                = tdata[i].start + iterationsPerTask;
		tdata[i].end         = ( limit < taskD->iterations) ? limit : taskD->iterations;
		tdata[i].step        = step;

		MThreadPool::createTask(Pi, (void *)&tdata[i], root);
	}

	MThreadPool::executeAndJoin(root);

	for( int i = 0; i < NUM_TASKS; ++i )
	{
		taskD->totalPi += tdata[i].pi;
	}
}

// Set up and tear down parallel tasks
double CalcPi(int iterations)
{
	double              Pi = 0;
	taskData            tdata;

	tdata.iterations = 0; tdata.totalPi = 0;
	
	MStatus pr = MThreadPool::init();
	if( MStatus::kSuccess == pr )
	{
		tdata.iterations  = iterations;
		tdata.totalPi     = 0;
		
		MThreadPool::newParallelRegion(DecomposePi, (void *)&tdata);
		
		Pi = tdata.totalPi/iterations;

		// Release reference to thread pool
		MThreadPool::release();
	}

	return Pi;
}

// Pi computation with spin-lock. This function is called from multiple threads
MThreadRetVal SpinSyncPi(void *data)
{
	threadData *myData = (threadData *)data;
	double pi = 0;

	for( int i = myData->start; i < myData->end; i++ )
	{
		double x = myData->step * ((double)i-0.5);
		pi += 4.0 / (1.0 + x*x);
	}
	myData->spinLock->lock();
	(*myData->globalPi) += pi;
	myData->spinLock->unlock();

	return 0;
}

// Function to create thread tasks
void DecomposeSpinSyncPi(void *data, MThreadRootTask *root)
{
	taskData *taskD = (taskData *)data;

	int iterationsPerTask = taskD->iterations/NUM_TASKS;
	int limit;
	MStatus pr = MStatus::kSuccess;

	std::unique_ptr<MSpinLock> spinLock(new MSpinLock());
	if( MStatus::kSuccess == pr )
	{

		threadData tdata[NUM_TASKS];

		double step = 1.0f/taskD->iterations;

		for( int i = 0; i < NUM_TASKS; ++i )
		{
			limit                = (i+1)*iterationsPerTask;
			tdata[i].threadNo    = i;
			tdata[i].pi          = 0;
			tdata[i].globalPi    = &taskD->totalPi;
			tdata[i].start       = i*iterationsPerTask;
			limit                = tdata[i].start + iterationsPerTask;
			tdata[i].end         = ( limit < taskD->iterations) ? limit : taskD->iterations;
			tdata[i].step        = step;
			tdata[i].spinLock    = spinLock.get();

			MThreadPool::createTask(SpinSyncPi, (void *)&tdata[i], root);
		}

		MThreadPool::executeAndJoin(root);

		for( int i = 0; i < NUM_TASKS; ++i )
		{
			taskD->totalPi += tdata[i].pi;
		}
	}
}

// Pi computation with mutex lock. This function is called from multiple threads
MThreadRetVal MutexSyncPi(void *data)
{
	threadData *myData = (threadData *)data;
	double pi = 0;

	for( int i = myData->start; i < myData->end; i++ )
	{
		double x = myData->step * ((double)i-0.5);
		pi += 4.0 / (1.0 + x*x);
	}
	myData->mutexLock->lock();
	(*myData->globalPi) += pi;
	myData->mutexLock->unlock();

	return 0;
}

// Function to create thread tasks
void DecomposeMutexSyncPi(void *data, MThreadRootTask *root)
{
	taskData *taskD = (taskData *)data;

	int iterationsPerTask = taskD->iterations/NUM_TASKS;
	int limit;
	MStatus pr = MStatus::kSuccess;

	std::mutex* mutexLock = new std::mutex();
	if( MStatus::kSuccess == pr )
	{

		threadData tdata[NUM_TASKS];

		double step = 1.0f/taskD->iterations;

		for( int i = 0; i < NUM_TASKS; ++i )
		{
			limit                = (i+1)*iterationsPerTask;
			tdata[i].threadNo    = i;
			tdata[i].pi          = 0;
			tdata[i].globalPi    = &taskD->totalPi;
			tdata[i].start       = i*iterationsPerTask;
			limit                = tdata[i].start + iterationsPerTask;
			tdata[i].end         = ( limit < taskD->iterations) ? limit : taskD->iterations;
			tdata[i].step        = step;
			tdata[i].mutexLock       = mutexLock;

			MThreadPool::createTask(MutexSyncPi, (void *)&tdata[i], root);
		}

		MThreadPool::executeAndJoin(root);

		for( int i = 0; i < NUM_TASKS; ++i )
		{
			taskD->totalPi += tdata[i].pi;
		}
	}
	//
	// Free resources and delete lock
	//
	delete mutexLock;
}

// Set up and tear down parallel tasks
double CalcSpinSyncPi(int iterations)
{
	double              Pi = 0;
	taskData            tdata;
	MStatus pr = MThreadPool::init();
	if( MStatus::kSuccess == pr )
	{
		tdata.iterations  = iterations;
		tdata.totalPi     = 0;
		
		MThreadPool::newParallelRegion(DecomposeSpinSyncPi, (void *)&tdata);

		Pi = tdata.totalPi/iterations;
		MThreadPool::release();
	}

	return Pi;
}

// Set up and tear down parallel tasks
double CalcMutexSyncPi(int iterations)
{
	double              Pi = 0;
	taskData            tdata;
	MStatus pr = MThreadPool::init();
	if( MStatus::kSuccess == pr )
	{
		tdata.iterations  = iterations;
		tdata.totalPi     = 0;
		
		MThreadPool::newParallelRegion(DecomposeMutexSyncPi, (void *)&tdata);

		Pi = tdata.totalPi/iterations;
		MThreadPool::release();
	}

	return Pi;
}

// Compute pi. This function is called from multiple asynchonous threads
MThreadRetVal AsyncPi(void *data)
{
	threadData *myData = (threadData *)data;

	for( int i = myData->start; i < myData->end; i++ )
	{
		 double x = myData->step * ((double)i-0.5);
		myData->pi += 4.0 / (1.0 + x*x);
	}
	return 0;
}

// variable to track threads. As each thread finishes it's work it
// increments this variable. The main thread will wait until this
// variable is equal to the thread count, meaning all threads have
// completed their work variable will equal the threadcount and the
static volatile int g_async_count = 0; 

static MSpinLock asyncSpinLock;

// increment thread completion variable. Uses a lock to prevent race
// conditions where two threads attempt to update the variable
// simultaneously
void AsyncCB(void *data)
{
	asyncSpinLock.lock();
	g_async_count++;
	asyncSpinLock.unlock();
}

static MSpinLock exchangeSpinLock;

// test if variable matches the expected value. Locks required to
// ensure threadsafe access to variables
bool Maya_InterlockedCompare(volatile int* variable, int compareValue)
{
	exchangeSpinLock.lock();
	bool rtn = (*variable == compareValue);
	exchangeSpinLock.unlock();
	return rtn;
}

// Barrier function. Main thread enters here and polls the count
// variable until all worker threads have indicated they have
// completed by incrementing this count.
void WaitForAsyncThreads(int val)
{
	while( !Maya_InterlockedCompare(&g_async_count, val)) {
		std::this_thread::yield();
	}
}

// Set up and tear down asynchronous thread tasks
double CalcAsyncPi(int iterations)
{
	double Pi = 0;

	g_async_count = 0;
	MStatus pr = MThreadAsync::init();
	if( MStatus::kSuccess == pr )
	{
		int iterationsPerTask = iterations/NUM_ASYNC_TASKS;
		int limit;

		threadData tdata[NUM_ASYNC_TASKS];
		
		double step = 1.0f/iterations;
		
		for( int i = 0; i < NUM_ASYNC_TASKS; ++i )
		{
			limit                = (i+1)*iterationsPerTask;
			tdata[i].threadNo    = i;
			tdata[i].pi          = 0;
			tdata[i].start       = i*iterationsPerTask;
			limit                = tdata[i].start + iterationsPerTask;
			tdata[i].end         = ( limit < iterations) ? limit : iterations;
			tdata[i].step        = step;
			
			// start threads. Each thread makes a call to AsyncCB
			// when completed which increments a counter. The wait
			// function below waits until all threads have completed
			// and incremented this counter.
			pr = MThreadAsync::createTask(AsyncPi, (void *)&tdata[i], AsyncCB, NULL);
			if( pr != MStatus::kSuccess )
			{
				return 0;
			}
		}

		// barrier here, waits until all threads have completed before continuing
		WaitForAsyncThreads(NUM_ASYNC_TASKS);
		
		// accumulate sum from each thread
		for( int i = 0; i < NUM_ASYNC_TASKS; ++i )
		{
			Pi += tdata[i].pi;
		}
		Pi /= iterations;
		MThreadAsync::release(); // release async thread
	}
	return Pi;
}

// MSimple command that invokes the serial and parallel thread calculations
MStatus threadTestWithLocksCmd::doIt( const MArgList& args )
{
	MString str = MString("Computation of pi using the Maya API\n");
	MGlobal::displayInfo(str);

	if(args.length() != 1) {
		MString str = MString("Invalid number of arguments, supply iteration count, usage: threadTestWithLocksCmd 100000");
		MGlobal::displayError(str);
		return MStatus::kFailure;
	}

	MStatus stat;

	int iterations = args.asInt( 0, &stat );
	if ( MS::kSuccess != stat ) {
		MString str = MString("Invalid number of arguments, supply iteration count, usage: threadTestWithLocksCmd 100000");
		MGlobal::displayError(str);
		return MStatus::kFailure;
	}

	double pi = 0.0;
	MString piStr;
	double elapsedTime = 0.0;
	MTimer timer;

	// run single threaded
	timer.beginTimer();
	pi = CalcPiSingleThreaded(iterations);
	timer.endTimer();
	elapsedTime = timer.elapsedTime();
	piStr.set(pi, 10); // 10 digits of precision
	str = MString("Unthreaded computation, pi = ") + piStr + MString(" calculated in ") + elapsedTime + MString("s");
	MGlobal::displayInfo(str);

	// run threaded and lock-free
	timer.beginTimer();
	pi = CalcPi(iterations);
	timer.endTimer();
	elapsedTime = timer.elapsedTime();
	piStr.set(pi, 10); // 10 digits of precision
	str = MString("Threaded computation, pi = ") + piStr + MString(" calculated in ") + elapsedTime + MString("s");
	MGlobal::displayInfo(str);

	// run threaded with spin-lock synchronisation
	timer.beginTimer();
	pi = CalcSpinSyncPi(iterations);
	timer.endTimer();
	elapsedTime = timer.elapsedTime();
	piStr.set(pi, 10); // 10 digits of precision
	str = MString("Threaded computation with spin-lock synchronization, pi = ") + piStr + MString(" calculated in ") + elapsedTime + MString("s");
	MGlobal::displayInfo(str);

	// run threaded with mutex synchronisation
	timer.beginTimer();
	pi = CalcMutexSyncPi(iterations);
	timer.endTimer();
	elapsedTime = timer.elapsedTime();
	piStr.set(pi, 10); // 10 digits of precision
	str = MString("Threaded computation with mutex synchronization, pi = ") + piStr + MString(" calculated in ") + elapsedTime + MString("s");
	MGlobal::displayInfo(str);

	// run async threaded
	timer.beginTimer();
	pi = CalcAsyncPi(iterations);
	timer.endTimer();
	elapsedTime = timer.elapsedTime();
	piStr.set(pi, 10); // 10 digits of precision
	str = MString("Threaded computation with async thread, pi = ") + piStr + MString(" calculated in ") + elapsedTime + MString("s\n");
	MGlobal::displayInfo(str);

	return MStatus::kSuccess;
}
