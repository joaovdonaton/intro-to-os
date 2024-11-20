#include <iostream>
#include <fstream>

// specify sizes for the data.txt file
#define NUM_PROCESSES 5
#define NUM_RESOURCES 3

void debugMatrix(int matrix[NUM_PROCESSES][NUM_RESOURCES]){
    std::cout << "printing matrix for debugging:" << std::endl;
    for (int i = 0; i < NUM_PROCESSES; i++){
        for (int j = 0; j < NUM_RESOURCES; j++){
            std::cout << matrix[i][j];
        }
        std::cout << std::endl;
    }

    std::cout << "matrix end" << std::endl;
}

int main(){
    int allocation[NUM_PROCESSES][NUM_RESOURCES];
    int max[NUM_PROCESSES][NUM_RESOURCES];
    int available[NUM_RESOURCES];
    int need[NUM_PROCESSES][NUM_RESOURCES];

    std::ifstream dataFile("data.txt");
    int k = 0;
    
    // read allocation matrix
    for(int i = 0; i < NUM_PROCESSES; i++){
        for(int j = 0; j < NUM_RESOURCES; j++){
            dataFile >> allocation[i][j];
        }
    }

    // read max matrix
    for(int i = 0; i < NUM_PROCESSES; i++){
        for(int j = 0; j < NUM_RESOURCES; j++){
            dataFile >> max[i][j];
        }
    }

    // read available resources
    dataFile >> available[0];
    dataFile >> available[1];
    dataFile >> available[2];

    // we also want to compute need matrix (max - allocation)
    for(int i = 0; i < NUM_PROCESSES; i++){
        for(int j = 0; j < NUM_RESOURCES; j++){
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    //std::cout << "available has: " << available[0] << available[1] << available[2] << std::endl;

    // safety algorithm
    int work[NUM_RESOURCES] = {available[0], available[1], available[2]};
    bool finish[NUM_PROCESSES] = {false, false, false, false, false};

    while(1){
        bool foundOne = false;
        for(int i = 0; i < NUM_PROCESSES; i++){
            if(!finish[i]){
                bool all_leq = true;
                for(int j = 0; j < NUM_RESOURCES; j++){
                    if(!(need[i][j] <= work[j])) all_leq = false; 
                }

                if(all_leq){ // we find first finish = false and need_i <= work
                    foundOne = true;
                    for (int j = 0; j < NUM_RESOURCES; j++){
                        work[j] = work[j] + allocation[i][j];
                    }
                    finish[i] = true;

                    std::cout << "Process " << i << " finishes" << std::endl;
                }
            }
        }

        if(!foundOne) break;
    }

    bool allTrue = true;
    for(bool f: finish){
        if(!f) allTrue = false;
    }

    if(allTrue) std::cout << "System is in a safe state, all " << NUM_PROCESSES << " processes will finish without deadlock" << std::endl;
    else std::cout << "System is in an unsafe state.    " << std::endl;
    return 0;
}