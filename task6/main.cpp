#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <limits>
#include <numeric>

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

	bool stickWithLeftSideJob = false;

	int timeWhenJobBegin = 0;

    Job job() {};

    void showJob(){
        std::cout << "R: " << R << " P: " << P << " Q: " << Q << " init index: " << initialIndex << "\n";
    }

	void showInitIndex()
	{
		std::cout << initialIndex << " ";
	}

	void showStickness()
	{
		std::cout << stickWithLeftSideJob << " ";
	}

	void showTimeWhenJobBegin()
	{
		std::cout << timeWhenJobBegin << " ";
	}
};

int calculateC_MAX(const std::vector<Job> & jobs){
    int m = 0 , c = 0;
    for(int i = 0; i < jobs.size(); i++){
        m = std::max(jobs[i].R, m) + jobs[i].P;
        c = std::max(c , m + jobs[i].Q);
    }
    return c;
}

// Zadanie o indexie index jest uwzględniane w liczonym CMAX
int calculateC_MAX(const std::vector<Job> & jobs, const int index){
    int m = 0 , c = 0;
    for(int i = 0; i <= index; i++){
        m = std::max(jobs[i].R, m) + jobs[i].P;
        c = std::max(c , m + jobs[i].Q);
    }
    return c;
}

struct DTO // Data Transfer Object
{
    std::vector<Job> order;
    int C_MAX;
};




