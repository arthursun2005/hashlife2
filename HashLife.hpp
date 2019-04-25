//
//  HashLife.hpp
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef HashLife_hpp
#define HashLife_hpp

#include "Node.h"

class HashLife
{
    
protected:
    
    Node* root;
    
    LifeRule rule;
    
public:
    
    long double generations = 0;
    
    HashLife() {
        root = Node::create(4);
    }
    
    ~HashLife() {
    }
    
    void step();
    
    Node*& getRoot();
    
    void set(long long x, long long y, int k);
    
    char at(long long x, long long y) const;
};

#endif /* HashLife_hpp */
