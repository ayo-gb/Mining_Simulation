#ifndef MINING_TRUCK_H
#define MINING_TRUCK_H

using namespace std;

 /**
  * @brief Defines the different states a vehicle can be in during simulation
  */
enum TruckStates {
    MINING,
    TRAVEL,
    UNLOAD
    };

/**
* @brief  Constructs new 'Truck' object
*/
struct Truck{
    const int id;                       // Truck ID
    const float miningCycleDuration;    // Truck Mining time (min)
    int state;                          // Current truck state
    float timeUntilNextState;           // Time until next state (min)
    bool isLoaded;                      // Truck load status
    bool isAssignedStation;             // Truck assignment status
    float numTravelCycles;              // Number of travel cycles completed by truck
    float numMiningCycles;              // Number of mining cycles completed by truck
    int numUnloads;                     // Number of truck unloads

    // Parameterized constructor
    Truck(const int id, const float mining_time) : id(id), miningCycleDuration(mining_time), state(TruckStates::MINING),
                                        timeUntilNextState(mining_time), isLoaded(false), isAssignedStation(false), numTravelCycles(0), 
                                        numMiningCycles(0), numUnloads(0) {}
};

/**
* @brief  Constructs new 'TruckPerformance' object
*/
struct TruckPerformanceStats{
    const int vehicleId;            // Vehicle Id
    float percentMiningTime;        // Percentage of time truck spent in mining state
    float percentTravelTime;        // Percentage of time truck spent in travel state
    float percentUnloadingTime;     // Percentage of time truck spent in unloading
    float percentIdleTime;          // Percentage of time truck spent idle (awaiting unload) state
    float totalMiningTime_hrs;      // Total hours the truck spent mining
    float totalUnloads;             // Total number of unloads

    // Parameterized constructor
    TruckPerformanceStats(const int id) : vehicleId(id), percentMiningTime(0), percentTravelTime(0), 
                                            percentUnloadingTime(0), percentIdleTime(0), totalMiningTime_hrs(0), totalUnloads(0) {}
};

#endif // MINING_TRUCK_H