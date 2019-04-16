#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Job{
    public:
    int tasks[20]; // 20 - max quantity of machines

    int entireLength = 0;
    int initialIndex = 0;
    Job() {} 


    int calculateJobLength(int size) {
        this->entireLength = 0;

        for(int i = 0; i < size; i++){
            this->entireLength += tasks[i];
        }
    }

    void reset(){
        entireLength = 0;
    }
};

void printArray(Job jobs[], int sizeJob, int sizeMachines){
    for(int i = 0; i < sizeJob; i++){
        std::cout << "Init index: " << jobs[i].initialIndex+1 << " entireLength: " << jobs[i].entireLength << "     [";
        for(int j = 0; j < sizeMachines; j++){
            std::cout << jobs[i].tasks[j] << ", ";  
        }
        std::cout << "]\n";
    }
}

void printInitialOrder(Job jobs[], int size){
    for(int i = 0; i < size; i++){
        std::cout << jobs[i].initialIndex+1 << " ";
    }
}

int temp[501][501];

int calculateC_MAX(Job jobs[], const int quantityOfJobs, const int quantityOfMachines){ // vecSizeLimit corespond with the quantity of jobs in subset
    
    
    int valueCarryForNextRow = 0;

    for(int i = 0; i < quantityOfJobs; i++){
        temp[i][0] = 0;
    }

    for(int i = 0; i < quantityOfMachines; i++){
        temp[0][i] = 0;
    }
    

    for (int row = 1; row <= quantityOfJobs; row++){
        for(int column = 1; column <= quantityOfMachines; column++){
            valueCarryForNextRow = jobs[row-1].tasks[column-1];
            temp[row][column] = std::max( temp[row-1][column], temp[row][column-1] ) + valueCarryForNextRow;
        }
    }
    return temp[quantityOfJobs][quantityOfMachines];
}



int main(){
    std::cout << "Witam\n";

    std::ifstream myfile("neh.data.txt");

    int quantityOfJobs = 0;
    int quantityOfMachines = 0;
    int temporaryValue;


    Job jobs[500]; // 500 - is max quantity of jobs in file

    std::string line;
    std::string dataIndexToString = "";

    Job currentJob;


    if( ! myfile.is_open()) throw std::runtime_error("Could not open file");

    for (int dataIndex = 0; dataIndex <= 120; dataIndex++){

        if( dataIndex < 10 ){
            dataIndexToString = "data.00" + std::to_string(dataIndex) + ":\r";
        } else if (dataIndex >= 10 && dataIndex < 100) {
            dataIndexToString = "data.0" + std::to_string(dataIndex) + ":\r";
        } else {
            dataIndexToString = "data." + std::to_string(dataIndex) + ":\r";
        }
        
        while ( getline(myfile, line) && (line != dataIndexToString ) ); // skip useless data

        myfile >> quantityOfJobs;
        myfile >> quantityOfMachines;

        
        // read data set
        for(int row = 0; row < quantityOfJobs; row++){
            for(int col = 0; col < quantityOfMachines; col++){
                myfile >> temporaryValue;
                currentJob.tasks[col] = temporaryValue;
            }
            currentJob.initialIndex = row;
            currentJob.calculateJobLength(quantityOfMachines);

            jobs[row] = currentJob;
        }

        //std::cout << "Init result: " << calculateC_MAX(jobs, 5, quantityOfMachines ) << "\n";
        
        //printArray(jobs, quantityOfJobs , quantityOfMachines);

        //std::cout << "\n\n";
        std::sort(jobs, jobs + quantityOfJobs, [] ( const Job& first, const Job& second) { return first.entireLength > second.entireLength;} );

       // printArray(jobs, quantityOfJobs , quantityOfMachines);

        // neh algorithm:
        

        int currentC_MAX = 0;
        int smallestC_MAX = 0;

        int quantityOfSwaps = 0;
        int quantityOfSwapsWithTheBestResult = 0;
        
        for(int currentJobIndex = 0, currentSetSize = 2; currentJobIndex < quantityOfJobs - 1; currentJobIndex++, currentSetSize++){

            currentC_MAX = calculateC_MAX(jobs, currentSetSize, quantityOfMachines );
            smallestC_MAX = currentC_MAX;

            // swap current job from LAST index jobVector.size() - 1 times, inside jobVector in order to obtain the smallestC_MAX for current set
            quantityOfSwaps = 0;
            quantityOfSwapsWithTheBestResult = 0;
            for(int i = currentSetSize - 1;  i>= 1; i--){ // start from last index of current set

                quantityOfSwaps++;
                std::swap(jobs[i], jobs[i-1]);
                currentC_MAX = calculateC_MAX(jobs, currentSetSize, quantityOfMachines );

                if(currentC_MAX < smallestC_MAX){
                    smallestC_MAX = currentC_MAX;
                    quantityOfSwapsWithTheBestResult = quantityOfSwaps;
                }
            } // after this loop, job which we were swaping is placed at the 0 index in vector

            // we are swaping job back to the best place
            for( int x = quantityOfSwaps, index = 0; x > quantityOfSwapsWithTheBestResult; x-- ){
                std::swap(jobs[index], jobs[index+1]);
                index++;
            }
        }


        std::cout << dataIndexToString << "\n";
        printInitialOrder(jobs, quantityOfJobs); 
        std::cout << "      result = " << calculateC_MAX(jobs, quantityOfJobs, quantityOfMachines ); std::cout << "\n";

    }
    

    return 0;
}
