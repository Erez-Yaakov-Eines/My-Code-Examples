#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_CRANE_ID craneId - 1
#define ARRAY_CURRENT_VESSEL_ID currentVesselId - 1
#define ARRAY_VESSEL_ID vesselId - 1


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../Constants.h"
#include "../Structures.h"

Barrier barrier;
UnloadingQuay unloadingQuay;

DWORD read, written;
HANDLE BarrierMutex;
HANDLE UnloadingQuayMutex;
HANDLE ReadHandle, WriteHandle;
HANDLE CanalToMedSeaMutex;
HANDLE ReportMutex;
HANDLE RandObjectMutex;
HANDLE CranesWaiting[MAX_CLUSTER_SIZE];
HANDLE VesselsWaiting[MAX_CLUSTER_SIZE];

CHAR globalPrintBuffer[STR_BUFF_SIZE];

int		approveClusterSize(char* buffer, int* clusterSize);
void	CloseAllHandles(Vessels* vessels, Cranes* cranes);
int		constructCranes(int numberOfCranes, Cranes* cranes);
int		CreateBarrier(int M);
void	CreateUnloadingQuay(Cranes* cranes, int numberOfCranes);
int		cyclicDecrement(int* index, int cycleLength);
int		dequeueVessel();
int		drawCraneNum(int clusterSize);
int		enqueueVessel(int vesselId);
void	enterBarrier(int vesselId);
void	enterUnloadingQuay(int vesselId);
int		exitUnloadingQuay(int vesselId);
int		fifoEnqueue(FifoQueue* fifo, int element);
int		fifoDequeue(FifoQueue* fifo);
void 	findCrane(int vesselId);
int		getUnoccupiedCraneId(int vesselId);
void	initFifoQueue(FifoQueue* fifo);
void	initHandles(Vessels* vessels, Cranes* cranes);
int		isDivisor(int denominator, int numerator);
int		isPrime(int);
int		isUnloadingQuayFree();
int		killCranes();
int		calcSleepTime();
int		checkSemaphoreAlloc(HANDLE semaphore);
void	CreateSemaphores(Vessels* vessels, Cranes* cranes);
int		randFromTo(int from, int to);
void	report(char* message);
void	recieveIncomingVessels(int clusterSize, char* buffer, Vessels* vessels);
void	recieveViaSuez(char* buffer);
void	releaseMVessels(int lastVesselInSubCluster);
void	seedRandom(int* internalVariableA, int* internalVariableB);
int		sendViaSuez(char* buffer);
void	sailViaSuez(char* buffer, int vesselId);
int		signalBarrier();
void	signalUnloadingQuayBusy();
DWORD	WINAPI Crane(PVOID Param);
DWORD	WINAPI Vessel(PVOID Param);

int main()
{
	ReadHandle = GetStdHandle(STD_INPUT_HANDLE);
	WriteHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CHAR buffer[STR_BUFF_SIZE];
	int clusterSize = 0, numberOfCranes = 0;
	Vessels vessels;
	Cranes cranes;
	initHandles(&vessels, &cranes);
	CreateSemaphores(&vessels, &cranes);
	seedRandom((int*)&buffer, (int*)&numberOfCranes);
	//wait for Haifa to negotiate cluster
	if (approveClusterSize(buffer, &clusterSize) != True) {
		ERROR_PRINT(EILAT_PORT_EXIT);
		CloseAllHandles(&vessels, &cranes);
		exit(EXIT_SUB_PROCESS_FAILURE);
	}

	numberOfCranes = drawCraneNum(clusterSize);
	if (CreateBarrier(numberOfCranes) != True ||
		constructCranes(numberOfCranes, &cranes) != True)
	{
		ERROR_PRINT(EILAT_PROCESS_ERROR);
		CloseAllHandles(&vessels, &cranes);
		exit(EXIT_SUB_PROCESS_FAILURE);
	}
	CreateUnloadingQuay(&cranes, numberOfCranes);

	recieveIncomingVessels(clusterSize, buffer, &vessels);

	WaitForMultipleObjects(clusterSize, vessels.VesselThreadHandles, TRUE, INFINITE);
	PRINT(EILAT_VESSELS_FINISHED);

	if (killCranes() == False) {
		ERROR_PRINT(EILAT_PROCESS_ERROR);
		CloseAllHandles(&vessels, &cranes);
		exit(EXIT_SUB_PROCESS_FAILURE);
	}

	WaitForMultipleObjects(numberOfCranes, cranes.CraneThreadHandles, TRUE, INFINITE);
	PRINT(EILAT_CRANES_FINISHED);

	PRINT(EILAT_PORT_EXIT);
	CloseAllHandles(&vessels, &cranes);
	return EXIT_SUCCESS;
}

