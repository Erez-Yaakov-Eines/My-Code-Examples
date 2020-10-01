#pragma once

typedef struct cyclic_fifo_queue {
	int Array[MAX_CLUSTER_SIZE];
	int head;
	int tail;
	int size;
}FifoQueue;

typedef struct crane_threads {
	HANDLE CraneThreadHandles[MAX_CLUSTER_SIZE];
	int CraneThreadsId[MAX_CLUSTER_SIZE];
	DWORD dwCranesThreadIdArray[MAX_CLUSTER_SIZE];
}Cranes;

typedef struct _vessel_threads {
	HANDLE VesselThreadHandles[MAX_CLUSTER_SIZE];
	int VesselThreadsId[MAX_CLUSTER_SIZE];
	DWORD dwVesselsThreadIdArray[MAX_CLUSTER_SIZE];
}Vessels;

typedef struct _barrier {
	FifoQueue vesselQ;
	int M;
}Barrier;

typedef struct unloadingQuayADT {
	HANDLE* Cranes;
	int vesselCount;
	int craneWeightsForAssignedVessel[MAX_CLUSTER_SIZE];
	int cranesOccupied[MAX_CLUSTER_SIZE];//also keeps vessel ID's
	int numberOfCranes;
	int isBusy;
	int lastSubClusterVessel;
} UnloadingQuay;