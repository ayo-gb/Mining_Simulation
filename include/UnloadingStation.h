#ifndef UNLOADING_STATION_H
#define UNLOADING_STATION_H

#include <MiningTruckProcessor.h>
#include <queue>

using namespace std;

 /**
  * @brief Defines the different states a station can be in during simulation
  */
enum UnloadingStationStates {
    AVAILABLE,
    OCCUPIED,
    };

/**
* @brief  Constructs new 'Station' object
*/
struct Station{
    const int id;                   // Station ID
    int state;                      // Current station state
    float waitTime;                 // Current station state (min)
    queue<int> vehicleIdQueue;      // Queue of vehicles to be unloaded by station
    int numVehiclesUnloaded;        // Number of trucks unloaded

    // Parameterized constructor
    Station(const int id) : id(id), state(UnloadingStationStates::AVAILABLE), waitTime(0), numVehiclesUnloaded(0), vehicleIdQueue() {}
};

/**
* @brief  Constructs new 'StationPerformance' object
*/
struct StationPerformanceStats{
    const int stationId;            // Station ID
    float totalUnloads;             // Total number of unloads
    float percentUnloadingTime;     // Percentage of time station spent in unloading
    float percentIdleTime;          // Percentage of time station spent in unloading
    float totalUnloadingTime_hrs;   // Total number of hours spent unloading vehicles
    float totalIdleTime_hrs;        // Total number of hours spent idle

    // Parameterized constructor
    StationPerformanceStats(const int id) : stationId(id), totalUnloads(0), totalUnloadingTime_hrs(0), totalIdleTime_hrs(0), 
                                    percentUnloadingTime(0), percentIdleTime(0) {}
};

#endif // UNLOADING_STATION_H