/*
Naive primality test
*/
int isPrime(int n) {
	if (n <= 1) return False;
	if (n <= 3) return True;
	if (n % 2 == 0 || n % 3 == 0) return False;
	for (int i = 5; i * i <= n; i = i + 6)
		if (n % i == 0 || n % (i + 2) == 0) return False;

	return True;
}

int sendViaSuez(char* buffer) {
	AQUIRE(CanalToMedSeaMutex);
	if (!WriteFile(WriteHandle, buffer, STR_BUFF_SIZE, &written, NULL)) {
		PRINT("Eilat Error writing to pipe\n");
		return False;
	}
	RELEASE(CanalToMedSeaMutex);
	return True;
}

void sailViaSuez(char* buffer, int vesselId) {
	AQUIRE(CanalToMedSeaMutex);

	PRINT("Vessel %d - entering Canal: Red Sea ==> Med. Sea\n", vesselId);
	SLEEP;

	if (!WriteFile(WriteHandle, buffer, STR_BUFF_SIZE, &written, NULL)) {
		PRINT("Eilat Error writing to pipe\n");
		exit(EXIT_SUB_PROCESS_FAILURE);
	}

	RELEASE(CanalToMedSeaMutex);

}

void recieveViaSuez(char* buffer) {
	if (!ReadFile(ReadHandle, buffer, STR_BUFF_SIZE, &read, NULL)) {
		PRINT("Eilat Error reading from pipe\n");
		exit(EXIT_SUB_PROCESS_FAILURE);
	}
}

//Needs ReportMutex Synchronization
void report(char* message) {
	// PRINT MACRO uses report
	time_t rawtime;
	struct tm *info;
	time(&rawtime);
	info = localtime(&rawtime);
	char hours[3], minutes[3], seconds[3];
	//append leading 0 to single digits in time format (probably already a method that does this
	//	but, whatever...)
	if (info->tm_hour < 10)
		sprintf(hours, "0%d", info->tm_hour);
	else
		sprintf(hours, "%d", info->tm_hour);
	if (info->tm_min < 10)
		sprintf(minutes, "0%d", info->tm_min);
	else
		sprintf(minutes, "%d", info->tm_min);
	if (info->tm_sec < 10)
		sprintf(seconds, "0%d", info->tm_sec);
	else
		sprintf(seconds, "%d", info->tm_sec);

	fprintf(stderr,"[%s:%s:%s] %s", hours, minutes, seconds, message);
}

void CloseAllHandles(Vessels* vessels, Cranes* cranes) {
	if (ReadHandle != NULL) CloseHandle(ReadHandle);
	if (WriteHandle != NULL) CloseHandle(WriteHandle);
	if (ReportMutex != NULL) CloseHandle(ReportMutex);
	if (CanalToMedSeaMutex != NULL) CloseHandle(CanalToMedSeaMutex);
	if (RandObjectMutex != NULL) CloseHandle(RandObjectMutex);
	if (BarrierMutex != NULL) CloseHandle(BarrierMutex);
	if (UnloadingQuayMutex != NULL) CloseHandle(UnloadingQuayMutex);

	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i) {
		if (vessels->VesselThreadHandles[i] != NULL)
			CloseHandle(vessels->VesselThreadHandles[i]);
		if (VesselsWaiting[i] != NULL)
			CloseHandle(VesselsWaiting[i]);
		if (cranes->CraneThreadHandles[i] != NULL)
			CloseHandle(cranes->CraneThreadHandles[i]);
		if (CranesWaiting[i] != NULL)
			CloseHandle(CranesWaiting[i]);
	}
}

