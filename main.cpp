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

HashLife* life;

std::vector<unsigned long> clocks;

#define mark clocks.push_back(clock())

float ms(int i) {
    return 1000.0f / (float) CLOCKS_PER_SEC * (clocks[i + 1] - clocks[i]);
}

void printAll() {
    for(int i = 0; i < clocks.size() - 1; ++i)
        printf("%.5f\n", ms(i));
}

int n = 60;

int x = 0;
int y = 0;

int main(int argc, const char * argv[]) {
    Node::init();
    
    life = new HashLife();
    
    life->set(0 + x, 1 + y, 1);
    life->set(1 + x, 0 + y, 1);
    life->set(2 + x, 0 + y, 1);
    life->set(2 + x, 1 + y, 1);
    life->set(2 + x, 2 + y, 1);
    
    mark;
    
    for(int i = 0; i < n; ++i) {
        life->step();
    }
    
    mark;
    
    printAll();
    
    printf("population: %llu \n", life->getRoot()->population);
        
    printf("root size: %d \n", life->getRoot()->level);
    
    printf("gen: %llu \n", life->generations);
    
    Node::free();
    
    return 0;
}
