#ifndef SIMULATION_H
#define SIMULATION_H

#include <MiningTruckProcessor.h>
#include <UnloadingStationProcessor.h>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <filesystem>
#include "spdlog/spdlog.h"

using namespace std;
using namespace spdlog;

/**
 * @class Simulation
 * @brief Manages the simulation of mining truck and unloading stations
 */
class Simulation{
    // ****IF MORE TIME NOTE: Would leave function declarations here and move definitions to .cpp file
    public:
        /**
        * @brief  Constructs new 'Simulation' object
        */
        Simulation(const size_t numMiningTrucks, const size_t numUnloadingStations, const float min_mining_duration_hrs,
                    const float max_mining_duration_hrs, const float travel_duration_hrs, const float unload_duration_hrs, 
                    const double simulation_time_hrs, const double simulation_timestep_min) : m_SimulationTime(simulation_time_hrs * 60), 
                    m_SimulationTimestep(simulation_timestep_min), m_CurrentSimulationTime(0),
                    m_MiningTrucksProcessor(numMiningTrucks, min_mining_duration_hrs, max_mining_duration_hrs, travel_duration_hrs, unload_duration_hrs),
                    m_UnloadingStationProcessor(numUnloadingStations, unload_duration_hrs), m_MiningTrucksPerformance(), m_UnloadingStationsPerformance() {}

        /**
        * @brief  Runs the full simulation to completion
        */
        void run(){
            while(m_CurrentSimulationTime <= m_SimulationTime){
                // Simulate step : update stations --> Update vehicles --> Assign vehicles to stations if applicable
                m_UnloadingStationProcessor.updateUnloadingStations(m_SimulationTimestep, m_MiningTrucksProcessor.m_MiningTrucksList); // update unloading stations
                m_MiningTrucksProcessor.updateMiningTrucks(m_SimulationTimestep); // update vehicles
                m_UnloadingStationProcessor.assignVehiclesToStations(m_MiningTrucksProcessor.m_MiningTrucksList, m_MiningTrucksProcessor.getLoadedTrucks());

                // Increment simulation time
                m_CurrentSimulationTime += m_SimulationTimestep;
            }
        };

         /**
        * @brief  Computes simulation performance statistics for mining trucks and unload stations
        */
        void computePerformanceStats(){
            const float unloadDuration{m_UnloadingStationProcessor.getUnloadDuration()};

            // Compute performance for all mining trucks
            for (const Truck& truck : m_MiningTrucksProcessor.m_MiningTrucksList) {
                TruckPerformanceStats truck_stats{computeTruckPerformance(truck, unloadDuration)};
                m_MiningTrucksPerformance.push_back(truck_stats);
            }

            // Compute performance for all unloading stations
            for (const Station& station : m_UnloadingStationProcessor.m_UnloadingStationsList) {
                StationPerformanceStats station_stats{computeStationPerformance(station, unloadDuration)};
                m_UnloadingStationsPerformance.push_back(station_stats);
            }
        }

        /**
        * @brief  Returns vector Mining Truck objects
        */
        const vector<Truck> getMiningTrucks(){
            return m_MiningTrucksProcessor.m_MiningTrucksList;
        };

        /**
        * @brief  Returns vector Mining Truck Performance objects
        */
        const vector<TruckPerformanceStats> getMiningTruckPerformances(){
            if(m_MiningTrucksPerformance.size() == 0){
                computePerformanceStats();
            }
            return m_MiningTrucksPerformance;
        };

        /**
        * @brief  Returns vector Unloading Station objects
        */
        const vector<Station> getUnloadingStations(){
            return m_UnloadingStationProcessor.m_UnloadingStationsList;
        };

        /**
        * @brief  Returns vector Unloading Station Performance objects
        */
        const vector<StationPerformanceStats> getUnloadingStationPerformances(){
            if(m_UnloadingStationsPerformance.size() == 0){
                computePerformanceStats();
            }
            return m_UnloadingStationsPerformance;
        };