int cyclicDecrement(int* index, int cycleLength) {
	if (cycleLength <= 0 || index == NULL) return False;

	else {
		*index = (*index - 1 + cycleLength) % cycleLength;
		return True;
	}
}

void seedRandom(int* internalVariableA, int* internalVariableB)
{
	AQUIRE(RandObjectMutex);
	// some bad pseudo randomness
	int seed = (int)internalVariableA + (int)internalVariableB + (int)time(NULL);
	srand(seed);

	RELEASE(RandObjectMutex);
}

int isDivisor(int denominator, int numerator) {
	if (denominator <= 0 || denominator > numerator) return -1;
	return ((numerator % denominator == 0) ? True:False);
}

void initFifoQueue(FifoQueue* fifo) {
	fifo->head = fifo->tail = 0;
	fifo->size = 0;
	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i) {
		fifo->Array[i] = EMPTY_ELEMENT;
	}
}

int fifoEnqueue(FifoQueue* fifo, int element) {
	//Error - case null ptr
	if (fifo == NULL) return False;
	//Error - case Queue is full
	if (fifo->size == MAX_CLUSTER_SIZE) return False;
	//Error - case non empty slot
	if (fifo->Array[fifo->tail] != -1) return False;
	//Normal Flow
	fifo->Array[fifo->tail] = element;
	cyclicDecrement(&fifo->tail, MAX_CLUSTER_SIZE);
	++(fifo->size);

	return True;
}

int	fifoDequeue(FifoQueue* fifo) {
	//Error - null ptr
	if (fifo == NULL) return False;
	//Error - Q has bad parameter "size"
	if (fifo->size <= 0) return False;

	int dequeuedElement = fifo->Array[fifo->head];
	fifo->Array[fifo->head] = EMPTY_ELEMENT;
	//Error - Q slot supposed to be full actually has EMPTY_ELEMENT
	if (dequeuedElement == EMPTY_ELEMENT) return False;
	//Normal Flow
	cyclicDecrement(&fifo->head, MAX_FIFO_SIZE);
	--(fifo->size);

	return dequeuedElement;
}

void initHandles(Vessels* vessels, Cranes* cranes) {
	ReportMutex = NULL;
	CanalToMedSeaMutex = NULL;
	BarrierMutex = NULL;
	UnloadingQuayMutex = NULL;
	RandObjectMutex = NULL;

	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i)
	{
		CranesWaiting[i] = NULL;
		VesselsWaiting[i] = NULL;

		cranes->dwCranesThreadIdArray[i] = -1;
		cranes->CraneThreadHandles[i] = NULL;
		cranes->CraneThreadsId[i] = -1;

		vessels->dwVesselsThreadIdArray[i] = -1;
		vessels->VesselThreadHandles[i] = NULL;
		vessels->VesselThreadsId[i] = -1;
	}
	
}

int approveClusterSize(char* buffer, int* clusterSize) {

	/* check cluster size parameter */
	recieveViaSuez(buffer);
	*clusterSize = atoi(buffer);
	PRINT("Eilat Port - Haifa Port requesting passage of cluster size %d\n", *clusterSize);
	if (isPrime(*clusterSize)) {
		sprintf(buffer, "%d", False);
		if (sendViaSuez(buffer) == False)
			return -1;
		PRINT("Eilat Port - Sending negative response to Haifa...\n");
		return False;
	}
	else {
		sprintf(buffer, "%d", True);
		if (sendViaSuez(buffer) == False)
			return -1;
		PRINT("Eilat Port - Sending positive response to Haifa...\n");
		return True;
	}
}

int drawCraneNum(int clusterSize) {
	//draw number of cranes in port
	int numberOfCranes = randFromTo(MIN_CRANE_NUM, clusterSize - 1);
	while (!isDivisor(numberOfCranes, clusterSize)) {//probabilistically wasteful but simple to implement.
		//Absolutely terrible if max cluster size is increased
		numberOfCranes = randFromTo(MIN_CRANE_NUM, clusterSize - 1);
	}
	return numberOfCranes;
}

