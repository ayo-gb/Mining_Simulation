#include <gtest/gtest.h>
#include <Simulation.h>
#include <MiningTruckProcessor.h>
#include <UnloadingStationProcessor.h>

using namespace std;

// Test case for the MiningTruckProcessor initialization
TEST(MiningSimulationTests, TestSimulationComputePerformance) {
    // Declare constants
    const size_t numUnloadingStations{1};
    const size_t numMiningVehicles{1};
    const float travelDuration_hrs{0.5};
    const float unloadDuration_min{5};
    const float minMiningDuration_hrs{1};
    const float maxMiningDuration_hrs{5};
    const float simulationTime_hrs{6}; // hours
    const float simulationTime_min{simulationTime_hrs * 60}; // minutes
    const float simulationTimestep{5}; // minutes

    // Initialize Simulation
    Simulation miningSimulation(numMiningVehicles, numUnloadingStations, minMiningDuration_hrs, maxMiningDuration_hrs, travelDuration_hrs, 
                                unloadDuration_min, simulationTime_hrs, simulationTimestep);
    
    // Run simulation for 6 hours
    miningSimulation.run();

    // Get trucks and manually compute stats
    const Truck truck{miningSimulation.getMiningTrucks()[0]};
    double truckPercentUnloading{truck.numUnloads * unloadDuration_min/simulationTime_min * 100};
    double truckPercentMining{truck.numMiningCycles * simulationTimestep/simulationTime_min * 100};
    double truckPercentTravel{truck.numTravelCycles * simulationTimestep/simulationTime_min * 100};
    double truckTotalMining_hrs{truck.numMiningCycles * simulationTimestep/60};
    int truckTotalUnloads{truck.numUnloads};
    
    // Get current list of stations and manually compute stats
    const Station station{miningSimulation.getUnloadingStations()[0]};
    double stationPercentUnloading{station.numVehiclesUnloaded * unloadDuration_min/simulationTime_min * 100};
    double stationPercentIdle{100.0 - stationPercentUnloading};
    double stationTotalUnloading_hrs{station.numVehiclesUnloaded * unloadDuration_min/60};
    double stationTotalIdle_hrs{simulationTime_hrs - stationTotalUnloading_hrs};
    int stationTotalUnloads{station.numVehiclesUnloaded};

    // Compute stats with computePerformanceStats
    miningSimulation.computePerformanceStats();


    // Get computed performance stats
    const vector<StationPerformanceStats> computedStationPerformance{miningSimulation.getUnloadingStationPerformances()};
    const vector<TruckPerformanceStats> computedTruckPerformance{miningSimulation.getMiningTruckPerformances()};
    
    // Verify matches for station performance
    EXPECT_FLOAT_EQ(computedStationPerformance[0].percentIdleTime, stationPercentIdle);
    EXPECT_FLOAT_EQ(computedStationPerformance[0].percentUnloadingTime, stationPercentUnloading);
    EXPECT_FLOAT_EQ(computedStationPerformance[0].totalIdleTime_hrs, stationTotalIdle_hrs);
    EXPECT_FLOAT_EQ(computedStationPerformance[0].totalUnloadingTime_hrs, stationTotalUnloading_hrs);
    EXPECT_EQ(computedStationPerformance[0].totalUnloads,  stationTotalUnloads);


    // Verify matches for vehicle performance
    EXPECT_FLOAT_EQ(computedTruckPerformance[0].percentUnloadingTime, truckPercentUnloading );
    EXPECT_FLOAT_EQ(computedTruckPerformance[0].percentTravelTime, truckPercentTravel );
    EXPECT_FLOAT_EQ(computedTruckPerformance[0].percentMiningTime, truckPercentMining );
    EXPECT_FLOAT_EQ(computedTruckPerformance[0].totalMiningTime_hrs, truckTotalMining_hrs );
    EXPECT_EQ(computedTruckPerformance[0].totalUnloads, truckTotalUnloads);
}