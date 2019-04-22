//
//  NodePool.cpp
//  HashLife2
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "NodePool.hpp"

Node *NodePool::alloc(const Node *node) {
    Node* _n = getFull(node->level);
    
    if(_n != NULL) {
        _n->set(node);
        return _n;
    }
    
    Node* n = new Node(node);
    
    addFull(n);
    
    return n;
}

Node *NodePool::alloc(int level) {
    Node* _n = getFull(level);
    
    if(_n != NULL) {
        _n->population = 0;
        _n->data = 0;
        if(!_n->isLeaf()) {
            _n->nw->population = 0;
            _n->ne->population = 0;
            _n->sw->population = 0;
            _n->se->population = 0;
        }
        return _n;
    }
    
    Node* n = new Node(level);
    
    addFull(n);
    
    return n;
}

void NodePool::addFull(Node *node) {
    if(!node->isLeaf()) {
        addFull(node->nw);
        addFull(node->ne);
        addFull(node->sw);
        addFull(node->se);
    }
    nodes.push(node);
}

Node* NodePool::getFull(int level) {
    int offsetLevel = level - 1;
    
    if(fullNodes.size() > offsetLevel) {
        if(!fullNodes[offsetLevel].empty()) {
            Node* n = fullNodes[offsetLevel].top();
            fullNodes[offsetLevel].pop();
            return n;
        }
        
        if(offsetLevel > 0 && fullNodes[offsetLevel - 1].size() > 3) {
            Node* nw = fullNodes[offsetLevel - 1].top();
            fullNodes[offsetLevel - 1].pop();
            
            Node* ne = fullNodes[offsetLevel - 1].top();
            fullNodes[offsetLevel - 1].pop();
            
            Node* sw = fullNodes[offsetLevel - 1].top();
            fullNodes[offsetLevel - 1].pop();
            
            Node* se = fullNodes[offsetLevel - 1].top();
            fullNodes[offsetLevel - 1].pop();
            
            Node* n = alloc(nw, ne, sw, se);
            
            return n;
        }
        
        if(fullNodes.size() > offsetLevel + 1 && !fullNodes[offsetLevel + 1].empty()) {
            Node* p = fullNodes[offsetLevel + 1].top();
            fullNodes[offsetLevel + 1].pop();
            
            fullNodes[offsetLevel].push(p->ne);
            fullNodes[offsetLevel].push(p->sw);
            fullNodes[offsetLevel].push(p->se);
            
            references.push(p);
            
            return p->nw;
        }
    }
    return NULL;
}

Node *NodePool::alloc(Node *nw, Node *ne, Node *sw, Node *se) {
    if(!references.empty()) {
        Node* n = references.top();
        references.pop();
        
        n->set(nw, ne, sw, se);
        
        return n;
    }
    
    Node* n = new Node(nw, ne, sw, se);
    
    nodes.push(n);
    
    return n;
}

void NodePool::destory(Node *node) {
    int offsetLevel = node->level - 1;
    
    if(offsetLevel + 1 > fullNodes.size())
        fullNodes.resize(offsetLevel + 1);
    
    fullNodes[offsetLevel].push(node);
}


void NodePool::free(Node *node) {
    references.push(node);
}


NodePool::~NodePool() {
    while(!nodes.empty()) {
        delete (nodes.top());
        nodes.pop();
    }
}