void recieveIncomingVessels(int clusterSize, char* buffer, Vessels* vessels) {
	int vesselsRecieved = 0, currentVesselId = 0;

	while (vesselsRecieved < clusterSize) {
		recieveViaSuez(buffer);
		currentVesselId = atoi(buffer);

		PRINT("Vessel %d - arrived at Red sea from Med. Sea\n", currentVesselId);
		++vesselsRecieved;

		vessels->VesselThreadsId[ARRAY_CURRENT_VESSEL_ID] = currentVesselId;
		vessels->VesselThreadHandles[ARRAY_CURRENT_VESSEL_ID] = CreateThread(NULL, 0, Vessel,
			&vessels->VesselThreadsId[ARRAY_CURRENT_VESSEL_ID], 0, &vessels->dwVesselsThreadIdArray[ARRAY_CURRENT_VESSEL_ID]);

		if (vessels->VesselThreadHandles[ARRAY_CURRENT_VESSEL_ID] == NULL)
		{
			printf("main::Unexpected Error in Thread %d  Creation\n", currentVesselId);
			exit(EXIT_SUB_PROCESS_FAILURE);
		}
	}
}

int constructCranes(int numberOfCranes, Cranes* cranes)
{
	/*Initialise Cranes in Array (according to
	number of cranes drawn*/
	for (int i = 0; i < numberOfCranes; ++i) {
		cranes->CraneThreadsId[i] = i + 1;
		cranes->CraneThreadHandles[i] = CreateThread(NULL, 0, Crane,
			&cranes->CraneThreadsId[i], 0, &cranes->dwCranesThreadIdArray[i]);

		//Need to ensure Thread was created 
		  //successfully, otherwise - Error Handling and 
		  //exit!
		if (cranes->CraneThreadHandles[i] == NULL)
		{
			PRINT("main::Unexpected Error in Thread %d  Creation\n", i);
			return False;
		}
	}
	return True;
}

DWORD WINAPI Crane(PVOID Param) {
	int craneId = *(int*)Param;
	PRINT("Crane  %d - Created\n", craneId);

	int cargoWeight = NOT_SET;
	//wait after creation
	SCHEDULING_CONSTRAINT(CranesWaiting[ARRAY_CRANE_ID]);
	//check if crane killed by Eilat main thread before ever Offloading.
	AQUIRE(UnloadingQuayMutex);
	if (unloadingQuay.cranesOccupied[ARRAY_CRANE_ID] == END_CRANE_LIFE) {
		PRINT("Crane  %d - Exiting...\n", craneId);
		return EXIT_SUCCESS;
	}
	RELEASE(UnloadingQuayMutex);

	//arbitraty negative value different from END_CRANE_LIFE
	int vesselId = END_CRANE_LIFE - 1;

	while (vesselId != END_CRANE_LIFE) {
		AQUIRE(UnloadingQuayMutex);

		vesselId = unloadingQuay.cranesOccupied[ARRAY_CRANE_ID];
		if (vesselId == UNOCCUPIED) {
			PRINT("Error! - Crane  %d recieved vesselId = -1\n", craneId);
		}
		PRINT("Crane  %d - Now servicing Vessel %d\n", craneId, vesselId);

		//cranes can work in parallel
		RELEASE(UnloadingQuayMutex);
		SLEEP;
		AQUIRE(UnloadingQuayMutex);

		cargoWeight = unloadingQuay.craneWeightsForAssignedVessel[ARRAY_CRANE_ID];

		unloadingQuay.craneWeightsForAssignedVessel[ARRAY_CRANE_ID] = NOT_SET;
		unloadingQuay.cranesOccupied[ARRAY_CRANE_ID] = UNOCCUPIED;
		END_SCHEDULING_CONSTRAINT(VesselsWaiting[ARRAY_VESSEL_ID]);

		RELEASE(UnloadingQuayMutex);

		SCHEDULING_CONSTRAINT(CranesWaiting[ARRAY_CRANE_ID]);

		//woken up crane checks new vesselId
		AQUIRE(UnloadingQuayMutex);
		vesselId = unloadingQuay.cranesOccupied[ARRAY_CRANE_ID];
		RELEASE(UnloadingQuayMutex);

	}

	PRINT("Crane  %d - Exiting...\n", craneId);
	return EXIT_SUCCESS;
}

