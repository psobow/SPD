#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <cmath>
#include <limits>
#include <algorithm>



int main(){

    std::ifstream myfile("witi.data.txt");
    int quantityOfData = 0;

    const int MAX_QUANTITY_OF_DATA = 20;
    int taskDurance[MAX_QUANTITY_OF_DATA], 
        taskPriority[MAX_QUANTITY_OF_DATA], 
        desirableDurance[MAX_QUANTITY_OF_DATA];

    std::string line;

    std::cout << "Hello World!" << std::endl;

    if( ! myfile.is_open()) throw std::runtime_error("Could not open file");

    for(int dataIndex = 10; dataIndex <= 20; dataIndex++){
        while ( getline(myfile, line) && (line != "data." + std::to_string(dataIndex) + ":\r") ); // skip useless data


        myfile >> quantityOfData;
        for(int i = 0; i < quantityOfData; i++){
            myfile >> taskDurance[i]
                   >> taskPriority[i]
                   >> desirableDurance[i];
        }

        // estimate subsets quantity
        int subSetQuantity = static_cast<int>( pow(2.0, static_cast<double>(quantityOfData) ) );

        std::vector<int> subSetVector(subSetQuantity);

        subSetVector[0] = 0; // first element has to be equal zero

        int sumOfTasksDuranceInSubSet = 0;

        for(int subSetIndex = 1; subSetIndex < subSetQuantity; subSetIndex++){
            sumOfTasksDuranceInSubSet = 0;

            for(int i = 0, b = 1; i < quantityOfData; i++, b*=2){
                /*
                parameter i is task index in tables,
                parameter b is consecutive power of 2  (eg. for 10 input data: 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024) 
                */

            if(subSetIndex&b) sumOfTasksDuranceInSubSet += taskDurance[i];
            }

            subSetVector[subSetIndex] = std::numeric_limits<int>::max();

            for(int i = 0, b = 1; i < quantityOfData; i++, b*=2){
                if(subSetIndex&b) {
                    // PD algorithm
                    subSetVector[subSetIndex] = std::min( 
                        subSetVector[subSetIndex], 
                        subSetVector[subSetIndex - b] + taskPriority[i] * std::max(0, sumOfTasksDuranceInSubSet - desirableDurance[i]) );
                }
            }
        }
        std::cout << "data." << dataIndex << "  Optimal time: " << subSetVector.back() << "\n";
        subSetVector.clear();
    }    
    

    myfile.close();

    return 0;
}