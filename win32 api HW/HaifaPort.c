/**
 * @file HaifaPort.c
 * @author Erez Yaakov Eines (Erez_Eines@yahoo.com)
 * @author Ori Itskovitsh
 * @brief 
 * @version 0.1
 * @date 01-08-2020
 * 
 * @copyright not so sure, but If any exist, they probably belong to Ruppin Academic Center and Dr. Moshe Deutsch
 * 
 */

/* KNOWN Issues

	1) This project uses functions which are considered unsafe due to security vulnerabilities,
		and does so for reasons of simplicity. 
	2) Some MACRO expansions failed when used as a condition for a control structure.
		bypassed using assignment of return value to local variable.
	3) Many methods WAIT on mutexes when they start.
		calling such methods carelessly from code that has already aquired mutexes can lead to some
		frustrating synchronization bugs.
	4) Some methods Wait and Release Semaphores/Mutex several times throughout, and probably 
		need refactoring or rethinking their cohesion / coupling with other methods.
	5) In contrast to Problem 4, some methods rely on other methods to aquire a mutex for them.
		i'm not so sure this is right for all methods which use this strategy in this project.
	6) Some methods return integer value for error checks while others do not.
		this can be corrected but should have been implemented from the beginning.
	7) Some variables are Global and they should not be. 
	8) Not all sensible parameter validity checks and edge cases exist in all methods.
*/ 

/* 
MUTEX HEIRARCHY (Highest first) 

	- BarrierMutex
	- UnloadingQuayMutex
	- CanalToMedSeaMutex, CanalToRedSeaMutex (DO NOT AQUIRE SIMULTANIOUSLY)
	- ReportMutex
	- RandObjectMutex

	!!! Never AQUIRE a mutex from this list while HOLDING a mutex below it in the Heirarchy !!!
	(this includes Releasing and Re-Aquiring)
*/

/* PROJECT TODOs*/
//TODO - heavily refactor + reformat code
//TODO - move all constant strings and numbers to MACRO or constexpr.
//TODO - document remaining methods/constants/vars in all files (I missed a bunch, started but tedious to finish and
//	usually did not document as I wrote the code due to laziness)
//TODO - generalize resource allocation/de-allocation
//TODO - move common methods to seperate headers and implementations and create MAKE file
//	to build compilation objects in proper order (at least I think I need a MAKE file for that...)
//TODO - consider using "extern" as solution for common processes global variables,
//	or try passing process information and thread parameters
//TODO - consider cancaling some methods and inlining some more code into existing methods 
//	(perhaps some methods aren't providing a strong enough service or don't appear to be common)

#define _CRT_SECURE_NO_WARNINGS
#define ARRAY_VESSEL_ID vesselId - 1

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Constants.h"
#include "Structures.h"

/**
 * @brief checks CL arguments for compatability with EXAM requirements.
 * 
 * @param argc argument count. given by OS. value is 0 if no argument supplied.
 * @param argv argument vector. array of strings containing CL arguments.
 *             argv[0] contains process name if available from OS and \0 o.w
 */
void argChecks(int argc, char **argv);
/**
 * @brief closes all allocated resources non-generically, meaning, any new resource allocated
 *        needs to be added to have code added to this method which is poor but good enough with
 *        two kids at home :)
 * 
 * @param vessels pointer to a structure containing allocation HANDLES of vessel threads
 */
void CloseAllHandles(Vessels *vessels);
/**
 * @brief Creates all Semaphore objects neccessary for Haifa Port, including some NAMED Mutexes
 * 			later referenced by Eilat Port.
 * 
 * @return int - True if successfull, False o.w
 */
int CreateSemaphores();
/**
 * @brief Create all annonymous pipes used from communication between Haifa
 * 		process and Eilat process.
 * 		Also redirects standart I/O of Eilat process info to appropriate pipe ends.
 * 
 * @return int - True if successful, False o.w
 */
int		CreateSuezCanal();
void 	CreateEilatPort();
void 	departVessels(int clusterSize, Vessels *vessels);
/**
 * @brief Zeros and Nulls all HANDLEs/Pointers/Variables...
 * 
 * 			Does not preform null check on parameter vessels.
 * 
 * @param vessels A pointer to a structure containing all vessel thread handles.
 */
