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
#include <unordered_map>

class HashLife
{
    
protected:
        
    Node* root;
        
    std::unordered_map<Node*, Node*> cache;
    
    LifeRule rule;
    
public:
    
    size_t generations = 0;
    
    HashLife() {
        root = Node::create(5);
    }
    
    ~HashLife() {
    }
    
    void step();
    
    Node*& getRoot();
    
    void set(int x, int y, int k);
    
    char at(int x, int y) const;
};

#endif /* HashLife_hpp */