DWORD WINAPI Vessel(PVOID Param)
{
	CHAR reportBuffer[STR_BUFF_SIZE];
	int vesselId = *(int*)Param;
	seedRandom((int*)&reportBuffer, (int*)&vesselId);

	PRINT("Vessel %d - arrived @ Eilat Port\n", vesselId);

	SLEEP;

	PRINT("Vessel %d - entering barrier\n", vesselId);
	enterBarrier(vesselId);
	enterUnloadingQuay(vesselId);
	if (exitUnloadingQuay(vesselId) != True) {
		return EXIT_FAILURE;
	}

	sprintf(reportBuffer, "%d", vesselId);
	sailViaSuez(reportBuffer, vesselId);

	return EXIT_SUCCESS;
}

int CreateBarrier(int M) {

	AQUIRE(BarrierMutex);

	if (M <= 0) {
		PRINT("\n\nError! - initBarrier M parameter <= 0\n\n");
		RELEASE(BarrierMutex);
		return False;
	}
	barrier.M = M;

	initFifoQueue(&barrier.vesselQ);

	RELEASE(BarrierMutex);
	return True;
}

void enterBarrier(int vesselId) {
	AQUIRE(BarrierMutex);

	//check for proper Enqueuing
	if (!enqueueVessel(vesselId)) {
		PRINT("vessel %d barrier enqueue error!", vesselId);
	}
	//Case M'th vessel entering AND Unloading Quay is free
	// This condition AQUIRES UnloadingQuayMutex!!!
	if (barrier.vesselQ.size == barrier.M && isUnloadingQuayFree()) {
		releaseMVessels(vesselId);
		return;
	}

	RELEASE(BarrierMutex);
	//Wait in barrier.
	SCHEDULING_CONSTRAINT(VesselsWaiting[ARRAY_VESSEL_ID]);
}

//Synchronized method!!! - Must aquire Barrier Mutex before using
int enqueueVessel(int vesselId)
{
	//Error - cannot enqueue Vessel with ID < 1
	if (vesselId <= 0) return False;

	return fifoEnqueue(&barrier.vesselQ, vesselId);
}

int dequeueVessel() 
{
	AQUIRE(BarrierMutex);

	int dequedVessel = fifoDequeue(&barrier.vesselQ);

	RELEASE(BarrierMutex);
	return dequedVessel;
}

void CreateUnloadingQuay(Cranes* cranes, int numberOfCranes) {
	unloadingQuay.vesselCount = EMPTY;
	unloadingQuay.Cranes = cranes->CraneThreadHandles;
	unloadingQuay.numberOfCranes = numberOfCranes;
	unloadingQuay.isBusy = False;
	unloadingQuay.lastSubClusterVessel = NOT_SET;
	
	for (int i = 0; i < numberOfCranes; ++i) {
		unloadingQuay.cranesOccupied[i] = UNOCCUPIED;
		unloadingQuay.craneWeightsForAssignedVessel[i] = NOT_SET;
	}
}

void enterUnloadingQuay(int vesselId) {

		PRINT("Vessel %d - entering Unloading Quay\n", vesselId);
		SLEEP;
		findCrane(vesselId);
}

int	isUnloadingQuayFree() {
	int isFree = False;
	AQUIRE(UnloadingQuayMutex);
		//Excessive error checks, for ease of Debugging.
		//Error - busy AND empty Quay
		if ((unloadingQuay.vesselCount != 0) && (unloadingQuay.isBusy == False)) {
			PRINT("ERROR! Unloading Quay busy but reports empty\n");
		}

		if (unloadingQuay.isBusy == False && unloadingQuay.vesselCount == EMPTY)
		{
			isFree = True;
		}
	RELEASE(UnloadingQuayMutex);
	//after releasing Mutex, result can technically change, but not in the context i'm using it.
	return isFree;
}