void 	initHandles(Vessels *vessels);
void 	recieveVessels(int clusterSize);
int 	recieveViaSuez(char *buffer);
void 	sailViaSuez(char *buffer, int vesselId);
/**
 * @brief Seeds the internal rand object using some terrible combination of the current system time
 * 			and the addresses of some local variables around where seedRandom is called.
 * 
 * @sync RandObjectMutex - All code in this method is run in the context of an aquired mutex.
 * 
 * @param internalVariableA 
 * @param internalVariableB 
 */
void 	seedRandom(int *internalVariableA, int *internalVariableB);
int 	sendClusterSize(int clusterSize, char *buffer);
void 	sendViaSuez(char *buffer);
int 	calcSleepTime();
void 	checkSemaphoreAlloc(HANDLE semaphore);
int 	randFromTo(int from, int to);
void 	report(char *message);
/**
 * @brief 
 * 
 * @param Param 
 * @return DWORD 
 */
DWORD 	WINAPI Vessel(PVOID Param);

/*Note - some of these variables don't belong as globals, but since it simplifies many methods by making
	them argument-less, i'm willing to cheat a little.
	Exposing all of these variables is a recipe for linking trouble and all sorts of nasty stuff,
	in a sustainable multithreaded full life-cycle application,
	but i'm cutting corners here to make the code more readable for myself without wasting too much time,
	with elaborate methods*/

HANDLE canalToRedSeaMutex;					   //to protect annonymous pipe
HANDLE ReportMutex;							   //to protect prints
HANDLE RandObjectMutex;						   //to protect "as far as one can :(", lousy library rand implementations
HANDLE VesselsOnEilatVoyage[MAX_CLUSTER_SIZE]; //for vessel threads to wait on while in Eilat
HANDLE EilatReadsFromHandle;				   //annonymous pipe end
HANDLE HaifaWritesToHandle;					   //annonymous pipe end
HANDLE HaifaReadsFromHandle;				   //annonymous pipe end
HANDLE EilatWritesToHandle;					   //annonymous pipe end
DWORD written, read;						   //statistics for reading from, and writing to, annonymous pipes
PROCESS_INFORMATION EilatPort_pi;			   // process information structure to populate before creating Eilat Port Process

CHAR globalPrintBuffer[STR_BUFF_SIZE];

//MAIN

int main(int argc, char *argv[])
{
	argChecks(argc, argv); //validate recieved arguments
	PRINT("Haifa Port - Cluster size is %d\n", atoi(argv[1]));
	Vessels vessels;
	/* INIT new process parameters and general use parameters */
	initHandles(&vessels);

	CHAR buffer[STR_BUFF_SIZE];
	int clusterSize = atoi(argv[1]);

	//run create procedures
	if (CreateSemaphores() != True)
	{
		ERROR_PRINT("ERROR! - Haifa main thread encountered an error\n");
		CloseAllHandles(&vessels);
		exit(EXIT_FAILURE);
	}
	CreateSuezCanal();
	CreateEilatPort();
	// negotiate cluster size with eilat port
	PRINT("Haifa Port - requesting passage to Eilat Port...\n");
	if (sendClusterSize(clusterSize, buffer) == False)
	{
		PRINT("Haifa Port - Eilat Port DENIED passage request\n");
		WaitForSingleObject(EilatPort_pi.hProcess, INFINITE);
		PRINT("Haifa Port - Exiting...\n");
		CloseAllHandles(&vessels);
		exit(EXIT_FAILURE);
	}
	//Normal flow...
	PRINT("Haifa Port - Eilat Port ACCEPTED passage request\n");
	// send vessels to Eilat
	departVessels(clusterSize, &vessels);
	/* wait for vessels to return to Haifa port */
	recieveVessels(clusterSize);

	/* wait for Eilat's port to terminate it's main */
	WaitForSingleObject(EilatPort_pi.hProcess, INFINITE);

	//JOIN on all vessel threads
	WaitForMultipleObjects(clusterSize, vessels.VesselThreadHandles, TRUE, INFINITE);
	PRINT("Haifa Port - All Vessel Threads are done\n");

	PRINT("Haifa Port - Exiting...\n");
	CloseAllHandles(&vessels);
	return EXIT_SUCCESS;
}

