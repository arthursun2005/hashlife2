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

std::unordered_map<Node*, Node*>* Node::hashMap;