int killCranes()
{
	AQUIRE(UnloadingQuayMutex);
	int semaphoreSuccess = False;
	for (int i = 0; i < unloadingQuay.numberOfCranes; ++i)
	{
		unloadingQuay.cranesOccupied[i] = END_CRANE_LIFE;
		semaphoreSuccess = END_SCHEDULING_CONSTRAINT(CranesWaiting[i]);
		if (semaphoreSuccess == False) {
			PRINT("Error! - Semaphore Release error\n");
			return False;
		}
	}

	RELEASE(UnloadingQuayMutex);
	return True;
}

void signalUnloadingQuayBusy() {
	AQUIRE(UnloadingQuayMutex);
//Excessive error checks, for ease of Debugging.
	if (unloadingQuay.isBusy == True) {
		PRINT("ERROR! tried signaling busy to an already busy unloading quay\n");
	}

	unloadingQuay.isBusy = True;
	unloadingQuay.vesselCount = unloadingQuay.numberOfCranes;

	RELEASE(UnloadingQuayMutex);
}

//Synchronized on Barrier Mutex & UnloadingQuay Mutex!!!!
int signalBarrier() {

	unloadingQuay.isBusy = True;
	unloadingQuay.vesselCount = unloadingQuay.numberOfCranes;

	int semaphoreSuccess = True;

	int leavingVessel = NO_VESSEL;
	for (int i = 0; i < barrier.M; ++i) {
		leavingVessel = dequeueVessel();
		if (leavingVessel == NO_VESSEL) {
			PRINT("vessel Dequeue ERROR!!!\n");
			return False;
		}
		//hard time with MACRO replacements :(
		semaphoreSuccess = END_SCHEDULING_CONSTRAINT(VesselsWaiting[leavingVessel - 1]);
		if (!semaphoreSuccess) {
			PRINT("Error! semaphore Release error\n");
			return False;
		}
	}
	return True;
}

//Synchronized on BarrierMutex!
void releaseMVessels(int vesselId) {
	int M = barrier.M;
	//signalUnloadingQuayBusy AQUIRES UnloadingQuay Mutex!!!
	signalUnloadingQuayBusy(M);
	//assumes running thread AQUIRED BarrierMutex
	RELEASE(BarrierMutex);//let others enter barrier queue while releasing vessels from barrier.
	//Release M vessels from barrier fifo Q
	for (int i = 0; i < M; ++i) {
		int leavingVessel = dequeueVessel();
		if (leavingVessel == NO_VESSEL) {
			PRINT("vessel Dequeue ERROR!!!\n");
		}
		
		if (leavingVessel != vesselId) {
			int semaphoreSuccess = True;//some annoying MACRO Replacement workaround
			semaphoreSuccess = END_SCHEDULING_CONSTRAINT(VesselsWaiting[leavingVessel - 1]);
			if (!semaphoreSuccess) {
				PRINT("Error! semaphore Release error\n");
				exit(EXIT_FAILURE);
			}
		}
	}

}

void findCrane(int vesselId) {
	AQUIRE(UnloadingQuayMutex);

	//find crane
	int craneId = getUnoccupiedCraneId(vesselId);
	unloadingQuay.cranesOccupied[ARRAY_CRANE_ID] = vesselId;
	PRINT("Vessel %d - Positioning at Quay Crane %d\n", vesselId, craneId);
	//draw random weight
	int myCargoWeight = randFromTo(MIN_CARGO_WEIGHT, MAX_CARGO_WEIGHT);
	PRINT("Vessel %d - Cargo weight = %d Tons\n", vesselId, myCargoWeight);
	//supply crane with weight via shared memory
	unloadingQuay.craneWeightsForAssignedVessel[ARRAY_CRANE_ID] = myCargoWeight;
	//Release Assigned crane to work
	END_SCHEDULING_CONSTRAINT(CranesWaiting[ARRAY_CRANE_ID]);

	RELEASE(UnloadingQuayMutex);
	//Wait for Crane to finish
	PRINT("Vessel %d - Waiting for Crane %d to Offload Cargo\n", vesselId, craneId);
	SCHEDULING_CONSTRAINT(VesselsWaiting[ARRAY_VESSEL_ID]);
}
//Synchronized method - must AQUIRE UnloadingQuay Mutex to enter
int getUnoccupiedCraneId(int vesselId) {
	for (int i = 0; i < unloadingQuay.numberOfCranes; ++i) {
		if (unloadingQuay.cranesOccupied[i] == UNOCCUPIED) return (i + 1);
	}
	return -1;
}

