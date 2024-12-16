#ifndef UNLOADING_STATION_PROCESSOR_H
#define UNLOADING_STATION_PROCESSOR_H

#include <MiningTruckProcessor.h>
#include <UnloadingStation.h>
#include <iostream>
#include "spdlog/spdlog.h"

using namespace std;
using namespace spdlog;

/**
* @class UnloadingStationProcessor
* @brief Manages the states and processes of all unloading stations in the simulation
*/
class UnloadingStationProcessor{
    // ****IF MORE TIME NOTE: Would leave function declarations here and move definitions to .cpp file
    // ****IF MORE TIME NOTE 2: Would add more error checking
    public:
        /**
        * @brief  Constructs new 'UnloadingStationProcessor' object
        */
        UnloadingStationProcessor(const float numUnloadingStations, const float unloadDuration_min): m_NumUnloadingStations(numUnloadingStations), 
                                    m_UnloadDuration(unloadDuration_min), m_UnloadingStationsList(initUnloadingStations(numUnloadingStations)), m_AvailableLoadingStationIdxs() {};

        /**
        * @brief  Construct all unloading stations in simulation
        * @param numUnloadingStations Number of unloading stations in simulation
        */
        static vector<Station> initUnloadingStations(const size_t numUnloadingStations){
            vector<Station> unloadingStations{};
            // Create Station objects with unique ids
            for(int i = 0; i < numUnloadingStations; i++){
                unloadingStations.push_back(Station(i));
            }
            return unloadingStations;
        };

        /**
        * @brief Updates the states of the loading stations
        * @param timestep_min Length of one timestep in minutes
        * @param miningTrucksList List of all Mining trucks in a simulation
        */
        void updateUnloadingStations(const float timestep_min, vector<Truck>& miningTrucksList){
            // Iterate through all trucks to update state
            for(Station& station : m_UnloadingStationsList){
                bool stateChange{false};

                // Change state of station according to current state
                switch(station.state){
                    case UnloadingStationStates::AVAILABLE:
                        // After station is available,
                        // if there are vehicles to be unloaded, change state to occupied
                        if(!station.vehicleIdQueue.empty()){
                            // Change state to occupied
                            station.state = UnloadingStationStates::OCCUPIED;

                            // Compute current wait time
                            station.waitTime = m_UnloadDuration * station.vehicleIdQueue.size();
                        }
                        break;
                    case UnloadingStationStates::OCCUPIED:
                        // If vehicle in queue, unload vehicle
                        if(!station.vehicleIdQueue.empty()){
                            // Get Vehicle id from front of queue
                            const int vehicleId = station.vehicleIdQueue.front();

                            // Unload vehicle at station
                            stateChange = unloadVehicleAtStation(miningTrucksList[vehicleId], station);

                            // Remove vehicle id from station queue
                            station.vehicleIdQueue.pop();
                        }
                        else{
                            stateChange = true;
                        }

                        // if no vehicles left in queue, change state to available
                        if(stateChange && station.vehicleIdQueue.empty()){
                            // change state to available
                            station.state = UnloadingStationStates::AVAILABLE;

                            // add station idx to queue of available stations
                            m_AvailableLoadingStationIdxs.push(station.id);
                        }
                        break;
                }
            }
        };

        /**
        * @brief  Updates the states of the loading stations
        * @param miningTrucksList List of all Mining trucks in a simulation
        * @param loadedTrucksIdx List of indexes/ids of newly loaded trucks in simulation
        */
        void assignVehiclesToStations(vector<Truck>& miningTrucksList, const vector<int>& loadedTrucksIdx){
            // Iterate through all loaded truck indices
            int minWaitStationIdx{};
            for(int idx : loadedTrucksIdx){
                // Get first available/ minimum wait station index
                minWaitStationIdx = getShortestWaitStationIdx();

                // Assign current truck to station
                assignVehicle(miningTrucksList[idx], m_UnloadingStationsList[minWaitStationIdx]);
            }
        };

        /**
        * @brief  Returns the number of unloading stations in simulation
        */
        const size_t getNumUnloadingStations(){
            return m_NumUnloadingStations;
        };

