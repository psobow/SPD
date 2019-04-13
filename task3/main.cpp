#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

class Job{
    public:
    std::vector<int> tasks;
    int entireLength = 0;
    Job() {} 


    int calculateJobLength() {
        int functionLength = 0;
        for(std::size_t i = 0; i < tasks.size(); i++){
            functionLength+=tasks[i];
        }
        this->entireLength += functionLength;
    }

    void reset(){
        tasks.clear();
        entireLength = 0;
    }

    void printlnJob() const {
        std::cout << "Job SUM = " << entireLength << " [";
        for(std::size_t i = 0; i < tasks.size(); i++){
            std::cout << tasks[i] << ", ";
        }
        std::cout << "]\n";
    } 
};


void printVector(std::vector<Job> &vec){
    int i = 0;
    for(auto&& job : vec){
        i++;
        std::cout << "index = " << i << " ";
        job.printlnJob();
    }
}

int calculateC_MAX(std::vector<Job> &vec, const int vecSizeLimit){

    return 0;
}

int main(){
    std::cout << "Witam\n";

    std::ifstream myfile("neh.data.txt");

    int quantityOfJobs = 0;
    int quantityOfMachines = 0;
    int temporaryValue = 0; // var used to read value from file into it

    std::vector<Job> jobVector;

    std::string line;

    Job currentJob;


    if( ! myfile.is_open()) throw std::runtime_error("Could not open file");

    while ( getline(myfile, line) && (line != "data.000:\r") ); // skip useless data

    myfile >> quantityOfJobs;
    myfile >> quantityOfMachines;

    std::cout << "jobs : " << quantityOfJobs << " machines: " << quantityOfMachines << "\n";

    
    // read data set
    for(int j = 0; j < quantityOfJobs; j++){
        for(int i = 0; i < quantityOfMachines; i++){
            myfile >> temporaryValue;
            currentJob.tasks.push_back(temporaryValue);
        }

        currentJob.calculateJobLength();
        jobVector.push_back(currentJob);
        currentJob.reset();
    }
    
    printVector(jobVector);

    std::cout << "list size() = " << jobVector.size() << "\n";

    std::sort(jobVector.begin(), jobVector.end(), [] ( const Job& first, const Job& second) { return first.entireLength > second.entireLength;} );

    std::cout << "\njobs insied vector after sorting:\n";
    printVector(jobVector);



    int currentC_MAX;
    int smallestC_MAX;
    int quantityOfSwapsWithTheBestResult;

    
    for(int currentJob = 0, currentSetSize = 2; currentJob < jobVector.size() - 1; currentJob++, currentSetSize++){

        currentC_MAX = calculateC_MAX(jobVector, currentSetSize);
        smallestC_MAX = currentC_MAX;

        // swap current job from last index jobVector.size() - 1 times, inside jobVector in order to obtain the smallestC_MAX
        int quantityOfSwaps = 0;
        for(int i = currentSetSize - 1;  i>= 1; i--){

            quantityOfSwaps++;
            std::swap(jobVector[i], jobVector[i-1]);
            currentC_MAX = calculateC_MAX(jobVector, currentSetSize);

            if(currentC_MAX < smallestC_MAX){
                smallestC_MAX = currentC_MAX;
                quantityOfSwapsWithTheBestResult = quantityOfSwaps;
                
            }
        } // after this loop, job which we were swaping is placed at the last index in vector

        // place job in the best possible place
        for( int x = quantityOfSwaps, index = 0; x > quantityOfSwapsWithTheBestResult; x-- ){
            std::swap(jobVector[index], jobVector[index+1]);
            index++;
        }
    }



    return 0;
}

/* To jest funkcja która liczy uszeregowanie dla 3 maszyn i nie ograniczonej ilości zadań:

int calculateC_MAX(std::vector<int> localR, std::vector<int> localP, 
    std::vector<int> localQ ){
    

    int m = 0 , c = 0;
    for(int i = 0; i < localR.size(); i++){
        m = std::max(localR[i], m) + localP[i];
        c = std::max(c , m + localQ[i]);
    }

    return c;
}

TODO: jak to przerobić żeby było dla nieograniczonej ilości maszyn?????
*/