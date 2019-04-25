//
//  main.cpp
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include <iostream>
#include <vector>
#include <fstream>
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

void readPattern(std::istream &in) {
    long long startX = 0, startY = 0;
    long long x = startX, y = startY;
    std::string str;
    
    unsigned int _num = 0;
    
    while(std::getline(in, str)) {
        if(str.size() < 1)
            return;
        
        if(str.front() == '#' || str.front() == 'x') {
            std::cout << str << std::endl;
            continue;
        }
        
        for(char& c : str) {
            unsigned int num = _num == 0 ? 1 : _num;
            
            if(c == 'b') {
                while(num-- > 0)
                    life->set(x++, y, 0);
                _num = 0;
            }else if(c == 'o') {
                while(num-- > 0)
                    life->set(x++, y, 1);
                _num = 0;
            }else if(c == '$') {
                y += num;
                x = startX;
                _num = 0;
            }else if(c >= '0' && c <= '9') {
                _num = _num * 10 + c - '0';
            }else if(c == '!') {
                return;
            }else{
                std::cout << "error in rle" << std::endl;
                return;
            }
        }
    }
}

int n = 500;

int x = -1;
int y = -1;

int main(int argc, const char * argv[]) {
    Node::init();
    
    life = new HashLife();
    
    std::ifstream file("spacefiller.rle");
    
    readPattern(file);
    
    printf("starting population: %.0Lf \n", life->getRoot()->population);
    
    mark;
    
    for(int i = 0; i < n; ++i) {
        life->step();
    }
    
    mark;
    
    printAll();
    
    printf("population: %.0Lf \n", life->getRoot()->population);
    printf("gen: %.0Lf \n", life->generations);
    
    Node::free();
    
    delete life;
    
    return 0;
}
