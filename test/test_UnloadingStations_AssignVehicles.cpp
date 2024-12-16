#include <gtest/gtest.h>
#include <Simulation.h>
#include <MiningTruckProcessor.h>
#include <UnloadingStationProcessor.h>

using namespace std;

// Checks if a value exists in the queue
bool valueInQueue(const queue<int>& q, int value) {
    // Create a copy of the queue
    std::queue<int> tempQueue = q;

    // Traverse through the queue
    while (!tempQueue.empty()) {
        if (tempQueue.front() == value) {
            return true; // return true if value found
        }
        tempQueue.pop();
    }

    return false; // return false if value not found
}

// Test case for the MiningTruckProcessor initialization
TEST(MiningSimulationTests, TestUnloadingStationsAssignVehicles) {
    // Declare constants
    const size_t numUnloadingStations{5};
    const size_t numMiningVehicles{5};
    float minMiningDuration_hrs{1};
    float maxMiningDuration_hrs{5};
    float travelDuration_hrs{0.5};
    float unloadDuration_min{5};

    // Initialize Unloading Station Processor
    UnloadingStationProcessor unloadingStationProcessor(numUnloadingStations, unloadDuration_min);

    // Create 5 Trucks
    vector<Truck> miningTrucks{};
    miningTrucks.push_back(Truck(0, 0)); // T1
    miningTrucks.push_back(Truck(1, 0)); // T2
    miningTrucks.push_back(Truck(2, 0)); // T3
    miningTrucks.push_back(Truck(3, 0)); // T4
    miningTrucks.push_back(Truck(4, 0)); // T5
    
    // Set trucks to be loaded
    for(Truck& truck: miningTrucks){
        truck.isLoaded = true;
    }

    // Add unloaded 6th truck
    miningTrucks.push_back(Truck(5, 0)); // T6

    // Create vector with indices of loaded trucks
    vector<int> loadedTruckIdxs{0, 1, 2, 3, 4};

    // Set S2 (3 vehicles), S3 (4 vehicles), S5 (1 vehicle) to have wait times
    unloadingStationProcessor.m_UnloadingStationsList[1].waitTime = 3 * unloadDuration_min;
    unloadingStationProcessor.m_UnloadingStationsList[2].waitTime = 4 * unloadDuration_min;
    unloadingStationProcessor.m_UnloadingStationsList[4].waitTime = unloadDuration_min;

    // Set S1 and be S4 to available stations
    queue<int> availableStationIdx;
    availableStationIdx.push(0);
    availableStationIdx.push(3);
    unloadingStationProcessor.setAvailableLoadingStations(availableStationIdx);

    // Run assign vehicles
    unloadingStationProcessor.assignVehiclesToStations(miningTrucks, loadedTruckIdxs);

    // Check where each vehicle assigned (T1 = S1, T2 = S4, T3 = S1, T4 = S4, T5 = S5)

    // Verify S1 has been assigned vehicles T1 and T3
    queue<int> s1_vehicleQueue{unloadingStationProcessor.m_UnloadingStationsList[0].vehicleIdQueue};
    EXPECT_EQ(s1_vehicleQueue.size(), 2);
    EXPECT_TRUE(valueInQueue(s1_vehicleQueue, 0));
    EXPECT_TRUE(valueInQueue(s1_vehicleQueue, 2));

    // Verify S4 has been assigned vehicles T2 and T4
    queue<int> s4_vehicleQueue{unloadingStationProcessor.m_UnloadingStationsList[3].vehicleIdQueue};
    EXPECT_EQ(s4_vehicleQueue.size(), 2);
    EXPECT_TRUE(valueInQueue(s4_vehicleQueue, 1));
    EXPECT_TRUE(valueInQueue(s4_vehicleQueue, 3));

    // Verify S5 has been assigned vehicle T5
    queue<int> s5_vehicleQueue{unloadingStationProcessor.m_UnloadingStationsList[4].vehicleIdQueue};
    EXPECT_EQ(s5_vehicleQueue.size(), 1);
    EXPECT_TRUE(valueInQueue(s5_vehicleQueue, 4));

    // Verify S2 and S3 have not been assigned vehicles 
    queue<int> s2_vehicleQueue{unloadingStationProcessor.m_UnloadingStationsList[1].vehicleIdQueue};
    EXPECT_EQ(s2_vehicleQueue.size(), 0);

    queue<int> s3_vehicleQueue{unloadingStationProcessor.m_UnloadingStationsList[2].vehicleIdQueue};
    EXPECT_EQ(s3_vehicleQueue.size(), 0);

    // Verify all loaded stations have been assigned stations
    EXPECT_TRUE(miningTrucks[0].isAssignedStation);
    EXPECT_TRUE(miningTrucks[1].isAssignedStation);
    EXPECT_TRUE(miningTrucks[2].isAssignedStation);
    EXPECT_TRUE(miningTrucks[3].isAssignedStation);
    EXPECT_TRUE(miningTrucks[4].isAssignedStation);

    // Verify unloaded station has not been assigned a station
    EXPECT_FALSE(miningTrucks[5].isAssignedStation);
}