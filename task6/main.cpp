#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>

class Job
{
public:
    // R - dostarczenia
    // P - Trwania
    // Q - stygnięcia
    int R;
    int P;
    int Q;

    int P_whichWillBeModifyInInterruptShrage;

    int initialIndex;

    Job job() {};

    void showJob(){
        std::cout << "R: " << R << " P: " << P << " Q: " << Q << " init index: " << initialIndex+1 << "\n";
    }
};

int calculateC_MAX(std::vector<Job> & jobs){
    int m = 0 , c = 0;
    for(int i = 0; i < jobs.size(); i++){
        m = std::max(jobs[i].R, m) + jobs[i].P;
        c = std::max(c , m + jobs[i].Q);
    }
    return c;
}

// Zadanie o indexie size nie jest uwzględniane w liczonym CMAX
int calculateC_MAX(std::vector<Job> & jobs, int size){
    int m = 0 , c = 0;
    for(int i = 0; i < size; i++){
        m = std::max(jobs[i].R, m) + jobs[i].P;
        c = std::max(c , m + jobs[i].Q);
    }
    return c;
}

struct DTO
{
    std::vector<Job> order;
    int C_MAX;
};




/*
 Funkcja znajduje indexy dostępnych zadań w danym przedziale czasu

 do tej fuunkcji trzeba przekazać vector prac z usunietymi pracami które zostały wykonane
*/

std::vector<int> getIndiciesAvailableJobs(std::vector<Job> & jobs, int elapsedTime){
    std::vector<int> result;
    for( int i = 0; i < jobs.size(); i++){
        if (jobs[i].R <= elapsedTime) {
            result.push_back(i);
        }
    }

    return result; 
}

// arguemtn jobs musi byc posorotwany rosnąco po R
        
DTO shrageNormal(std::vector<Job> jobs){
    std::vector<Job> result;

    int elapsedTime = jobs[0].R;
    int newElapsedTime = 0;
    int elapsedTimeNext;
    int Cmax = 0;
    int biggestQIndex;
    int biggestQ;

    std::vector<int> indicesAvailableJobs;


    while (jobs.size() != 0){
        indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime);

        if (indicesAvailableJobs.size() != 0){
            biggestQIndex = indicesAvailableJobs[0];
            biggestQ = jobs[biggestQIndex].Q;

            for (int index : indicesAvailableJobs){
                if(biggestQ < jobs[index].Q){ 
                    biggestQ = jobs[index].Q;
                    biggestQIndex = index;

                } else if (biggestQ == jobs[index].Q){ // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

                    if(jobs[biggestQIndex].initialIndex > jobs[index].initialIndex){
                        biggestQ = jobs[index].Q;
                        biggestQIndex = index;
                    }
                }
            }
            // trzeba znaleźć index pierwszego zadania którego czas R jest wiekszy od elapsedTime, to będzie tNext

            elapsedTimeNext = std::numeric_limits<int>::max();
            for (int i = 0; i < jobs.size(); i++) {
                if (jobs[i].R > elapsedTime){
                    elapsedTimeNext = jobs[i].R;
                    break;
                }
            }

            /*
            newElapsedTime = std::min(elapsedTime + jobs[biggestQIndex].P, elapsedTimeNext);
            jobs[biggestQIndex].P = jobs[biggestQIndex].P - (newElapsedTime - elapsedTime);
            elapsedTime = newElapsedTime;
            */

            elapsedTime += jobs[biggestQIndex].P;

            /*
            // jeśli P zmniejszyło się do zera to obliczay Cmax i usuwamy tą prace z kolekcji
            if (jobs[biggestQIndex].P == 0){
                Cmax = std::max(elapsedTime + jobs[biggestQIndex].Q, Cmax);
                // usunać z kolekcji jobs obiekt pod indexem biggestQIndex
                jobs.erase(jobs.begin() + biggestQIndex);
            }
            */

            Cmax = std::max(elapsedTime + jobs[biggestQIndex].Q, Cmax);
            
            result.push_back(jobs[biggestQIndex]);

            // usunać z kolekcji jobs obiekt pod indexem biggestQIndex
            jobs.erase(jobs.begin() + biggestQIndex);

        } else {
            elapsedTime = elapsedTimeNext;
        }
    }    

    DTO dto;
    dto.C_MAX = Cmax;
    dto.order = result;

    //std::cout << "Cmax: " << Cmax << "\n";

    return dto;
}


// arguemtn jobs musi byc posorotwany rosnąco po R
        
