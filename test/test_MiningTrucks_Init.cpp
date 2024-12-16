#include <gtest/gtest.h>
#include <MiningTruckProcessor.h>

using namespace std;

// Test case for the MiningTruckUpdate function
TEST(MiningSimulationTests, TestMiningTrucksUpdate) {
    // Declare constants
    const size_t numMiningTrucks{1};
    float minMiningDuration_hrs{1};
    float maxMiningDuration_hrs{5};
    float travelDuration_hrs{0.5};
    float unloadDuration_min{5};
    float simulationTimestep{5}; // minutes
    float simulationTime{}; // minutes

    // Initialize Mining Truck Processor
    MiningTrucksProcessor miningTrucksProcessor(numMiningTrucks, minMiningDuration_hrs, maxMiningDuration_hrs, travelDuration_hrs, unloadDuration_min);

    // Run Simulation for full mining time
    const float miningCycleTime{miningTrucksProcessor.m_MiningTrucksList[0].miningCycleDuration};
    simulationTime = miningCycleTime;
    float currentSimTime{};
    while(currentSimTime < simulationTime){
        EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::MINING);        
        miningTrucksProcessor.updateMiningTrucks(simulationTimestep);
        currentSimTime+= simulationTimestep;
    }

    // Verify Truck has changed to travel state
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::TRAVEL);

    // Verify truck is loaded
    EXPECT_TRUE(miningTrucksProcessor.m_MiningTrucksList[0].isLoaded);

    // Run simulation for travel time
    simulationTime += travelDuration_hrs * 60;
    while(currentSimTime < simulationTime){
        EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::TRAVEL);
        miningTrucksProcessor.updateMiningTrucks(simulationTimestep);
        currentSimTime+= simulationTimestep;
    }

    // Verify Truck has changed to Unload state
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::UNLOAD);

    // Verify truck index added to list of trucks for unload
    const vector<int> loadedTrucks{miningTrucksProcessor.getLoadedTrucks()};
    EXPECT_EQ(loadedTrucks.size(), 1);
    EXPECT_EQ(loadedTrucks[0], 0);

    // Verify truck is loaded
    EXPECT_TRUE(miningTrucksProcessor.m_MiningTrucksList[0].isLoaded);

    // Run simulation for 10 more timesteps without unloading
    simulationTime += simulationTimestep * 10;
    while(currentSimTime < simulationTime){
        EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::UNLOAD);
        miningTrucksProcessor.updateMiningTrucks(simulationTimestep);
        currentSimTime+= simulationTimestep;
    }

    // Verify Truck remained in unload state
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::UNLOAD);

    // Verify number of unloads is 0
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].numUnloads, 0);

    // Verify truck is loaded
    EXPECT_TRUE(miningTrucksProcessor.m_MiningTrucksList[0].isLoaded);

    // Simulate truck unload
    miningTrucksProcessor.m_MiningTrucksList[0].isLoaded = false;

    // Run simulation for unload time
    simulationTime += unloadDuration_min;
    while(currentSimTime < simulationTime){
        EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::UNLOAD);
        miningTrucksProcessor.updateMiningTrucks(simulationTimestep);
        currentSimTime+= simulationTimestep;
    }

    // Verify Truck has changed to travel state
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::TRAVEL);

     // Run simulation for travel time
    simulationTime += travelDuration_hrs * 60;
    while(currentSimTime < simulationTime){
        EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::TRAVEL);
        miningTrucksProcessor.updateMiningTrucks(simulationTimestep);
        currentSimTime+= simulationTimestep;
    }

    // Verify Truck has changed back to mining state
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].state, TruckStates::MINING);

    // Verify truck is unloaded
    EXPECT_FALSE(miningTrucksProcessor.m_MiningTrucksList[0].isLoaded);

    // Verify number of travel cycles is accurate
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].numTravelCycles, ceil(2 * travelDuration_hrs * 60/simulationTimestep));

    // Verify number of unloading cycles is accurate
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].numUnloads, ceil(unloadDuration_min/simulationTimestep));

    // Verify number of mining cycles is accurate
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList[0].numMiningCycles, ceil(miningCycleTime/simulationTimestep));
}