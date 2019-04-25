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
    
    generations += pow(2.0f, (root->level - 2));
}

void HashLife::set(long long x, long long y, int k) {
    while(root->outside(x, y)) {
        root = root->expand();
    }
    root = root->set(x, y, k);
}

char HashLife::at(long long x, long long y) const
{
    return root->at(x, y);
}

Node*& HashLife::getRoot() {
    return root;
}
