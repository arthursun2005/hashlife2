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

Node*& HashLife::getRoot() {
    return root;
}