        /**
        * @brief  Print performance statistics of mining trucks
        */
        void printMiningTruckPerformanceStatistics(){
            if(m_MiningTrucksPerformance.size() == 0){
                computePerformanceStats();
            }

            cout << "Mining Truck Performance: " << endl;
            for (const TruckPerformanceStats& stats : m_MiningTrucksPerformance) {
                printTruckPerformance(stats);
            }
        };

        /**
        * @brief  Print performance statistics of unloading stations
        */
        void printUnloadingStationPerformanceStatistics(){
            if(m_UnloadingStationsPerformance.size() == 0){
                computePerformanceStats();
            }

            cout << "Unloading Station Performance: " << endl;
            for (const StationPerformanceStats& stats : m_UnloadingStationsPerformance) {
                printStationPerformance(stats);
            }
        };

        /**
         * @brief Writes truck performance stats to a CSV file.
         * @param fileName Base name of the file to save.
         */
        void writeTruckPerformanceToCSV(const string& fileName, const string& resultsDir) {
            // Compute performance if not done already
            if(m_MiningTrucksPerformance.size() == 0){
                computePerformanceStats();
            }

            // Check that results directory exists
            if (!filesystem::exists(resultsDir)) {
                if (!filesystem::create_directory(resultsDir)) {
                    error("Error: Could not create directory {}", resultsDir);
                    return;
                }
            }

            // Append date and time to fileName
            string dateTime = getCurrentDateTime();
            string fullFileName = resultsDir + fileName + "_" + dateTime + ".csv";

            // Open file in write mode
            ofstream outFile(fullFileName);
            if (!outFile) {
                error("Error: Could not open file {} for writing.", fullFileName);
                return;
            }

            // Write CSV header
            outFile << "Vehicle ID,Percent Mining Time,Percent Travel Time,Percent Unloading Time,Percent Idle Time,Total Mining Time (hrs),Total Unloads\n";

            // Write data to csv file
            for (const TruckPerformanceStats& stats : m_MiningTrucksPerformance) {
                outFile << stats.vehicleId << ","
                        << fixed << setprecision(2)
                        << stats.percentMiningTime << ","
                        << stats.percentTravelTime << ","
                        << stats.percentUnloadingTime << ","
                        << stats.percentIdleTime << ","
                        << stats.totalMiningTime_hrs << ","
                        << stats.totalUnloads << "\n";
            }
            info("Mining Truck Performance stats written to file: {}", fullFileName);
        }

        /**
         * @brief Writes station performance stats to a CSV file.
         * @param fileName Base name of the file to save.
         * @param resultsDir Name of the results directory to save file in.
         */
        void writeStationPerformanceToCSV(const string& fileName, const string& resultsDir) {
            // Compute performance if not done already
            if(m_UnloadingStationsPerformance.size() == 0){
                computePerformanceStats();
            }

            // Check that results directory exists
            if (!filesystem::exists(resultsDir)) {
                if (!filesystem::create_directory(resultsDir)) {
                    error("Error: Could not create directory {}", resultsDir);
                    return;
                }
            }

            // Append date and time to fileName
            string dateTime = getCurrentDateTime();
            string fullFileName = resultsDir + fileName + "_" + dateTime + ".csv";

            // Open file in write mode
            ofstream outFile(fullFileName);
            if (!outFile) {
                error("Error: Could not open file {} for writing.", fullFileName);
                return;
            }

            // Write CSV header
            outFile << "Station ID,Percent Unloading Time,Percent Idle Time,Total Idle Time (hrs),Total Unloading Time (hrs),Total Unloads\n";

            // Write data to csv file
            for (const StationPerformanceStats& stats : m_UnloadingStationsPerformance) {
                outFile << stats.stationId << ","
                        << fixed << setprecision(2)
                        << stats.percentUnloadingTime << ","
                        << stats.percentIdleTime << ","
                        << stats.totalIdleTime_hrs << ","
                        << stats.totalUnloadingTime_hrs << ","
                        << stats.totalUnloads << "\n";
            }
            info("Unloading Station Performance stats written to file: {}", fullFileName);
        }
        
    private:
        /**
        * @brief  The full duration of the simulation (minutes)
        */
        const double m_SimulationTime;

        /**
        * @brief  Length of a single timestep of the simulation (minutes)
        */
        const double m_SimulationTimestep;

