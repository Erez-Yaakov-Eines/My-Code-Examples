#pragma once

#define STR_BUFF_SIZE 256
#define MAX_SLEEP_TIME 3000//milliseconds
#define MIN_SLEEP_TIME 5//milliseconds
#define ERROR_SLEEP_TIME 5000
#define MIN_CLUSTER_SIZE 2
#define MAX_CLUSTER_SIZE 50 //No more than MAXIMUM_WAIT_OBJECTS (64 I guess...)
#define MAX_FIFO_SIZE MAX_CLUSTER_SIZE
#define MIN_CRANE_NUM 2
#define MIN_CARGO_WEIGHT 5
#define MAX_CARGO_WEIGHT 50

#define EXIT_SUB_PROCESS_FAILURE 128

#define True 1
#define False 0
#define INCREMENT_ONCE 1
#define COUNT_NOT_REQ NULL
#define EMPTY_ELEMENT -1
#define NO_VESSEL EMPTY_ELEMENT
#define NOT_SET 0
#define EMPTY 0
#define UNOCCUPIED -1
#define END_CRANE_LIFE -5

#define NO_CMD_ARGS "Error! - no command line arguments provided\nProper Usage: :\\> HaifaPort.exe 12\n"
#define MANY_CMD_ARGS "too many command line arguments provided\nProper Usage: :\\> HaifaPort.exe 12\n"
#define EXCEEDS_CMD_ARGS "argument value not between %d-%d\n",MIN_CLUSTER_SIZE,MAX_CLUSTER_SIZE
#define ARG_IS_NAN "NAN argument recieved.\n Please provide a single numeric argument\n"
#define VOYAGE_STARTED "Vessel %d - starts sailing @ Haifa Port\n"
#define EILAT_PROCESS_ERROR "Error! - Eilat process encountered an error\n"
#define EILAT_VESSELS_FINISHED "Eilat Port - All Vessel Threads are done\n"
#define EILAT_CRANES_FINISHED "Eilat Port - All cranes finished\n"
#define EILAT_PORT_EXIT "Eilat Port - Exiting...\n"

#define SLEEP Sleep(calcSleepTime())

//PRINT Macro might wait on print mutex before printing so print time is when mutex was aquired
//	not necessarily when Macro was used. also some prints occur outside of any other mutex lock
//	and might be context switched before they were supposed to occur, giving a different tiiming even if
//	the above didn't matter.
#define PRINT(...) do { WaitForSingleObject(ReportMutex, INFINITE);\
								sprintf(globalPrintBuffer, __VA_ARGS__);\
								report(globalPrintBuffer);\
								ReleaseMutex(ReportMutex);\
								} while(0)
//Prints and sleeps. Used for running batch testing and having process not exit immediately and
//	move to next test. TO DISABLE add MACRO to replace ERROR_PRINT with PRINT.
//	perhaps I should have made this more automatic using #ifndef DEBUG or similar...
#define ERROR_PRINT(...) do { WaitForSingleObject(ReportMutex, INFINITE);\
								sprintf(globalPrintBuffer, __VA_ARGS__);\
								report(globalPrintBuffer);\
								sprintf(globalPrintBuffer, "Sleeping momentarily...\n");\
								report(globalPrintBuffer);\
								ReleaseMutex(ReportMutex);\
								} while(0);\
								Sleep(ERROR_SLEEP_TIME)


#define AQUIRE(X) WaitForSingleObject(X, INFINITE)
#define RELEASE(X) ReleaseMutex(X)
#define SCHEDULING_CONSTRAINT(X) WaitForSingleObject(X, INFINITE)
#define END_SCHEDULING_CONSTRAINT(X) ReleaseSemaphore(X, INCREMENT_ONCE, COUNT_NOT_REQ);