//AUX function implementations
void argChecks(int argc, char **argv)
{
	//Argument count Error flow
	if (argc != 2)
	{
		if (argc == 1)
			printf(NO_CMD_ARGS);
		else
			printf(MANY_CMD_ARGS);
		exit(EXIT_FAILURE);
	}
	//Error flow - 0 vessels - to seperate case of 0 return value from atoi()
	if (strlen(argv[1]) == 1 && argv[1][0] == '0')
	{ //check if command line argument is the character '0'
		printf(EXCEEDS_CMD_ARGS);
		exit(EXIT_FAILURE);
	}
	int clusterSize = atoi(argv[1]); //note - this will accept a parameter with prefixed number such as "54banana".
									 //note - this will possibly fail if argument exceeds int range representation. //TODO character count limit
	//Error flow - NAN
	if (clusterSize == 0)
	{ //argument does not start with an integral number
		printf(ARG_IS_NAN);
		exit(EXIT_FAILURE);
	}
	//Error flow - parameter value small/large
	if (clusterSize < MIN_CLUSTER_SIZE || clusterSize > MAX_CLUSTER_SIZE)
	{
		printf(EXCEEDS_CMD_ARGS);
		exit(EXIT_FAILURE);
	}
	//Normal flow
}

int CreateSuezCanal()
{
	/* set up security attributes so that pipe handles are inherited */
	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

	/* create Haifa to Eilat lane */
	if (!CreatePipe(&EilatReadsFromHandle, &HaifaWritesToHandle, &sa, 0))
	{ // not sure if sa is referenced or copied. perhaps scope issue after returning from this function.
		PRINT("Create \"Haifa to Eilat\" Lane Failed\n");
		return False;
	}

	/* we do not want the child to inherit opposite ends of the pipes */
	SetHandleInformation(HaifaWritesToHandle, HANDLE_FLAG_INHERIT, 0);

	/* create Eilat to Haifa lane */
	if (!CreatePipe(&HaifaReadsFromHandle, &EilatWritesToHandle, &sa, 0))
	{
		PRINT("Create \"Eilat to Haifa\" Lane Failed\n");
		return False;
	}

	/* we do not want the child to inherit opposite ends of the pipes */
	SetHandleInformation(HaifaReadsFromHandle, HANDLE_FLAG_INHERIT, 0);
	return True;
}

void CreateEilatPort()
{
	TCHAR ProcessName[256];
	STARTUPINFO si;

	/* establish the START_INFO structure for the child process */
	GetStartupInfo(&si);
	si.hStdError = GetStdHandle(STD_OUTPUT_HANDLE); //use error stream for console output (is by default but i don't care)
	si.hStdOutput = EilatWritesToHandle;

	/* redirect the standard input to the read end of the pipe */
	si.hStdInput = EilatReadsFromHandle;
	si.dwFlags = STARTF_USESTDHANDLES;
	//absolute path avoids problems when debugger runs at a different active directory than windows cmd line.
	//this way, running from project debugger and running from cmd line both find the file.
	//this can be replaced to relative path at release.
	wcscpy(ProcessName, L"C:\\Users\\Yael\\Documents\\Operating Systems\\EXAM\\Debug\\EilatPort.exe");

	/* create the child process */
	if (!CreateProcess(NULL,
					   ProcessName,
					   NULL,
					   NULL,
					   TRUE, /* inherit handles */
					   0,
					   NULL,
					   NULL,
					   &si,
					   &EilatPort_pi))
	{
		fprintf(stderr, "Eilat Port Creation Failed\n");
		exit(EXIT_FAILURE);
	}

	/* close the pipe handles not used by Haifa port (does not close the handle that Eilat holds) */
	CloseHandle(EilatReadsFromHandle);
	CloseHandle(EilatWritesToHandle);
}