        /**
        * @brief  Tracks the current elapsed time of the simulation (minutes)
        */
        double m_CurrentSimulationTime;

        /**
        * @brief  Processor that manages mining trucks
        */
        MiningTrucksProcessor m_MiningTrucksProcessor;

        /**
        * @brief  Processor that manages unloading stations
        */
        UnloadingStationProcessor m_UnloadingStationProcessor;

        /**
        * @brief  Vector to hold performance stats of all mining trucks
        */
        vector<TruckPerformanceStats> m_MiningTrucksPerformance;

        /**
        * @brief  Vector to hold performance stats of all unload stations
        */
        vector<StationPerformanceStats> m_UnloadingStationsPerformance;

        /**
        * @brief  Computes and returns the performance statistics of a truck
        */
        TruckPerformanceStats computeTruckPerformance(const Truck& truck, const float& unloadDuration_min){
            TruckPerformanceStats stats(truck.id);
            stats.percentMiningTime = (truck.numMiningCycles * m_SimulationTimestep/ m_SimulationTime) * 100;
            stats.percentTravelTime = (truck.numTravelCycles * m_SimulationTimestep/ m_SimulationTime) * 100;
            stats.percentUnloadingTime = (truck.numUnloads * unloadDuration_min/ m_SimulationTime) * 100;
            stats.percentIdleTime = 100.0 - stats.percentMiningTime - stats.percentTravelTime - stats.percentUnloadingTime;
            stats.totalMiningTime_hrs = (truck.numMiningCycles *  m_SimulationTimestep)/60;
            stats.totalUnloads = truck.numUnloads;
            return stats;
        };

        /**
        * @brief  Computes and returns the performance statistics of an unloading station
        */
        StationPerformanceStats computeStationPerformance(const Station& station, const float& unloadDuration_min){
            StationPerformanceStats stats(station.id);
            stats.percentUnloadingTime = (station.numVehiclesUnloaded * unloadDuration_min/ m_SimulationTime) * 100;
            stats.percentIdleTime = 100.0 - stats.percentUnloadingTime;
            stats.totalUnloadingTime_hrs = (station.numVehiclesUnloaded * unloadDuration_min)/ 60;
            stats.totalIdleTime_hrs = (m_SimulationTime/60) - stats.totalUnloadingTime_hrs;
            stats.totalUnloads = station.numVehiclesUnloaded;
            return stats;
        };

        /**
        * @brief Prints the performance statistics of a mining truck
        */
        void printTruckPerformance(const TruckPerformanceStats& stats) {
                std::cout << " - Truck ID: " << stats.vehicleId
                << ", Mining Time: " << fixed << setprecision(2) << stats.percentMiningTime << "%"
                << ", Travel Time: " << stats.percentTravelTime << "%"
                << ", Unloading Time: " << stats.percentUnloadingTime << "%"
                << ", Idle Time: " << stats.percentIdleTime << "%"
                << ", Total Mining Time (hrs): " << stats.totalMiningTime_hrs
                << ", Total Unloads: " << stats.totalUnloads
                << std::endl;
                }
        
        /**
         * @brief Prints the performance statistics of an unloading station
         */
        void printStationPerformance(const StationPerformanceStats& stats) {
            std::cout << " - Station ID: " << stats.stationId
                    << ", Total Unloads: " << stats.totalUnloads
                    << ", Percent Unloading Time: " << fixed << setprecision(2) << stats.percentUnloadingTime << "%"
                    << ", Percent Idle Time: " << stats.percentIdleTime << "%"
                    << ", Total Unloading Time (hrs): " << stats.totalUnloadingTime_hrs
                    << ", Total Idle Time (hrs): " << stats.totalIdleTime_hrs
                    << std::endl;
                    }

        /**
         * @brief Get current date and time as a string in "YYYY-MM-DD_HH-MM-SS" format.
         */
        std::string getCurrentDateTime() {
            time_t now = time(0);
            tm* localTime = localtime(&now);

            char buffer[20];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", localTime);
            return std::string(buffer);
        }

};

#endif // SIMULATION_H