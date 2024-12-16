#ifndef MINING_TRUCK_PROCESSOR_H
#define MINING_TRUCK_PROCESSOR_H

#include <MiningTruck.h>
#include <random>
#include <iostream>

using namespace std;

/**
* @class MiningTrucksProcessor 
* @brief Manages the states and processes of all mining stations in the simulation
*/
class MiningTrucksProcessor{
    // ****IF MORE TIME NOTE 1: Would function declarations here and move definitions to .cpp file
    // ****IF MORE TIME NOTE 2: Would add more error checking
    public:
        /**
        * @brief  Constructs new 'MiningTrucksProcessor' object
        */
        MiningTrucksProcessor(const size_t numMiningTrucks, const float min_mining_duration_hrs, const float max_mining_duration_hrs, 
                                const float travel_duration_hrs, const float unload_duration_min): m_NumMiningTrucks(numMiningTrucks), m_TravelDuration(travel_duration_hrs * 60), 
                                m_UnloadDuration(unload_duration_min), m_MiningTrucksList(initMiningTrucks(numMiningTrucks, min_mining_duration_hrs, max_mining_duration_hrs)), 
                                m_LoadedTrucksIdx(){};

        /**
        * @brief  Construct all mining trucks in simulation
        * @param numMiningTrucks Number of mining trucks in the simulation
        * @param minMiningDuration_hrs Minimum length of a truck's mining cycle in hours
        * @param maxMiningDuration_hrs Maximum length of a truck's mining cycle in hours
        */
        static vector<Truck> initMiningTrucks(const size_t numMiningTrucks, float minMiningDuration_hrs, const float maxMiningDuration_hrs){
            vector<Truck> miningTrucks{};
            // Create Truck objects with randomly generated mining durations between min and max
            for(int i = 0; i < numMiningTrucks; i++){
                miningTrucks.push_back(Truck(i, generateMiningTime(minMiningDuration_hrs, maxMiningDuration_hrs)));
            }
            return miningTrucks;
        };

        /**
        * @brief  Updates the states of the mining trucks
        * @param timestep_minutes Length of one timestep in minutes
        */
        void updateMiningTrucks(const float timestep_minutes){
            // Vector to hold newly loaded truck indices
            vector<int> loadedTrucksIds{};

            // Iterate through all trucks to update state
            for(Truck& truck : m_MiningTrucksList){
                bool stateChange{false};
                // Change state of variable according to current state
                switch(truck.state){
                    case TruckStates::MINING:
                        stateChange = runTruckCycle(truck, timestep_minutes);
                        truck.numMiningCycles++;
                        if(stateChange){
                            // After Mining, 
                            // change state to travel
                            truck.state = TruckStates::TRAVEL;
            
                            // Mark Truck as loaded
                            truck.isLoaded = true;

                            // Increment time until next state
                            truck.timeUntilNextState += m_TravelDuration;  // IDEALLY WOULD MAKE TIMES FOR EACH STATE CONFIGURABLE
                        }
                        break;
                    case TruckStates::TRAVEL:
                        stateChange = runTruckCycle(truck, timestep_minutes);
                        truck.numTravelCycles++; // increment travel cycle
                        if(stateChange){
                            // After Travelling and if unloaded, start mining
                            if(truck.isLoaded){
                                // After Travelling and if loaded, change to unload
                                // Change State to Unload
                                truck.state = TruckStates::UNLOAD;

                                // Add truck id to list of trucks to be assigned an unloading station
                                loadedTrucksIds.push_back(truck.id);

                                // Increment time until next state
                                truck.timeUntilNextState += m_UnloadDuration;
                            }
                            else{
                                // Change state to mining
                                truck.state = TruckStates::MINING;

                                // Increment time until next state
                                truck.timeUntilNextState += truck.miningCycleDuration;
                            }
                        }    
                        break;
                    case TruckStates::UNLOAD:
                        // if unloaded, begin travel back to mining station
                        if(!truck.isLoaded){
                            stateChange = runTruckCycle(truck, timestep_minutes);
                            truck.numUnloads++;

                            if (stateChange){
                                // change state to travel
                            truck.state = TruckStates::TRAVEL;

                            // Increment time until next state
                            truck.timeUntilNextState += m_TravelDuration;
                            }    
                        }
                        // If loaded, stay in unload state
                        break;
                }
            }

            // Save vector of trucks awaiting unloading station assignment
            m_LoadedTrucksIdx = loadedTrucksIds;
        };

        /**
        * @brief  Prints all of the trucks in the simulation along with their mining durations
        */
        void printTrucks() {
            for (const auto& truck : m_MiningTrucksList) {
                cout << "Truck ID: " << truck.id << ", Mining Time: " << truck.miningCycleDuration << " min \n" << endl;
            }
        };

        /**
        * @brief  Returns the number of mining trucks in simulation
        */
        const size_t getNumMiningTrucks(){
            return m_NumMiningTrucks;
        };

         /**
        * @brief  Returns the travel duration of the mining trucks in minutes
        */
        const float getTravelDuration(){
            return m_TravelDuration;
        };

        /**
        * @brief  Returns vector with all newly loaded trucks
        */
        const vector<int> getLoadedTrucks(){
            return m_LoadedTrucksIdx;
        };

        /**
         * @brief List of all mining trucks
         */
        vector<Truck> m_MiningTrucksList;

    private:
        /**
        * @brief  Number of mining trucks in simulation
        */
        const size_t m_NumMiningTrucks;

        /**
        * @brief  Duration of travel process (minutes)
        */
        const float m_TravelDuration;

        /**
        * @brief  Duration of unload process (minutes)
        */
        const float m_UnloadDuration;

        /**
        * @brief  Indices of trucks awaiting assignment to unloading station
        */
        vector<int> m_LoadedTrucksIdx;

        /**
        * @brief  Generates random mining time between min and max hours --> IF MORE TIME, GENERATE RANDOM FLOAT
        */
        static const float generateMiningTime(const float& minMiningDuration_hrs, const float& maxMiningDuration_hrs){
            // Initialize random number generator to assign mining times
            random_device rd;
            mt19937 gen(rd());
            uniform_real_distribution<float> distrib(minMiningDuration_hrs, maxMiningDuration_hrs); // Generates random int between min and max
            return distrib(gen) * 60; // convert hours to minutes
        };

        /**
        * @brief  Runs one time step for truck. Returns true if truck requires state change, otherwise false
        */
        bool runTruckCycle(Truck& truck, const float& timestep_minutes){
            bool stateChange{false};

            // Decrement time until next step by timestep, if not already 0
            truck.timeUntilNextState -= timestep_minutes;

            // Check if state change necessary
            // If timeUntilNextState <= 0 --> change state
            if(truck.timeUntilNextState <= 0){
                stateChange = true;
            }

            // Returns state change status
            return stateChange;
        };
};

#endif // MINING_TRUCK_PROCESSOR_H