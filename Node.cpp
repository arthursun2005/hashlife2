//
//  Node.cpp
//  HashLife2
//
//  Created by Arthur Sun on 4/23/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "Node.h"

size_t std::hash<Node*>::operator () (const Node* node) const
{
    return node->hash;
}

bool std::equal_to<Node*>::operator () (const Node* a, const Node* b) const
{
    if(a->level != b->level)
        return false;
    
    if(a->population != b->population)
        return false;
    
    if(a->isLeaf())
        return a->data == b->data;
    
    return a->nw == b->nw && a->ne == b->ne && a->sw == b->sw && a->se == b->se;
}

Node* Node::step(const LifeRule& rule)
{
    if(result != NULL)
        return result;
    
    if(population == 0)
        return result = nw;
    
    if(level == 2) {
        return result = apply(rule);
    }
    
    Node* n00 = nw->step(rule);
    Node* n01 = create(nw->ne, ne->nw, nw->se, ne->sw)->step(rule);
    Node* n02 = ne->step(rule);
    Node* n10 = create(nw->sw, nw->se, sw->nw, sw->ne)->step(rule);
    Node* n11 = create(nw->se, ne->sw, sw->ne, se->nw)->step(rule);
    Node* n12 = create(ne->sw, ne->se, se->nw, se->ne)->step(rule);
    Node* n20 = sw->step(rule);
    Node* n21 = create(sw->ne, se->nw, sw->se, se->sw)->step(rule);
    Node* n22 = se->step(rule);
    
    return result = create(
       create(n00, n01, n10, n11)->step(rule),
       create(n01, n02, n11, n12)->step(rule),
       create(n10, n11, n20, n21)->step(rule),
       create(n11, n12, n21, n22)->step(rule)
    );
}

Node* Node::expand() {
    if(isLeaf()) {
        return create(
          create(0, 0, 0, at(3)),
          create(0, 0, at(2), 0),
          create(0, at(1), 0, 0),
          create(at(0), 0, 0, 0)
        );
    }
    
    Node* empty = Node::create(level - 1);
    
    Node* n0 = create(empty, empty, empty, nw);
    
    Node* n1 = create(empty, empty, ne, empty);
    
    Node* n2 = create(empty, sw, empty, empty);
    
    Node* n3 = create(se, empty, empty, empty);
    
    return create(n0, n1, n2, n3);
}

Node* Node::apply(const LifeRule& rule) {    
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

std::unordered_set<Node*>* Node::hashSet = NULL;
