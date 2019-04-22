//
//  main.cpp
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include <iostream>
#include <vector>
#include "HashLife.hpp"

HashLife life;

std::vector<unsigned long> clocks;

#define mark clocks.push_back(clock())

float ms(int i) {
    return 1000.0f / (float) CLOCKS_PER_SEC * (clocks[i + 1] - clocks[i]);
}

void printAll() {
    for(int i = 0; i < clocks.size() - 1; ++i)
        printf("%.5f\n", ms(i));
}

int n = 1;

int main(int argc, const char * argv[]) {
    life.set(0, 1, 1, life.getRoot());
    life.set(1, 0, 1, life.getRoot());
    life.set(2, 0, 1, life.getRoot());
    life.set(2, 1, 1, life.getRoot());
    life.set(2, 2, 1, life.getRoot());
    
    /*
    life.set(0, 0, 1, life.getRoot());
    life.set(1, 0, 1, life.getRoot());
    life.set(0, 1, 1, life.getRoot());
    life.set(1, 1, 1, life.getRoot());
     */
    
    //life.getRoot()->print();
    
    mark;
    
    for(int i = 0; i < n; ++i) {
        life.step();
    }
    
    mark;
    
    printAll();
    
    //life.getRoot()->print();
    
    printf("population: %ld \n", life.getRoot()->population);
        
    printf("root size: %d \n", life.getRoot()->level);
    
    printf("gen: %zu \n", life.generations);
    
    printf("%.4f MB \n", life.poolBytes() * 0.000001f);
    
    return 0;
}