int CreateSemaphores()
{
	//Initialise Semaphores
	canalToRedSeaMutex = CreateMutex(NULL, FALSE, NULL);
	checkSemaphoreAlloc(canalToRedSeaMutex);
	ReportMutex = CreateMutex(NULL, FALSE, TEXT("_Haifa_Eilat_report_Mutex")); //Named mutex! - used for sharing with Eilat port
	checkSemaphoreAlloc(ReportMutex);
	RandObjectMutex = CreateMutex(NULL, FALSE, TEXT("_Haifa_Eilat_Rand_Mutex")); //Named mutex! - used for sharing with Eilat port
	checkSemaphoreAlloc(RandObjectMutex);

	//initialize a scedualing constraint semaphore for each Vessel with initial value of 0 and max value 1.
	// once a Vessel sails to Eilat, It's thread WAITs on it's own semaphore, and will be signaled
	// by the returning Eilat Vessal.
	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i)
	{
		VesselsOnEilatVoyage[i] = CreateSemaphore(NULL, 0, 1, NULL);
		if (VesselsOnEilatVoyage[i] == NULL)
		{
			PRINT("Error! semaphore allocation error.\n");
			return False;
		}
	}
	return True;
}

DWORD WINAPI Vessel(PVOID Param)
{
	char reportBuffer[STR_BUFF_SIZE];
	int vesselId = *(int *)Param;
	seedRandom((int *)&reportBuffer, (int *)&vesselId);
	//report Voyage started and sleep
	PRINT(VOYAGE_STARTED, vesselId);
	SLEEP;

	//enter Canal to Red Sea via appropriate lane
	sprintf(reportBuffer, "%d", vesselId);
	sailViaSuez(reportBuffer, vesselId);

	PRINT("Vessel %d - done sailing @ Haifa Port\n", vesselId);
	SLEEP;

	return EXIT_SUCCESS;
}

void sendViaSuez(char *buffer)
{
	AQUIRE(canalToRedSeaMutex);

	if (!WriteFile(HaifaWritesToHandle, buffer, STR_BUFF_SIZE, &written, NULL))
	{
		fprintf(stderr, "Haifa Error writing to pipe\n");
		exit(EXIT_FAILURE);
	}

	RELEASE(canalToRedSeaMutex);
}

void sailViaSuez(char *buffer, int vesselId)
{
	AQUIRE(canalToRedSeaMutex);

	PRINT("Vessel %d - entering Canal: Med. Sea ==> Red Sea\n", vesselId);
	SLEEP;

	if (!WriteFile(HaifaWritesToHandle, buffer, STR_BUFF_SIZE, &written, NULL))
	{
		PRINT("Haifa Error writing to pipe\n");
		exit(EXIT_FAILURE);
	}

	RELEASE(canalToRedSeaMutex);
	//wait for return from Eilat port
	SCHEDULING_CONSTRAINT(VesselsOnEilatVoyage[ARRAY_VESSEL_ID]);
}

int recieveViaSuez(char *buffer)
{
	if (!ReadFile(HaifaReadsFromHandle, buffer, STR_BUFF_SIZE, &read, NULL))
	{
		PRINT("Haifa Error reading from pipe\n");
		exit(EXIT_FAILURE);
	}
	return atoi(buffer);
}

void CloseAllHandles(Vessels *vessels)
{ //this should be made generic by using a structure to hold all allocated
	//resources but i'm trying to get everything to run within exam time limits, and will address if I have time...
	if (HaifaReadsFromHandle != NULL)
		CloseHandle(HaifaReadsFromHandle);
	if (HaifaWritesToHandle != NULL)
		CloseHandle(HaifaWritesToHandle);
	if (canalToRedSeaMutex != NULL)
		CloseHandle(canalToRedSeaMutex);
	if (ReportMutex != NULL)
		CloseHandle(ReportMutex);
	if (EilatPort_pi.hProcess != NULL)
		CloseHandle(EilatPort_pi.hProcess);
	if (EilatPort_pi.hThread != NULL)
		CloseHandle(EilatPort_pi.hThread);
	if (RandObjectMutex != NULL)
		CloseHandle(RandObjectMutex);
	//TODO - close vessel threads
	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i)
	{
		if (VesselsOnEilatVoyage[i] != NULL)
			CloseHandle(VesselsOnEilatVoyage[i]);
		if (vessels->VesselThreadHandles[i] != NULL)
			CloseHandle(vessels->VesselThreadHandles[i]);
	}
}