int shrageWithInterrupt(std::vector<Job> jobs){ 

    int elapsedTime = jobs[0].R;
    int newElapsedTime = 0;
    int elapsedTimeNext;
    int Cmax = 0;
    int biggestQIndex;
    int biggestQ;

    std::vector<int> indicesAvailableJobs;


    while (jobs.size() != 0){
        indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime);

        if (indicesAvailableJobs.size() != 0){
            biggestQIndex = indicesAvailableJobs[0];
            biggestQ = jobs[biggestQIndex].Q;

            for (int index : indicesAvailableJobs){
                if(biggestQ < jobs[index].Q){ 
                    biggestQ = jobs[index].Q;
                    biggestQIndex = index;

                } else if (biggestQ == jobs[index].Q){ // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

                    if(jobs[biggestQIndex].initialIndex > jobs[index].initialIndex){
                        biggestQ = jobs[index].Q;
                        biggestQIndex = index;
                    }
                }
            }
            // trzeba znaleźć index pierwszego zadania którego czas R jest wiekszy od elapsedTime, to będzie tNext

            elapsedTimeNext = std::numeric_limits<int>::max();
            for (int i = 0; i < jobs.size(); i++) {
                if (jobs[i].R > elapsedTime){
                    elapsedTimeNext = jobs[i].R;
                    break;
                }
            }

            
            newElapsedTime = std::min(elapsedTime + jobs[biggestQIndex].P_whichWillBeModifyInInterruptShrage, elapsedTimeNext);
            jobs[biggestQIndex].P_whichWillBeModifyInInterruptShrage = jobs[biggestQIndex].P_whichWillBeModifyInInterruptShrage - (newElapsedTime - elapsedTime);
            elapsedTime = newElapsedTime;
            


            
            // jeśli P zmniejszyło się do zera to obliczay Cmax i usuwamy tą prace z kolekcji
            if (jobs[biggestQIndex].P_whichWillBeModifyInInterruptShrage == 0){
                Cmax = std::max(elapsedTime + jobs[biggestQIndex].Q, Cmax);
                
                //result.push_back(jobs[biggestQIndex]);

                // usunać z kolekcji jobs obiekt pod indexem biggestQIndex
                jobs.erase(jobs.begin() + biggestQIndex);
            }
            

        } else {
            elapsedTime = elapsedTimeNext;
        }
    }    


    //std::cout << "Cmax: " << Cmax << "\n";

    //return result; // TODO: to nie ma żadnego sensu tutaj

    return Cmax;

    //--------------------------------------------------------------
    // znaleźlismy index zadania z największym Q. teraz musimy sprawdzić czy w trakcie czasu P tego zadania 
    // pojawi się zadanie z wiekszym Q 

    // jeśli pojawi się takie zadanie to wtedy zmniejszamy P pierwszego zadania które się wykonywało, o tyle ile się wykonalo
    // oraz zwiększamy elapsedTime o tyle ile wykonalo się pierwsze zadanie
    // i zaczynamy wykonywać to drugie zadanie.
    // zadanie pierwsze powraca do póli zadań dostępnych.

    // chyba rekurencyjnie to będziez trzeba zrobić 


    
    //elapsedTime += jobs[biggestQIndex].P;

    //jobs.erase(jobs.begin() + biggestQIndex -1);  // munmap_chunk(): invalid pointer // to przez tą minus jedynke

    //jobs[biggestQIndex].R = std::numeric_limits<int>::max(); // podmieniamy wartość R zadania które własnie dodaliśmy do kolekcji result.
    // powodem tej podmiany jest to że keidy usuwaliśmy za pomocą erase() to wylatywał nam wyjątek munmap_chunk();

}

void findBACjobs(std::vector<Job> jobs)
{

}

void findBjobs(std::vector<Job> jobs)
{

}

int main(int argc,char **argv){
    
    std::vector<Job> jobs;
    
    Job temporaryJob;
    DTO dto;
    int currentBestCmax = std::numeric_limits<int>::max();
    std::vector<Job> currentBestOrder;

    std::ifstream myfile("carl.data.txt");
    int quantityOfData = 0;

    std::string line;

    std::cout << "Hello world!\n";

    if( ! myfile.is_open()) throw std::runtime_error("Could not open file");

    std::string currentDataSet;

    for(int dataSetIndex = 9; dataSetIndex <= 9; dataSetIndex++){

        currentDataSet = "data.00" + std::to_string(dataSetIndex) + ":\r";

        while (getline(myfile, line) && (line != currentDataSet)); // skip useless data
        myfile >> quantityOfData;

        // read data
                
        for(int i = 0; i < quantityOfData; i++){
            myfile >> temporaryJob.R >> temporaryJob.P >> temporaryJob.Q;
            temporaryJob.initialIndex = i;
            temporaryJob.P_whichWillBeModifyInInterruptShrage = temporaryJob.P;

            jobs.push_back(temporaryJob);
        }

        // Posortujmye vector prac po R rosnąco

        std::sort(jobs.begin(), jobs.end(), [] (const Job & first, const Job & second ){ return first.R < second.R; });

    
        // Pierwsze dostępne zadanie będzie zadaniem o najmniejszym R
        // Zadanie to będzie dostępne w momencie elapsedTime równym jego R 


        

        // Carier

        dto = shrageNormal(jobs);
        std::cout << "Cmax, vectora zwróconego z funkcji shrageNormal, obliczony w CalculateC_MAX: " << calculateC_MAX(dto.order) << "\n";
        if (dto.C_MAX < currentBestCmax)
        {
            currentBestCmax = dto.C_MAX;
            currentBestOrder = dto.order;
        }
        // TODO: znaleźć zadania A, B, C

        
        
        
        
    /*
        for (auto &obj : result){
            obj.showJob();
        }
    */



       // std::cout << "\n Current data set:" << dataSetIndex << " C_max = " << C_max << "\n";


        jobs.clear();
    }


    return 0;
}

/*
 Zadanie dostępne to zadanie dla którego minął już czas R ale jeszcze nie minął czas P

t
tnext
currentJob

t=std::min(t+currentJob.P, tnext)

zmniejszamy currentJob.P o tyle ile sie przesuneliśmy jednostek

// to obliczamy w przypadku wyzerowania pola P currentJob
Cmax = std::max(t + currentJob.Q, Cmax)
*/