//
//  main.cpp
//  AlgoIIWeek5
//
//  Created by Chris Welhoelter on 4/29/15.
//  Copyright (c) 2015 Chris Welhoelter. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <math.h>
#include <limits>

// helper function declaraion
size_t nextSet(size_t x); // Gosper's hack

// main
int main(int argc, const char * argv[]) {
    
    std::string fileName;
    std::cout << "enter the file name: ";
    std::cin >> fileName;
    std::cout << std::endl;
    
    std::fstream fsIn;
    fsIn.open(fileName);
    if (!fsIn.is_open()){
        std::cout << "could not open " << fileName << std::endl;
        exit(EXIT_FAILURE);
    }
    
    size_t cityCount;
    fsIn >> cityCount;
    
    std::vector<std::pair<float, float>> cities(cityCount);
    for (size_t i = 0; i < cityCount; i++){
        fsIn >> cities[i].first;
        fsIn >> cities[i].second;
    }
    
    // initialize distanceTable and cache the distance between each pair of cities
    std::vector<std::vector<float>> distanceTable(cityCount, std::vector<float>(cityCount));
    for (size_t i = 0; i < cityCount; i++){
        for (size_t j = 0; j < cityCount; j++){
            distanceTable[i][j] = sqrt(pow((cities[i].first - cities[j].first), 2) + pow((cities[i].second - cities[j].second), 2));
        }
    }
    
    // initialize tspTable, our main subproblem table
    std::vector<std::vector<float>> tspTable(pow(2, cityCount - 1), std::vector<float>(cityCount, std::numeric_limits<float>::infinity()));
    
    // base cases
    tspTable[0][0] = 0;
    
    for (size_t i = 0; i < cityCount - 1; i++){
        tspTable[1 << i][i] = distanceTable[0][i + 1];
    }
    
    
    // main loop
    size_t highBit = 1 << (cityCount - 1);
    for (size_t m = 2; m < cityCount; m++){
        size_t curCitySet = (1 << m) - 1;  // initialize curCitySet to be "least" bit array with m elements
        while (!(curCitySet & highBit)){
            
            // for each potential city j
            for (size_t j = 0; j < cityCount - 1; j++){
                // if j is included in curCitySet
                if ((1 << j) & curCitySet){
                    // for each potential city k
                    for (size_t k = 0; k < cityCount - 1; k++){
                        // if k is included in curCitySet and is not j
                        if (((1 << k) & curCitySet) && k != j){
                            // tspTable[curCitySet][j] = min{tspTable[curCitySet - {j}][k] + distanceTable[k][j]}
                            if (tspTable[curCitySet ^ (1 << j)][k] + distanceTable[k + 1][j + 1] < tspTable[curCitySet][j]){
                                tspTable[curCitySet][j] = tspTable[curCitySet ^ (1 << j)][k] + distanceTable[k + 1][j + 1];
                            }
                        }
                    }
                }
            }

            
            curCitySet =  nextSet(curCitySet);
        }
    }
    
    float ret = std::numeric_limits<float>::infinity();
    for (size_t i = 0; i < cityCount - 1; i++){
        if ((tspTable[(1 << (cityCount - 1)) - 1][i]) + distanceTable[i + 1][0]  < ret){
            ret = (tspTable[(1 << (cityCount - 1)) - 1][i]) + distanceTable[i + 1][0];
        }
    }
    
    std::cout << "minimum distance path is " << ret << std::endl;
    
    return 0;
}

// helper function implementation
size_t nextSet(size_t x){ // Gosper's hack
    size_t y = x & -x;
    size_t c = x + y;
    return (((x ^ c) >> 2) / y) | c;
}