void initHandles(Vessels *vessels)
{ //this should be made generic by using a structure to hold all allocated
	//resources but i'm trying to get everything to run within exam time limits, and will address if I have time...
	canalToRedSeaMutex = NULL;
	ReportMutex = NULL;
	EilatReadsFromHandle = NULL;
	HaifaWritesToHandle = NULL;
	HaifaReadsFromHandle = NULL;
	EilatWritesToHandle = NULL;
	ZeroMemory(&EilatPort_pi, sizeof(EilatPort_pi));
	EilatPort_pi.hProcess = NULL;
	EilatPort_pi.hThread = NULL;
	RandObjectMutex = NULL;

	for (int i = 0; i < MAX_CLUSTER_SIZE; ++i)
	{
		vessels->dwVesselsThreadIdArray[i] = 0;
		vessels->VesselThreadHandles[i] = NULL;
		vessels->VesselThreadsId[i] = NOT_SET;
	}
}

int sendClusterSize(int clusterSize, char *buffer)
{
	/* send Eilat size of cluster */
	int response;
	sprintf(buffer, "%d", clusterSize);
	sendViaSuez(buffer);
	recieveViaSuez(buffer);
	response = atoi(buffer);
	//Eilat rejected cluster or sent non integer response or sent integer != 1
	if (response != True)
	{
		return False;
	}
	// Eilat Accepted
	else
		return True;
}

void seedRandom(int *internalVariableA, int *internalVariableB)
{
	AQUIRE(RandObjectMutex);
	// some bad pseudo randomness
	int seed = (int)internalVariableA + (int)internalVariableB + (int)time(NULL);
	srand(seed);

	RELEASE(RandObjectMutex);
}

//assumes no Vessel returns from Eilat before this method returns.
//can be improved using scheduling constraints.
void departVessels(int clusterSize, Vessels *vessels)
{
	/*Initialise Vessels in Array (according to cluster size)*/
	for (int i = 0; i < clusterSize; ++i)
	{
		vessels->VesselThreadsId[i] = i + 1;
		vessels->VesselThreadHandles[i] = CreateThread(NULL, 0, Vessel,
													   &vessels->VesselThreadsId[i], 0, &vessels->dwVesselsThreadIdArray[i]);
		//Error - OS syscall error creating new thread
		if (vessels->VesselThreadHandles[i] == NULL)
		{
			PRINT("main::Unexpected Error in Thread %d  Creation\n", i);
			exit(EXIT_FAILURE);
		}
	}
}

void recieveVessels(int clusterSize)
{
	CHAR buffer[STR_BUFF_SIZE];
	int recievedVesselId = 0, semaphoreSuccess = False;
	//listen for cluster size number of vessels returning.
	for (int i = 0; i < clusterSize; ++i)
	{
		recievedVesselId = recieveViaSuez(buffer);
		// Error - case Eilat sent bad parameter through canal (i.e NAN or ID = 0)
		if (recievedVesselId == 0)
		{
			PRINT("bad things went down at recieveVessels() Haifa port\n");
			exit(EXIT_FAILURE);
		}
		//Release vessel to finish voyage at Haifa Port
		semaphoreSuccess = END_SCHEDULING_CONSTRAINT(VesselsOnEilatVoyage[recievedVesselId - 1]);
		if (!semaphoreSuccess)
		{
			PRINT("Error! semaphore Release error\n");
			exit(EXIT_FAILURE);
		}
	}
}

int calcSleepTime()
{
	return randFromTo(MIN_SLEEP_TIME, MAX_SLEEP_TIME);
}

void checkSemaphoreAlloc(HANDLE semaphore)
{
	if (semaphore == NULL)
	{
		printf("Haifa CreateMutex error: %d\n", GetLastError());
		exit(EXIT_FAILURE);
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		printf("Haifa CreateMutex opened an existing mutex\n");
	}
}

//Needs ReportMutex Synchronization
void report(char *message)
{
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

	fprintf(stderr, "[%s:%s:%s] %s", hours, minutes, seconds, message);
}

int randFromTo(int from, int to)
{
	if (from < 0 || from >= to)
		return -1;

	AQUIRE(RandObjectMutex);

	int generated = ((rand() % (to - from + 1)) + from);

	RELEASE(RandObjectMutex);

	return generated;
}