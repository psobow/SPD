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
    int initialP;

    int initialIndex;

    Job job() {};

    void showJob(){
        std::cout << "R: " << R << " P: " << initialP << " Q: " << Q << " init index: " << initialIndex+1 << "\n";
    }
};

int calculateC_MAX(std::vector<Job> & jobs){
    int m = 0 , c = 0;
    for(int i = 0; i < jobs.size(); i++){
        m = std::max(jobs[i].R, m) + jobs[i].initialP;
        c = std::max(c , m + jobs[i].Q);
    }
    return c;
}




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
        
void shrageWithInterrupt(std::vector<Job> & jobs){
    std::vector<Job> deepCopyJobs = jobs;

    int elapsedTime = deepCopyJobs[0].R;
    int newElapsedTime = 0;
    int elapsedTimeNext;
    int Cmax = 0;
    int biggestQIndex;
    int biggestQ;

    std::vector<int> indicesAvailableJobs;


    while (deepCopyJobs.size() != 0){
        indicesAvailableJobs = getIndiciesAvailableJobs(deepCopyJobs, elapsedTime);

        if (indicesAvailableJobs.size() != 0){
            biggestQIndex = indicesAvailableJobs[0];
            biggestQ = deepCopyJobs[biggestQIndex].Q;

            for (int index : indicesAvailableJobs){
                if(biggestQ < deepCopyJobs[index].Q){ 
                    biggestQ = deepCopyJobs[index].Q;
                    biggestQIndex = index;

                } else if (biggestQ == deepCopyJobs[index].Q){ // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

                    if(deepCopyJobs[biggestQIndex].initialIndex > deepCopyJobs[index].initialIndex){
                        biggestQ = deepCopyJobs[index].Q;
                        biggestQIndex = index;
                    }
                }
            }
            // trzeba znaleźć index pierwszego zadania którego czas R jest wiekszy od elapsedTime, to będzie tNext

            elapsedTimeNext = std::numeric_limits<int>::max();
            for (int i = 0; i < deepCopyJobs.size(); i++) {
                if (deepCopyJobs[i].R > elapsedTime){
                    elapsedTimeNext = deepCopyJobs[i].R;
                    break;
                }
            }


            newElapsedTime = std::min(elapsedTime + deepCopyJobs[biggestQIndex].P, elapsedTimeNext);
            deepCopyJobs[biggestQIndex].P = deepCopyJobs[biggestQIndex].P - (newElapsedTime - elapsedTime);
            elapsedTime = newElapsedTime;
            
            // jeśli P zmniejszyło się do zera to obliczay Cmax i usuwamy tą prace z kolekcji
            if (deepCopyJobs[biggestQIndex].P == 0){
                Cmax = std::max(elapsedTime + deepCopyJobs[biggestQIndex].Q, Cmax);
                // usunać z kolekcji deepCopyJobs obiekt pod indexem biggestQIndex
                deepCopyJobs.erase(deepCopyJobs.begin() + biggestQIndex);
            }
        } else {
            elapsedTime = elapsedTimeNext;
        }
    }    


    std::cout << "Cmax: " << Cmax << "\n";



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

int main(int argc,char **argv){
    
    std::vector<Job> jobs;
    Job temporaryJob;
    


    std::ifstream myfile("schr.data.txt");
    int quantityOfData = 0;

    std::string line;

    std::cout << "Hello world!\n";

    if( ! myfile.is_open()) throw std::runtime_error("Could not open file");

    std::string currentDataSet;

    for(int dataSetIndex = 0; dataSetIndex <= 8; dataSetIndex++){

        currentDataSet = "data.00" + std::to_string(dataSetIndex) + ":\r";

        while (getline(myfile, line) && (line != currentDataSet)); // skip useless data
        myfile >> quantityOfData;

        // read data
                
        for(int i = 0; i < quantityOfData; i++){
            myfile >> temporaryJob.R >> temporaryJob.P >> temporaryJob.Q;
            temporaryJob.initialIndex = i;
            temporaryJob.initialP = temporaryJob.P;

            jobs.push_back(temporaryJob);
        }

        // Posortujmye vector prac po R rosnąco

        std::sort(jobs.begin(), jobs.end(), [] (const Job & first, const Job & second ){ return first.R < second.R; });

    
        // Pierwsze dostępne zadanie będzie zadaniem o najmniejszym R
        // Zadanie to będzie dostępne w momencie elapsedTime równym jego R 


        

        // Shrange przerwania

        shrageWithInterrupt(jobs);

        
        
        
        
        
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