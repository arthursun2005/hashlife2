//
//  HashLife.hpp
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef HashLife_hpp
#define HashLife_hpp

#include "NodePool.hpp"
#include "HashMap.h"
#include <unordered_map>

class HashLife
{
    
protected:
    
    NodePool pool;
    
    Node* root;
    
    std::unordered_map<Node*, Node*> cache;
    
    //HashMap<Node*, Node*> cache;
    
    LifeRule rule;
    
    Node* solve(Node*);
    
    void expandOnce();
        
public:
    
    size_t generations = 0;
    
    HashLife() {
        root = pool.alloc(3);
        root->clear();
    }
    
    ~HashLife() {
    }
    
    size_t poolBytes() {
        return pool.bytes();
    }
    
    void step();
    
    Node*& getRoot();
    
    char set(int x, int y, int k, Node* node);
    
    char at(int x, int y, Node* node) const;
};

#endif /* HashLife_hpp */
