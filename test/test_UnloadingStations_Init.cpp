#include <gtest/gtest.h>
#include <UnloadingStationProcessor.h>

using namespace std;

// Test case for the MiningTruckProcessor initialization
TEST(MiningSimulationTests, TestUnloadingStationsInit) {
    // Declare constants
    const size_t numUnloadingStations{5};
    float unloadDuration_min{5};

    // Initialize Unloading Station Processor
    UnloadingStationProcessor unloadingStationProcessor(numUnloadingStations, unloadDuration_min);

    // Check number of stations created
    EXPECT_EQ(unloadingStationProcessor.getNumUnloadingStations(), numUnloadingStations);
    EXPECT_EQ(unloadingStationProcessor.m_UnloadingStationsList.size(), numUnloadingStations);

    // Check unloading station initializations are as expected
    for(int i = 0; i < numUnloadingStations; i++){
        Station station{unloadingStationProcessor.m_UnloadingStationsList[i]};

        // Check Truck id = index
        EXPECT_EQ(station.id, i);

        // Check initial state is availaible
        EXPECT_EQ(station.state, UnloadingStationStates::AVAILABLE);

        // Check all counts are 0
        EXPECT_EQ(station.numVehiclesUnloaded, 0);
        EXPECT_EQ(station.waitTime, 0);
        EXPECT_EQ(station.vehicleIdQueue.size(), 0);
    }
}