#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <string>

std::vector<int> R, P, Q;
std::string line;
int quantityOfDataLines = 0;
std::ifstream myfile("data.txt");


void printVector(const std::vector<int> VEC);
bool isVectorContainingOnlyBigNumbers(const std::vector<int> VEC, const int BIG_NUMBER);
std::vector<int> getNewOrderWithSortR(std::vector<int> localR);
std::vector<int> getReorganisedVector(const std::vector<int> VEC, const std::vector<int> NEW_ORDER);
void printVectorWithIncrementedValues(std::vector<int> VEC);
int calculateC_MAX(std::vector<int> localR, std::vector<int> localP, std::vector<int> localQ);

int main(){
    std::cout << "Hello World!\n";
    std::string dataName;
    int tempR, tempP, tempQ;
    std::vector<int> newOrder;
            
    std::vector<int> newR;
    std::vector<int> newP;
    std::vector<int> newQ;

    std::vector<int> Cmax;

    if (myfile.is_open()) {

        for(int i = 1; i < 5; i++){
            dataName = "data." + std::to_string(i);

            // Skip useless data
            while ( getline(myfile, line) && line != dataName ) {}

            // read quantity of measures
            myfile >> quantityOfDataLines;
            
            // read data
            
            for(int i = 0; i < quantityOfDataLines; i++){
                myfile >> tempR >> tempP >> tempQ;
                R.push_back( tempR );
                P.push_back( tempP );
                Q.push_back( tempQ );
            }

            
            std::vector<int> newOrder = getNewOrderWithSortR(R);
            
            std::vector<int> newR = getReorganisedVector(R, newOrder);
            std::vector<int> newP = getReorganisedVector(P, newOrder);
            std::vector<int> newQ = getReorganisedVector(Q, newOrder);
            
            Cmax.push_back( calculateC_MAX(newR,newP,newQ) );

            std::cout << "New order for " + dataName + " with SortR "; printVectorWithIncrementedValues( newOrder ); 
            std::cout << "Calculated Cmax for " + dataName + " = " << Cmax[i-1] << "\n"; // has to be i-1 cuz we got date from 1 to 4 while Cmax start indexing from zero

            //clear vectors
            R.clear(); P.clear(); Q.clear();
            newOrder.clear(); newR.clear(); newP.clear(); newQ.clear();
        }
        myfile.close();

        std::cout << "Sum for every Cmax = " << Cmax[0] + Cmax[1] + Cmax[2] + Cmax[3] << "\n";
    
    } else {
        std::cerr << "Unable to open file. Exit program.\n";
    }
    return 0;
}




std::vector<int> getNewOrderWithSortR(std::vector<int> localR){
    const int BIG_NUMBER = 1999999999;

    std::vector<int> newOrder;
    int smallestNumber;
    int smallestNumberIndex;

    while ( isVectorContainingOnlyBigNumbers(localR, BIG_NUMBER) == false ) {

        smallestNumber = localR[0];
        smallestNumberIndex = 0;

        // find index of the smallest number in vector
        for(int i = 0; i < localR.size(); i++){
            if(smallestNumber > localR[i] ){

                smallestNumber = localR[i];
                smallestNumberIndex = i;
            }
        }
        newOrder.push_back( smallestNumberIndex );
        localR[smallestNumberIndex] = BIG_NUMBER; // use assignment instead of erase!! cuz with erase() you modifying initial order!!
    }
    return newOrder;
}
bool isVectorContainingOnlyBigNumbers(const std::vector<int> VEC, const int BIG_NUMBER){
    for(int i = 0; i < VEC.size(); i++){
        if (VEC[i] != BIG_NUMBER) return false;
    }
    return true;
}

std::vector<int> getReorganisedVector(const std::vector<int> VEC, const std::vector<int> NEW_ORDER){
    if( VEC.size() != NEW_ORDER.size()){
        throw std::invalid_argument("Invalid arguments! Vectors sizes are diffrent. can not get reorder vector.\n");
    }

    std::vector<int> reOrderedVector;
    int indexOfNextSmallestNumber;

    for(int i = 0; i < NEW_ORDER.size(); i++){
        indexOfNextSmallestNumber = NEW_ORDER[i];
        reOrderedVector.push_back( VEC[indexOfNextSmallestNumber] );
    }
    return reOrderedVector;
}

void printVector(const std::vector<int> VEC){
    std::cout << " [";
    for(int i = 0; i < VEC.size(); i++){
        std::cout << " " << VEC[i];
    }
    std::cout << " ]\n";
}



void printVectorWithIncrementedValues(std::vector<int> VEC){
    for(int i = 0; i < VEC.size(); i++ ) VEC[i]++;
    printVector(VEC);
}




int calculateC_MAX(std::vector<int> localR, std::vector<int> localP, 
    std::vector<int> localQ ){
    

    int m = 0 , c = 0;
    for(int i = 0; i < localR.size(); i++){
        m = std::max(localR[i], m) + localP[i];
        c = std::max(c , m + localQ[i]);
    }

    return c;
}