#include <gtest/gtest.h>
#include <MiningTruckProcessor.h>

using namespace std;

// Checks if all values in a vector are not the same
bool allValuesNotSame(const vector<float>& vec) {
    // If vector is empty, return false
    if (vec.empty()){
        return false;
    } 
    return !all_of(vec.begin(), vec.end(), [first = vec[0]](int value) {
        return value == first;
    });
}

// Test case for the MiningTruckProcessor initialization
TEST(MiningSimulationTests, TestMiningTrucksInit) {
    // Declare constants
    const size_t numMiningTrucks{5};
    float minMiningDuration_hrs{1};
    float maxMiningDuration_hrs{5};
    float travelDuration_hrs{0.5};
    float unloadDuration_min{5};

    // Initialize Mining Truck Processor
    MiningTrucksProcessor miningTrucksProcessor(numMiningTrucks, minMiningDuration_hrs, maxMiningDuration_hrs, travelDuration_hrs, unloadDuration_min);

    // Check number of trucks created
    EXPECT_EQ(miningTrucksProcessor.getNumMiningTrucks(), numMiningTrucks);
    EXPECT_EQ(miningTrucksProcessor.m_MiningTrucksList.size(), numMiningTrucks);

    // Check truck initializations are as expected
    vector<float> miningTimes{};
    for(int i = 0; i < numMiningTrucks; i++){
        Truck truck{miningTrucksProcessor.m_MiningTrucksList[i]};

        // Check Truck id = index
        EXPECT_EQ(truck.id, i);

        // Check initial state is mining
        EXPECT_EQ(truck.state, TruckStates::MINING);

        // Check all counts are 0
        EXPECT_EQ(truck.numMiningCycles, 0);
        EXPECT_EQ(truck.numTravelCycles, 0);
        EXPECT_EQ(truck.numUnloads, 0);

        // Check assignments are false
        EXPECT_FALSE(truck.isLoaded);
        EXPECT_FALSE(truck.isAssignedStation);

        // Expect mining time is greater or equal to 1 hr = 60 min
        EXPECT_GE(truck.miningCycleDuration, 60);

        // Expect mining time is less or equal to 5 hr = 5 * 60 min
        EXPECT_LE(truck.miningCycleDuration, 5*60);
        
        // Expect time until next state is equal to mining time
        EXPECT_EQ(truck.miningCycleDuration, truck.timeUntilNextState);

        // Check mining cycles are different
        miningTimes.push_back(truck.miningCycleDuration);
    }

    // Check mining cycles are not the same
    EXPECT_TRUE(allValuesNotSame(miningTimes));
}