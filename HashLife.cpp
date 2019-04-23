//
//  HashLife.cpp
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "HashLife.hpp"

void HashLife::step()
{
    while (root->level < 4 ||
           root->nw->population != root->nw->se->se->population ||
           root->ne->population != root->ne->sw->sw->population ||
           root->sw->population != root->sw->ne->ne->population ||
           root->se->population != root->se->nw->nw->population)
        root = root->expand();
    
    root = root->step(rule);
    
    generations += 1L << (root->level - 2);
}

void HashLife::set(int x, int y, int k) {
    root = root->set(x, y, k);
}

char HashLife::at(int x, int y) const
{
    return root->at(x, y);
}

Node* Node::apply(const LifeRule& rule) {
    assert(level == 2);
        
    /**
     3  2  3  2
     1  0  1  0
     3  2  3  2
     1  0  1  0
     */
    
    char sum0 = nw->at(3) + nw->at(2) + ne->at(3) + nw->at(1) + ne->at(1) + sw->at(3) + sw->at(2) + se->at(3);
    
    char sum1 = nw->at(2) + ne->at(3) + ne->at(2) + nw->at(0) + ne->at(0) + sw->at(2) + se->at(3) + se->at(2);
    
    char sum2 = nw->at(1) + nw->at(0) + ne->at(1) + sw->at(3) + se->at(3) + sw->at(1) + sw->at(0) + se->at(1);
    
    char sum3 = nw->at(0) + ne->at(1) + ne->at(0) + sw->at(2) + se->at(2) + sw->at(0) + se->at(1) + se->at(0);
    
    int _nw = rule.at(nw->at(0), sum0);
    
    int _ne = rule.at(ne->at(1), sum1);
    
    int _sw = rule.at(sw->at(2), sum2);
    
    int _se = rule.at(se->at(3), sum3);

    return create(_nw, _ne, _sw, _se);
}

Node*& HashLife::getRoot() {
    return root;
}