int exitUnloadingQuay(int vesselId) {
	AQUIRE(BarrierMutex);
	AQUIRE(UnloadingQuayMutex);
	
	--(unloadingQuay.vesselCount);

	SLEEP;
	PRINT("Vessel %d - Exiting Unloading Quay\n", vesselId);

	if (unloadingQuay.vesselCount == EMPTY)
	{
		unloadingQuay.isBusy = False;

		if (barrier.vesselQ.size >= unloadingQuay.numberOfCranes) {
			if (signalBarrier() != True) {
				PRINT("Error - Vessel %d error exiting Quay\n", vesselId);
				RELEASE(UnloadingQuayMutex);
				RELEASE(BarrierMutex);
				return False;
			}
		}
	}
	RELEASE(UnloadingQuayMutex);
	RELEASE(BarrierMutex);
	return True;
}

int calcSleepTime() {
	return randFromTo(MIN_SLEEP_TIME, MAX_SLEEP_TIME);
}

int checkSemaphoreAlloc(HANDLE semaphore) {
	if (semaphore == NULL) {
		PRINT("Eilat CreateMutex error: %d\n", GetLastError());
		return False;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS) {
		PRINT("Eilat CreateMutex opened an existing mutex\n");
		return False;
	}
	return True;
}

int randFromTo(int from, int to) {
	if (from < 0 || from >= to) return -1;

	AQUIRE(RandObjectMutex);

	int generated = ((rand() % (to - from + 1)) + from);

	RELEASE(RandObjectMutex);

	return generated;
}

void CreateSemaphores(Vessels* vessels, Cranes* cranes) {
	ReportMutex = OpenMutex(//this is a named mutex created by Haifa port
		MUTEX_ALL_ACCESS,            // request full access
		FALSE,                       // handle not inheritable
		TEXT("_Haifa_Eilat_report_Mutex"));  // object name

	RandObjectMutex = OpenMutex(//this is a named mutex created by Haifa port
		MUTEX_ALL_ACCESS,            // request full access
		FALSE,                       // handle not inheritable
		TEXT("_Haifa_Eilat_Rand_Mutex"));  // object name

	//initialize a scedualing constraint semaphore for each Crane with initial value of 0 and max value 1.
	//	once a Crane is created, It's thread WAITs on it's own semaphore, and will be signaled
	//	by the Unloading Quay ADT to start working on an incoming Eilat Vessal.
	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i)
	{
		CranesWaiting[i] = CreateSemaphore(NULL, 0, 1, NULL);
		VesselsWaiting[i] = CreateSemaphore(NULL, 0, 1, NULL);
		if (CranesWaiting[i] == NULL || VesselsWaiting[i] == NULL)
		{
			PRINT("Error! semaphore allocation error.\n");
			CloseAllHandles(vessels, cranes);
			exit(EXIT_FAILURE);
		}
	}

	CanalToMedSeaMutex = CreateMutex(NULL, FALSE, NULL);
	BarrierMutex = CreateMutex(NULL, FALSE, NULL);
	UnloadingQuayMutex = CreateMutex(NULL, FALSE, NULL);

	//non-generic check allocations :(
	if (checkSemaphoreAlloc(ReportMutex) == False ||
		checkSemaphoreAlloc(CanalToMedSeaMutex) == False ||
		checkSemaphoreAlloc(BarrierMutex) == False ||
		checkSemaphoreAlloc(UnloadingQuayMutex) == False ||
		checkSemaphoreAlloc(RandObjectMutex) == False)
	{
		PRINT("banana\n");
		CloseAllHandles(vessels, cranes);
		exit(EXIT_SUB_PROCESS_FAILURE);
	}

}