        /**
        * @brief  Returns the unload duration in minutes
        */
        const float getUnloadDuration(){
            return m_UnloadDuration;
        };

        /**
        * @brief  Sets the queue of available loading stations
        */
        void setAvailableLoadingStations(const queue<int>& availableLoadingStations){
            m_AvailableLoadingStationIdxs = availableLoadingStations;
        };

        /**
         * @brief List of all unloading stations
         */
        vector<Station> m_UnloadingStationsList;

    private:
        /**
        * @brief  Number of mining trucks in simulation
        */
        const size_t m_NumUnloadingStations;

        /**
        * @brief  Duration of unloading process (minutes)
        */
        const float m_UnloadDuration;

        /**
        * @brief  Queue of Indices of available loading stations
        */
        queue<int> m_AvailableLoadingStationIdxs;

        /**
        * @brief  Gets the index of the station with the shortest wait time
        */
       int getShortestWaitStationIdx(){
            int shortestWaitIdx{-1};

            // Return any available station idx first
            if(!m_AvailableLoadingStationIdxs.empty()){
                // Get first available unloading station index
                shortestWaitIdx = m_AvailableLoadingStationIdxs.front();

                // return index
                return shortestWaitIdx;
            }

            // loop through all unavailable stations to and track shortest wait time
            float shortestWaitTime{};
            for(int i = 0; i < m_NumUnloadingStations; i++){
                Station currentStation{m_UnloadingStationsList[i]};
                if(i == 0){
                    shortestWaitIdx = currentStation.id;
                    shortestWaitTime = currentStation.waitTime;
                    continue;
                }

                if(currentStation.waitTime < shortestWaitTime){
                    shortestWaitIdx = currentStation.id;
                    shortestWaitTime = currentStation.waitTime;
                }
            }

            // Validate that shortest wait idx has been set
            if(shortestWaitIdx == -1){
                error("getShortestWait Error: Shortest wait time unloading station could not be found.");
            }
            return shortestWaitIdx;
       };

       /**
        * @brief  Performs actions related to unloading a vehicle, and returns true if state change required
        */
       bool unloadVehicleAtStation(Truck& loadedTruck, Station& unloadingStation){
        bool stateChange{false};

        // Check that vehicle is loaded
        if(!loadedTruck.isLoaded){
            error("UnloadVehicle Error: Vehicle is not loaded.");
            return stateChange;
        }

        // Check that unload duration is valid
        if (m_UnloadDuration < 0){
            error("UnloadVehicle Error: Invalid unload duration");
            return stateChange;
        }

        // Increment station unload count
        unloadingStation.numVehiclesUnloaded++;

        // update station wait time
        unloadingStation.waitTime -= m_UnloadDuration;

        // Change vehicle unload status
        loadedTruck.isLoaded = false;

        // Change vehicle assigment status;
        loadedTruck.isAssignedStation = false;

        if(unloadingStation.waitTime <= 0){
            stateChange = true;
        }

        return stateChange;
       };

       /**
        * @brief  Assigns a vehicle to a specific station
        */
       void assignVehicle(Truck& loadedTruck, Station& unloadingStation){
            // Check that vehicle is loaded
            if(!loadedTruck.isLoaded){
                error("AssignVehicle Error: Vehicle is not loaded.");
                return;
            }

            // Check that vehicle has not already been assigned
            if(loadedTruck.isAssignedStation){
                error("AssignVehicle Error: Vehicle has already been assigned to unloading station.");
                return;
            }

            // Check that unload duration is valid
            if (m_UnloadDuration < 0){
                error("UnloadVehicle Error: Invalid unload duration");
                return;
            }

            // Add vehicle id to station queue
            unloadingStation.vehicleIdQueue.push(loadedTruck.id);

            // Update station wait time
            unloadingStation.waitTime += m_UnloadDuration;

            // If station id in availability list, remove
            if(!m_AvailableLoadingStationIdxs.empty() && m_AvailableLoadingStationIdxs.front() == unloadingStation.id){
                m_AvailableLoadingStationIdxs.pop();
            }

            // Change vehicle assignment status
            loadedTruck.isAssignedStation = true;
       };
};

#endif // UNLOADING_STATION_PROCESSOR_H