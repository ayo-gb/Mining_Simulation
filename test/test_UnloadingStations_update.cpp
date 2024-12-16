#include <gtest/gtest.h>
#include <Simulation.h>
#include <MiningTruckProcessor.h>
#include <UnloadingStationProcessor.h>

using namespace std;

// Test case for the MiningTruckProcessor initialization
TEST(MiningSimulationTests, TestUnloadingStationsUpdate) {
    // Declare constants
    const size_t numUnloadingStations{1};
    const size_t numMiningVehicles{5};
    float travelDuration_hrs{0.5};
    float unloadDuration_min{5};
    float simulationTime{};
    float simulationTimestep{5}; // minutes

    // Initialize Unloading Station Processor
    UnloadingStationProcessor unloadingStationProcessor(numUnloadingStations, unloadDuration_min);

    // Create vector of 3 trucks
    vector<Truck> miningTrucks{};
    miningTrucks.push_back(Truck(0, 0)); // T1
    miningTrucks.push_back(Truck(1, 0)); // T2
    miningTrucks.push_back(Truck(2, 0)); // T3

    // Run Simulation for 15 minutes
    simulationTime = 30;
    float currentSimTime{};
    while(currentSimTime < simulationTime){
        EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].state, UnloadingStationStates::AVAILABLE);        
        unloadingStationProcessor.updateUnloadingStations(simulationTimestep, miningTrucks);
        currentSimTime+= simulationTimestep;
    }

    // Verify station has remained available
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].state, UnloadingStationStates::AVAILABLE);

    // Verify wait time is 0
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].waitTime, 0);

    // Simulate loaded vehicle assigned to station
    miningTrucks[0].isAssignedStation = true;
    miningTrucks[0].isLoaded = true;
    queue<int> vehicleIdQueue;
    vehicleIdQueue.push(0);
    unloadingStationProcessor.m_UnloadingStationsList[0].vehicleIdQueue = vehicleIdQueue;

    // Run Simulation for one timestep
    simulationTime += simulationTimestep;
    while(currentSimTime < simulationTime){       
        unloadingStationProcessor.updateUnloadingStations(simulationTimestep, miningTrucks);
        currentSimTime+= simulationTimestep;
    }

    // Verify station changed state to occupied
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].state, UnloadingStationStates::OCCUPIED);

    // Verify wait time equal to unload duration
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].waitTime, unloadDuration_min);

    // Run simulation for unload duration
    simulationTime += unloadDuration_min;
    while(currentSimTime < simulationTime){
        EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].state, UnloadingStationStates::OCCUPIED);
        unloadingStationProcessor.updateUnloadingStations(simulationTimestep, miningTrucks);
        currentSimTime+= simulationTimestep;
    }

    // Verify vehicle is unloaded
    EXPECT_FALSE(miningTrucks[0].isLoaded);

    // Verify vehicle is unassigned
    EXPECT_FALSE(miningTrucks[0].isAssignedStation);

    // Verify station queue is empty
    EXPECT_TRUE(unloadingStationProcessor.m_UnloadingStationsList[0].vehicleIdQueue.empty());

    // Verify station changed state to available
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].state, UnloadingStationStates::AVAILABLE);

    // Verify wait time is 0
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].waitTime, 0);

    // Verify unload count is 1
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList[0].numVehiclesUnloaded, 1);
}