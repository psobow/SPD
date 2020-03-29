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

// Funkcja dodaje zadanie o największym Q do vectora result
// zwraca kolejność uszeregowania
        // TODO: to nie musi nic zwracać bo i tak operuje na refernecji
std::vector<Job> & addNewJobWithBiggestQ(std::vector<Job> & jobs, std::vector<int> & availbleTasksIndices, 
                                        std::vector<Job> &resultJobs, int & elapsedTime){

    int biggestQIndex = availbleTasksIndices[0];
    int biggestQ = jobs[biggestQIndex].Q;

    for (int index : availbleTasksIndices){
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







    //--------------------------------------------------------------
    // znaleźlismy index zadania z największym Q. teraz musimy sprawdzić czy w trakcie czasu P tego zadania 
    // pojawi się zadanie z wiekszym Q 

    // jeśli pojawi się takie zadanie to wtedy zmniejszamy P pierwszego zadania które się wykonywało, o tyle ile się wykonalo
    // oraz zwiększamy elapsedTime o tyle ile wykonalo się pierwsze zadanie
    // i zaczynamy wykonywać to drugie zadanie.
    // zadanie pierwsze powraca do póli zadań dostępnych.

    // chyba rekurencyjnie to będziez trzeba zrobić 


    resultJobs.push_back(jobs[biggestQIndex]);
    elapsedTime += jobs[biggestQIndex].P;

    //jobs.erase(jobs.begin() + biggestQIndex -1);  // munmap_chunk(): invalid pointer // to przez tą minus jedynke

    jobs[biggestQIndex].R = std::numeric_limits<int>::max(); // podmieniamy wartość R zadania które własnie dodaliśmy do kolekcji result.
    // powodem tej podmiany jest to że keidy usuwaliśmy za pomocą erase() to wylatywał nam wyjątek munmap_chunk();

    return resultJobs;
}

int main(int argc,char **argv){
    
    std::vector<Job> jobs, result;
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
        try 
		{
			if (quantityOfData == 0) throw std::runtime_error( std::string("Could not read data from file. (probably inappropriate end of line character in variable currentDataSet)\n") + 
				"Try \\n, \\n\\r, \\r, or none" );
		}
		catch (const std::exception & e)
		{
			std::cerr << e.what() << "\n";
			exit(-1);
		}

        // read data
                
        for(int i = 0; i < quantityOfData; i++){
            myfile >> temporaryJob.R >> temporaryJob.P >> temporaryJob.Q;
            temporaryJob.initialIndex = i;

            jobs.push_back(temporaryJob);
        }

        // Posortujmye vector prac po R rosnąco

        std::sort(jobs.begin(), jobs.end(), [] (const Job & first, const Job & second ){ return first.R < second.R; });

    
        // Pierwsze dostępne zadanie będzie zadaniem o najmniejszym R
        // Zadanie to będzie dostępne w momencie elapsedTime równym jego R 
        Job jobWithSmallestR = jobs[0];
        int elapsedTime = jobWithSmallestR.R; 

        int initialQuantitiOfJobs = jobs.size();
        std::vector<int> indicesAvailableJobs;

        // Shrange zwykłe

        // wykonujemy dopóki w kolekcji result jest mniej prac niż prac wczytanych

        while (result.size() != initialQuantitiOfJobs){

            // Z puli zadań wyszukujemy te indexy zadań których R jest mniejsze bądź równe elapsedTime
            // oraz nie bierzemy pod uwage zadań uprzednio dodanych do kolekcji result

            indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime); 

            // W przypadku gdy obecnie nie ma żdanych dostępnych zadań w czasie elapsedTime
            // To w kolekcji z zadaniami wyszukujemy pierwsze zadanie którego R jest większe od elapsedTime oraz różne od MAX_INT (ponieważ po dodaniu zadania do result, jego wartość R podmieniamy na MAX_INT)

            if(indicesAvailableJobs.size() == 0){
                // W tym przypadku znaleźć pierwsze większe R niz elapsedTime z vectora jobs i przypisac je do elapsed time
                for (int i = 0; i < jobs.size(); i++){
                    if(jobs[i].R > elapsedTime && (jobs[i].R != std::numeric_limits<int>::max())){
                        elapsedTime = jobs[i].R;
                        // kiedy znaleźliśmy następne elapsedTime dla którego będą jakieś zadania, to ponownie wyszukujemy dostępne zadania
                        indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime); 
                        break;
                    }
                }

            }

            // z puli dostępnych zadań, dodaj zadanie do kolekcji result na podstawie parametru Q
            // w tej funkcji dzieje sie wiele rzeczy.
            // 1. dodajemy zadanie o najwiekszym Q do kolekcji result
            // 2. zwiększamy elapsed time o wartość P zadania które dodaliśmy do kolekcji result
            // 3. podmieniamy wartość R zadania na MAX_INT aby pomijać to zadanie w kolejnych iteracjach.
            result = addNewJobWithBiggestQ(jobs, indicesAvailableJobs, result, elapsedTime);
        
        }
        
        
        
    /*
        for (auto &obj : result){
            obj.showJob();
        }
    */

        int C_max = calculateC_MAX(result);

        std::cout << "\n Current data set:" << dataSetIndex << " C_max = " << C_max << "\n";
        for (auto &obj : result){
            std::cout << obj.initialIndex+1 << ", ";
        }
        std::cout << "\n";

        result.clear();
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
