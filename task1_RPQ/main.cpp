#include <iostream>
#include <fstream>
#include <vector>

std::vector<int> R(100, 0), P(100, 0), Q(100, 0);

std::string line;
int quantityOfDataLines = 0;
std::ifstream myfile("data.txt");

bool isVectorContainingOnlyBigNumbers(const std::vector<int> VEC, const int BIG_NUMBER);
void printVector(const std::vector<int> VEC, const int SIZE );

// R algorithm require R Vector to calculate new order
std::vector<int> getNewOrderWithsortR(std::vector<int> localR, const int SIZE);


int main(){
    std::cout << "Hello World!\n";
    

    if (myfile.is_open()) {

        // Skip useless data
        while ( getline(myfile, line) && line != "data.1") {}

        // read quantity of measures
        myfile >> quantityOfDataLines;

            // std::cout << "ostatnia wczytana linia: " << line << "\n";
            // std::cout << "ilosc pomiarow: " << quantityOfDataLines << "\n";
        
        // read valid data
        for(int i = 0; i < quantityOfDataLines; i++){
            myfile >> R[i] >> P[i] >> Q[i];
        }

        std::cout << "input array content: "; printVector(R, quantityOfDataLines); std::cout << "\n";
        std::vector<int> newOrder = getNewOrderWithsortR(R, quantityOfDataLines);

        std::cout << "new order : "; printVector(newOrder, quantityOfDataLines); std::cout<< "\n";

        myfile.close();        
    
    } else {
        std::cerr << "Unable to open file. Exit program.\n";
    }
    

    return 0;
}

std::vector<int> getNewOrderWithsortR(std::vector<int> localR, const int SIZE){
    const int BIG_NUMBER = 1999999999;

    // cut vec to required length
    std::vector<int> temporaryR;
    for(int i = 0; i < SIZE; i++){
        temporaryR.push_back(localR[i]);
    }

    std::vector<int> newOrder;
    
    int smallestNumber;
    int smallestNumberIndex;
    while ( isVectorContainingOnlyBigNumbers(temporaryR, BIG_NUMBER) == false ) {

        smallestNumber = temporaryR[0];
        smallestNumberIndex = 0;

        // find index of the smallest number in vector
        for(int i = 0; i < temporaryR.size(); i++){
            if(smallestNumber > temporaryR[i] ){

                smallestNumber = temporaryR[i];
                smallestNumberIndex = i;
            }
        }
        newOrder.push_back( smallestNumberIndex+1 );
        temporaryR[smallestNumberIndex] = BIG_NUMBER; // use assignment instead of erase!! cuz in erase you modifying order!!
    }
    return newOrder;
}

bool isVectorContainingOnlyBigNumbers(const std::vector<int> VEC, const int BIG_NUMBER){
    for(int i = 0; i < VEC.size(); i++){
        if (VEC[i] != BIG_NUMBER) return false;
    }
    return true;
}

void printVector(const std::vector<int> VEC, const int SIZE){
    std::cout << " [";
    for(int i = 0; i < SIZE; i++){
        std::cout << " " << VEC[i];
    }
    std::cout << " ]\n";
}