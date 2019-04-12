#include <iostream>
#include <fstream>
#include <vector>
#include <list>
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


void printList(std::list<Job> &list){
    int i = 0;
    for(auto&& job : list){
        i++;
        std::cout << "index = " << i << " ";
        job.printlnJob();
    }
}

int main(){
    std::cout << "Witam\n";

    std::ifstream myfile("neh.data.txt");

    int quantityOfJobs = 0;
    int quantityOfMachines = 0;
    int temporaryValue = 0; // var used to read value from file into it

    std::list<Job> myJobList;

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
        myJobList.push_back(currentJob);
        currentJob.reset();
    }
    
    printList(myJobList);

    std::cout << "list size() = " << myJobList.size() << "\n";

    myJobList.sort( [] ( const Job& first, const Job& second) { return first.entireLength > second.entireLength;} );

    std::cout << "\njobs insied list after sorting:\n";
    printList(myJobList);

    /* iterator and iserting test */ 
    // weź wypierdol ten iterator
    //std::list<Job>::iterator it = myJobList.begin();

    // TODO: na chuj ci ten iterator użyj poprostu swapa i zapisuj wyniki!


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