/*
 Funkcja znajduje indexy dostępnych zadań w przedziale czasu od zera do argumentu elapsedTime włącznie
*/
std::vector<int> getIndiciesAvailableJobs(const std::vector<Job> & jobs, const int elapsedTime){
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
	std::sort(jobs.begin(), jobs.end(), [](const Job & first, const Job & second) { return first.R < second.R; });
    std::vector<Job> result;

    int elapsedTime = jobs[0].R;
    int newElapsedTime = 0;
    int elapsedTimeNext;
    int Cmax = 0;
    int biggestQIndex;
    int biggestQ;

	int auxiliaryVariableForEstimatingStickness = 0;

    std::vector<int> indicesAvailableJobs;


    while (jobs.size() != 0){
        indicesAvailableJobs = getIndiciesAvailableJobs(jobs, elapsedTime);

        if (indicesAvailableJobs.size() != 0)
		{ 
            biggestQIndex = indicesAvailableJobs[0];
            biggestQ = jobs[biggestQIndex].Q;

            for (int index : indicesAvailableJobs){
                if(biggestQ < jobs[index].Q){ 
                    biggestQ = jobs[index].Q;
                    biggestQIndex = index;

				}
				else if (biggestQ == jobs[index].Q) { // jeżeli takie same Q to bierzemy o mniejszym indexie początkowym

					if (jobs[biggestQIndex].initialIndex > jobs[index].initialIndex) {
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
				Ten fragment jest potrzebny do poszukiwania zadania A w algorytmie Cariera
				mianowicie jeśli conajmniej dwa razy z rzędu wszedłem do tego if'a to znaczy że pomiędzy tymi zadaniami nie ma dziury.
				nie ma dziury ponieważ elapsedTime przestawiłem na elapsedTime += jobs[biggestQIndex].P; czyli następne zadanie jest dostępne odrazu po zakończeniu obecnego

				Gdybym musiał przestawić elapsedTime na elapsedTime = elapsedTimeNext; to znaczy że pomiędzy zadaniami byłaby dziura. ponieważ po zakońzceniu czasu P obecnego zadania 
				kolejne zadanie jeszcze nie jest gotowe i trzeba czekać.

				Ponadto za każdym razem kiedy pojawi się dziura to resetuje wartość pomocnicznej zmiennej do sprawdzania przyklejenia mianowocie ustawiam ją na zero
				auxiliaryVariableForEstimatingStickness = 0;

			*/
			auxiliaryVariableForEstimatingStickness++;
			if (auxiliaryVariableForEstimatingStickness > 1)
			{
				jobs[biggestQIndex].stickWithLeftSideJob = true;
			}

			// To pole będzie na potrzebne w algorytmie Cariera podczas szukania zadania A
			jobs[biggestQIndex].timeWhenJobBegin = elapsedTime;

            elapsedTime += jobs[biggestQIndex].P;



            Cmax = std::max(elapsedTime + jobs[biggestQIndex].Q, Cmax);
            
            result.push_back(jobs[biggestQIndex]);

            // usunać z kolekcji jobs obiekt pod indexem biggestQIndex
            jobs.erase(jobs.begin() + biggestQIndex);

        } else {
            elapsedTime = elapsedTimeNext;
			auxiliaryVariableForEstimatingStickness = 0;
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
	std::sort(jobs.begin(), jobs.end(), [](const Job & first, const Job & second) { return first.R < second.R; });
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
            


            
            // jeśli P zmniejszyło się do zera to obliczay Cmax i usuwamy tą prace z kolekcji
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


// Funkcja zwraca index zadania B, zadanie B zawsze będzie znalezione
// Do funkcji musimy przekazać kolekcje posortowaną w shrageNormal
int findBjob(const std::vector<Job> & jobs)
{
	/*
	Poszukiwanie zadania B:
	liczym Cmax całej kolekcji jobs.
	zaczynamy od zadania najbardziej na prawo w kolekcji jobs.
	przesuwamy się w każdej iteracji o jedno zadanie w lewo i liczym Cmax.
	w każdej iteracji liczymy Cmax, jeśli obliczone Cmax będzie różne od początkowego Cmax to znaczy że zadanie o poprzednim indexie jest zadaniem B
	*/

	int initialCmax = calculateC_MAX(jobs);
	int jobsSize = jobs.size();
	int resultIndex = 0;

	for (int jobIndex = jobsSize - 1; jobIndex >= 0; jobIndex--)
	{
		if (calculateC_MAX(jobs, jobIndex) != initialCmax)
		{
			resultIndex = jobIndex + 1;
			break;
		}
	}
	return resultIndex;
}

// Zadanie A nie zawsze musi się znajdować w kolekcji
// Do funkcji musimy przekazać kolekcje posortowaną w shrageNormal
int findAjob(const std::vector<Job> & jobs, const int indexOfBJob)
{
	/*
	Zadanie A musi się znajdować w jednym bloku razem z zadaniem B. te zadania muszą być przyklejone do siebie 
	
	    W algorytmie shrageNormal oceniamy które zadania są przyklejone do tego polewej
		Oraz obliczamy faktyczny czas w którym zadanie zacznie się wykonywać  

		czyli teraz musimy zanleźć granice jednolitego bloku zadań w którym znajduje się zadanie B
		następnie w tym bloku musimy znaleźć pierwsze zadanie od lewej którego czas rozpoczęcia minus jego R wynosi zero
	*/

	// TODO: co w przypadku kiedy rozmiar bloku wynosi tylko jedno zadanie? czyli zadanie B nie jest przyklejone do niczego.

	// zaczynamy od zadania o indexie indexOfBJob idziemy w lewo i szukamy pierwszego zadania którego pole stickWithLeftSideJob wynosi false
	int indexOfLeftBound = -1;
	for (int index = indexOfBJob; index >= 0; index--)
	{
		if (jobs[index].stickWithLeftSideJob == false)
		{
			indexOfLeftBound = index;
			break;
		}
	}
	// po tej pętli znaleźliśmy index lewej granicy bloku zadań. (zadanie o indexie indexOfLeftBound jest ostatnim przyklejonym zadaniem w bloku razem z zadaniem B)
	// teraz w tym bloku szukamy od lewej pierwszego zadania którego fatkyczny czas rozpoczęcia minus jego R wynosi zero.
	int resultIndexOfAJob = -1;
	for (int index = indexOfLeftBound; index <= indexOfBJob; index++)
	{
		if ( (jobs[index].timeWhenJobBegin - jobs[index].R) == 0)
		{
			resultIndexOfAJob = index;
			break;
		}
	}

	
	return resultIndexOfAJob; // może zwrócić -1
}

// Zadanie C nie zawsze musi się znajdować w kolekcji
// Do funkcji musimy przekazać kolekcje posortowaną w shrageNormal
int findCJob(const std::vector<Job>& jobs, const int indexOfAJob, const int indexOfBJob )
{	
	/*
	   Teraz w kolekcji jobs rozpatrujemy blok zadań zaczynający się w zadaniu o indexie indexOfAJob do zadania o indexie indexOfBJob włącznie z tymi zadaniami.
	   szukamy w tym bloku takiego zadania którego Q jest mniejsze od Q zadania B
	   jeśli Q są takie same to bierzemy to najbardziej na prawo (czyli zaczniemy przeszukiwanie od prawej do lewej i zwracamy pierwsze znalezione)
	*/
	
	int resultIndexOfCJob = -1;
	for (int index = (indexOfBJob - 1); index >= indexOfAJob; index--)
	{
		if (jobs[index].Q < jobs[indexOfBJob].Q)
		{
			resultIndexOfCJob = index;
			break;
		}
	}
	return resultIndexOfCJob; // może zwrócić -1
}


DTO carier(DTO initDto) 
{

	DTO dto = shrageNormal(initDto.order);
	/*std::vector<Job> currentBestOrder;

	if (dto.C_MAX < currentBestCmax)
	{
		currentBestCmax = dto.C_MAX;
		currentBestOrder = dto.order;
	}*/

	int indexOfBJobInsideDTOorder = findBjob(dto.order);
	int indexOfAJobInsideDTOorder = findAjob(dto.order, indexOfBJobInsideDTOorder);
	int indexOfCJobInsideDTOorder = findCJob(dto.order, indexOfAJobInsideDTOorder, indexOfBJobInsideDTOorder);
	
	if (indexOfCJobInsideDTOorder == -1)
	{
		return dto;
	}
	
	std::vector<Job> K;
	for (int index = (indexOfCJobInsideDTOorder + 1); index <= indexOfBJobInsideDTOorder; index++)
	{
		K.push_back(dto.order[index]);
	}
	
	
	int smallestRinK = (*std::min_element(K.begin(), K.end(),
		[] (const Job & lhs, const Job & rhs) 
		{
			return lhs.R < rhs.R;
		})).R;
	
	int smallestQinK = (*std::min_element(K.begin(), K.end(),
		[](const Job & lhs, const Job & rhs) 
		{
			return lhs.Q < rhs.Q;
		})).Q;
	
	int sumOfEveryPparametersInK = std::accumulate(K.begin(), K.end(), 0,
		[](int i, const Job & job) 
		{ 
			return job.P + i; 
		});

	
	int copyValueRofCJobInsideDTOorder = dto.order[indexOfCJobInsideDTOorder].R;

	// linijka 14
	dto.order[indexOfCJobInsideDTOorder].R = std::max(copyValueRofCJobInsideDTOorder, (smallestRinK + sumOfEveryPparametersInK));
	

	int CmaxFromShragerInterrupt = shrageWithInterrupt(dto.order);
	
	//Niewiadomo w które miejsce dołaczyć to zadanie C do zbioru K // Chyba na początek
	
	std::vector<Job> deepCopyOfK = K;
	
	deepCopyOfK.insert(deepCopyOfK.begin(), dto.order[indexOfCJobInsideDTOorder]);

	int H_OD_K = smallestRinK + sumOfEveryPparametersInK + smallestQinK;


	CmaxFromShragerInterrupt = std::max(H_OD_K, CmaxFromShragerInterrupt);
	

	if (CmaxFromShragerInterrupt < dto.C_MAX)
	{
		dto = carier(dto);
	}
	
	dto.order[indexOfCJobInsideDTOorder].R = copyValueRofCJobInsideDTOorder;

	int copyValueQofCJobInsideDTOorder = dto.order[indexOfCJobInsideDTOorder].Q;

	dto.order[indexOfCJobInsideDTOorder].Q = std::max(copyValueQofCJobInsideDTOorder, (smallestQinK + sumOfEveryPparametersInK));

	CmaxFromShragerInterrupt = shrageWithInterrupt(dto.order);


	H_OD_K = smallestRinK + sumOfEveryPparametersInK + smallestQinK;

	CmaxFromShragerInterrupt = std::max(H_OD_K, CmaxFromShragerInterrupt);

	if (CmaxFromShragerInterrupt < dto.C_MAX)
	{
		dto = carier(dto);
	}
	dto.order[indexOfCJobInsideDTOorder].Q = copyValueQofCJobInsideDTOorder;
	
	return dto;
	




	/*
	linijka 12:
	zrobić kolekcje K na zadania albo indexy pomiedzy C - B
	Czyli jak C to jest 1 
	oraz B to jest 4
	to wtedy K to 2,3,4
	
	Linijka 13:
	wśrod zadań z kolekcji K znaleźć najmniejszy parametr R, to będzie r(K)
	oraz najmniejszy parametr Q to będzie q(K)
	(szukamy samych wartości R i Q a nie zadań które mają te wartości)

	musimy znaleźć p(k) I to jest suma wszystkich parametrów P wszystkich zadań w wektorze K, Czyli p zadania 2 plus p zadania 3 plus p zadania 4

	linijka 14:
	Tam masz tak że rpi(c) to jest r zadania c
	oraz r(K) + p(K)
	i z tych dwóch wartosci wbieram większą

	linjka 15:
	liczmy Cmax za pomocą shrage z przerwaniami na kolekcji jobs to będzie LB

	linijka 16:
	h(k) to Cmax vectora K
	I h(k plus c) czyli cmaxa wektora k do którego wrzucisz c kurwa ale nie wiadomo gdzie to jebane zadanie wrzucić czy na początek czy na koniec

	*/

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
            temporaryJob.P_whichWillBeModifyInInterruptShrage = temporaryJob.P;

            jobs.push_back(temporaryJob);
        }

        // Posortujmye vector prac po R rosnąco

        std::sort(jobs.begin(), jobs.end(), [] (const Job & first, const Job & second ){ return first.R < second.R; });


        

        // Carier
		dto.order = jobs;
		dto = carier(dto);
		std::cout << "Current data set : " << dataSetIndex << "\n";
		std::cout << "dto.cmax : " << dto.C_MAX << "\n\n";
		std::cout << "Order: [";
		for (auto& obj : dto.order) {
			obj.showInitIndex();
		}
		std::cout << "]\n\n";

		// std::cout << "\n Current data set:" << dataSetIndex << " C_max = " << C_max << "\n";






		
        // TODO: znaleźć zadania A, B, C




		// Testy poszukiwania zadania B: dla zestawu danych data.009
		/*
		// Cmaxy kolejny bloków zadań
		for (int i = 0; i < dto.order.size(); i++)
		{
			std::cout << calculateC_MAX(dto.order, i) << ", ";
		}
		std::cout << "\n";
		//std::cout << "Parametr Q zadania pod indexem 4: " << dto.order[4].Q << "\n";
		std::cout << "index zadania B zwrocony z funkcji findBJob: " << findBjob(dto.order) << "\n";
        */



		// Testy wyznaczenia przyklejonych zadań do siebie
		/*
		std::cout << "Kolejnosc zadan shrageNormal: ";
		for (auto& obj : dto.order) {
			obj.showInitIndex();
		}
		std::cout << "\n";

		for (auto& obj : dto.order) {
			obj.showStickness();
		}
		std::cout << "\n";
		*/




        
		// Testy obliczenia czasu w którym zadanie rozpocznie się wykonywać, jets to ptorzebne do obliczenia zadania A w algorytmie Cariera
		/*
		std::cout << "Kolejnosc zadan shrageNormal: ";
		for (auto& obj : dto.order) {
			obj.showInitIndex();
		}
		std::cout << "\n";

		for (auto& obj : dto.order) {
			obj.showTimeWhenJobBegin();
		}
		*/
        

		// Testy poszukiwania zadania B a później zadania A a później zadania C dla zestawu danych data.009
			/*
		std::cout << "Kolejnosc zadan po shrageNormal wzgledem indexow poczatkowych:\n";
		for (auto& obj : dto.order) {
			obj.showInitIndex();
		}
		std::cout << "\n";

		// Cmaxy kolejny bloków zadań
		std::cout << "Cmaxy kolejny blokow zadan: \n";
		for (int i = 0; i < dto.order.size(); i++)
		{
			std::cout << calculateC_MAX(dto.order, i) << ", ";
		}
		std::cout << "\n";

		std::cout << "Testy przyklejenia kolejnych zadan do siebie: (jedynka oznacza ze zadanie jest sklejone z zadaniem po lewej\n";
		for (auto& obj : dto.order) {
			obj.showStickness();
		}
		std::cout << "\n";

		int indexOfBJob = findBjob(dto.order);

		std::cout << "Indexy zadan A B C nie odnosza sie do indexow initialIndex." << "\n";
		std::cout << "Indexy zadan A B C odnosza sie do indexow zadan w kolekcji posortowanej przez shrageNormal" << "\n";
		std::cout << "czyli zadanie B znajduje sie pod czwartym indexem w kolekcji: [5 0 1 2 3 4 6]" << "\n";

		std::cout << "index zadania B : " << indexOfBJob << "\n";

		int indexOfAJob = findAjob(dto.order, indexOfBJob);

		std::cout << "index zadania A : " << findAjob(dto.order, indexOfBJob) << "\n";

		std::cout << "index zadania C : " << findCJob(dto.order, indexOfAJob, indexOfBJob) << "\n";

      

	   */




        jobs.clear();
    }
	

    return 0;
}


/*
Notatki do shrage z przerwaniami
 Zadanie dostępne to zadanie dla którego minął już czas R ale jeszcze nie minął czas P

t
tnext
currentJob

t=std::min(t+currentJob.P, tnext)

zmniejszamy currentJob.P o tyle ile sie przesuneliśmy jednostek

// to obliczamy w przypadku wyzerowania pola P currentJob
Cmax = std::max(t + currentJob.Q, Cmax)
*/