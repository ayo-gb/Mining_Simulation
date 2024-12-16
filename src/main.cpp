#include <iostream>
#include <Simulation.h>
#include "spdlog/spdlog.h"

using namespace std;
using namespace spdlog;

// Total number of hours for simulation
static const double SIMULATION_TIME{72}; 

// Simulation timestep in minutes
static const double SIMULATION_TIMESTEP{5};

// Duration of unload process in minutes
static const double UNLOAD_DURATION{5};

// Duration of travel process in hours
static const double TRAVEL_DURATION{0.5}; 

// Minimum mining duration in hours
static const double MIN_MINING_DURATION{1}; 

// Maximum mining duration in hours
static const double MAX_MINING_DURATION{5}; 

int main(int argc, char* argv[])
{
    // Check if the user provided exactly two additional arguments
    if (argc != 3) {
        error("Usage: {} <number_of_mining_trucks> <number_of_unloading_stations>\n", argv[0]);
        return 1;
    }

    // Convert command-line arguments to int
    const int numMiningTrucks{stoi(argv[1])};
    const int numUnloadingStations{stoi(argv[2])};

    // Validate inputs
    if(numMiningTrucks <= 0){
        error("Invalid number of mining trucks (must be > 0): {}", numMiningTrucks);
        // std::cerr << "Invalid number of mining trucks (must be > 0): " << numMiningTrucks << endl;
        return 1;
    }

    if(numUnloadingStations <= 0){
        error("Invalid number of unloading stations (must be > 0): {}", numUnloadingStations);
        // std::cerr << "Invalid number of unloading stations (must be > 0): " << numUnloadingStations<< endl;
        return 1;
    }

    // Initialize Simulation
    info("Initializing Simulation...");
    info("Simulation Duration: {}", SIMULATION_TIME);
    info("Simulation Timestep: {}", SIMULATION_TIMESTEP);
    info("Number of mining trucks: {}", numMiningTrucks);
    info("Number of unloading stations: {}", numUnloadingStations);
    Simulation miningSimulation(numMiningTrucks, numUnloadingStations, MIN_MINING_DURATION, MAX_MINING_DURATION, TRAVEL_DURATION, 
                                UNLOAD_DURATION, SIMULATION_TIME, SIMULATION_TIMESTEP);

    // Run simulation
    info("Running Simulation...");
    miningSimulation.run();
    info("Simulation Complete...");

    // Compute Simulation Performance Statistics
    info("Computing Simulation Performance Results...");
    miningSimulation.computePerformanceStats();

    // Print Performance Results
    miningSimulation.printMiningTruckPerformanceStatistics();
    miningSimulation.printUnloadingStationPerformanceStatistics();

    // Print simulation results to csv files
    info("Saving Simulation Results to CSV files...");
    const std::string resultsDir = "results/"; // Define the results directory
    const string miningTruckFileName = "MiningSimulationResults_MiningTrucks";
    const string unloadingStationFileName = "MiningSimulationResults_UnloadingStations";
    miningSimulation.writeTruckPerformanceToCSV(miningTruckFileName, resultsDir);
    miningSimulation.writeStationPerformanceToCSV(unloadingStationFileName, resultsDir);

    info("Done!");
    return 0;
}