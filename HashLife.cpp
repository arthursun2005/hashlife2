//
//  HashLife.cpp
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "HashLife.hpp"

Node *HashLife::solve(Node *node) {
    assert(!node->isLeaf());
    
    if(node->population == 0) {
        //return pool.alloc(node->level - 1);
        printf("%d\n", node->level);
    }
    
    /*
    Node* k;
    
    bool found = cache.find(node, &k);
    
    if(found) {
        if(node == root)
            printf("%ld\n", b - a);
        
        return pool.alloc(k);
    }
    */
    
    auto it = cache.find(node);
    if(it != cache.end()) {
        //return pool.alloc(it->second);
    }
    
    
    if(node->level == 2) {
        Node* n = node->apply(rule, &pool);
        
        cache.insert(std::pair<Node*, Node*>(pool.alloc(node), pool.alloc(n)));
        
        return n;
    }
    
    Node* cnt = pool.alloc(node->nw->se, node->ne->sw, node->sw->ne, node->se->nw);
    
    Node* h1 = pool.alloc(node->nw->ne, node->ne->nw, node->nw->se, node->ne->sw);
    Node* h2 = pool.alloc(node->sw->ne, node->se->nw, node->sw->se, node->se->sw);
    
    Node* v1 = pool.alloc(node->nw->sw, node->nw->se, node->sw->nw, node->sw->ne);
    Node* v2 = pool.alloc(node->ne->sw, node->ne->se, node->se->nw, node->se->ne);
    
    Node* n00 = solve(node->nw);
    Node* n01 = solve(h1);
    Node* n02 = solve(node->ne);
    Node* n10 = solve(v1);
    Node* n11 = solve(cnt);
    Node* n12 = solve(v2);
    Node* n20 = solve(node->sw);
    Node* n21 = solve(h2);
    Node* n22 = solve(node->se);
    
    Node* nw = pool.alloc(n00, n01, n10, n11);
    Node* ne = pool.alloc(n01, n02, n11, n12);
    
    Node* sw = pool.alloc(n10, n11, n20, n21);
    Node* se = pool.alloc(n11, n12, n21, n22);
    
    Node* n = pool.alloc(solve(nw), solve(ne), solve(sw), solve(se));
    
    cache.insert(std::pair<Node*, Node*>(pool.alloc(node), pool.alloc(n)));
    
    pool.free(nw);
    pool.free(ne);
    pool.free(sw);
    pool.free(se);
    
    pool.destory(n00);
    pool.destory(n01);
    pool.destory(n02);
    pool.destory(n10);
    pool.destory(n11);
    pool.destory(n12);
    pool.destory(n20);
    pool.destory(n21);
    pool.destory(n22);
    
    pool.free(cnt);
    pool.free(h1);
    pool.free(h2);
    pool.free(v1);
    pool.free(v2);
    
    return n;
}

void HashLife::step() {
    while (root->level < 4 ||
        root->nw->population != root->nw->se->se->population ||
        root->ne->population != root->ne->sw->sw->population ||
        root->sw->population != root->sw->ne->ne->population ||
        root->se->population != root->se->nw->nw->population)
        expandOnce();
    
    root->computeAllHashs();
    
    Node* n = solve(root);
    
    root->assign_cnt(n);
    
    pool.destory(n);
    
    generations += (1 << (root->level - 2));
}

char HashLife::set(int x, int y, int k, Node* node) {
    return node->set(x, y, k);
}

char HashLife::at(int x, int y, Node* node) const
{
    return node->at(x, y);
}

void HashLife::expandOnce() {
    int l = root->level - 1;
    
    Node* n0 = pool.alloc(pool.alloc(l), pool.alloc(l), pool.alloc(l), root->nw);
    
    Node* n1 = pool.alloc(pool.alloc(l), pool.alloc(l), root->ne, pool.alloc(l));
    
    Node* n2 = pool.alloc(pool.alloc(l), root->sw, pool.alloc(l), pool.alloc(l));
    
    Node* n3 = pool.alloc(root->se, pool.alloc(l), pool.alloc(l), pool.alloc(l));
    
    root->set(n0, n1, n2, n3);
}

Node* Node::apply(const LifeRule& rule, NodePool* pool) {
    assert(level == 2);
    
    Node* node = pool->alloc(1);
    
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
    
    node->set(3, rule.at(nw->at(0), sum0));
    
    node->set(2, rule.at(ne->at(1), sum1));
    
    node->set(1, rule.at(sw->at(2), sum2));
    
    node->set(0, rule.at(se->at(3), sum3));
    
    node->computeHash();
    
    return node;
}

Node*& HashLife::getRoot() {
    return root;
}
