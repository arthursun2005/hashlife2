//
//  Node.h
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#include "LifeRule.h"
#include <cstdlib>
#include <assert.h>
#include <functional>

class NodePool;

struct Node
{
    Node* nw;
    Node* ne;
    Node* sw;
    Node* se;
    
    int level;
    
    char data = 0;
    
    long population = 0;
    
    size_t hash;
    
    Node() {
    }
    
    Node(const Node* node) {
        population = node->population;
        data = node->data;
        level = node->level;
        hash = node->hash;
        
        if(!node->isLeaf()) {
            nw = new Node(node->nw);
            ne = new Node(node->ne);
            sw = new Node(node->sw);
            se = new Node(node->se);
        }
    }
    
    Node(int level) : level(level) {
        if(level > 1) {
            nw = new Node(level - 1);
            ne = new Node(level - 1);
            sw = new Node(level - 1);
            se = new Node(level - 1);
        }
    }
    
    Node(Node* nw, Node* ne, Node* sw, Node* se) {
        set(nw, ne, sw, se);
    }
    
    ~Node() {
    }
    
    void destory() {
        if(!isLeaf()) {
            nw->destory();
            delete nw;
            ne->destory();
            delete ne;
            sw->destory();
            delete sw;
            se->destory();
            delete se;
        }
    }
    
    inline char at(char i) const
    {
        return (data&(1 << i)) >> i;
    }
    
    void set(char position, char x)
    {
        char oldBit = at(position);
        
        data &= ~(1 << position);
        data |= (x << position);
        
        population += x - oldBit;
    }
    
    void clear() {
        data = 0;
        population = 0;
        if(!isLeaf()) {
            nw->clear();
            ne->clear();
            sw->clear();
            se->clear();
        }
    }
    
    char set(int x, int y, int k) {
        if(isLeaf()) {
            /**
             (-1, 0),  (0, 0)
             (-1, -1), (0, -1)
             */
            
            long pp = population;
            
            if(pp == 0) data = 0;
            
            set(y * 2 - x + 2, k);
            
            return population - pp;
        }
        
        int qwidth = 1 << (level - 2);
        
        if(population == 0) {
            nw->population = 0;
            ne->population = 0;
            sw->population = 0;
            se->population = 0;
        }
        
        char dp;
        
        if(x >= 0) {
            if(y >= 0) {
                dp = ne->set(x - qwidth, y - qwidth, k);
            }else{
                dp = se->set(x - qwidth, y + qwidth, k);
            }
        }else{
            if(y >= 0) {
                dp = nw->set(x + qwidth, y - qwidth, k);
            }else{
                dp = sw->set(x + qwidth, y + qwidth, k);
            }
        }
        
        population += dp;
        
        return k;
    }
    
    char at(int x, int y) const
    {
        if(population == 0)
            return 0;
        
        if(isLeaf()) {
            return at(y * 2 - x + 2);
        }else{
            int qwidth = 1 << (level - 2);
            
            if(x >= 0) {
                if(y >= 0) {
                    return ne->at(x - qwidth, y - qwidth);
                }else{
                    return se->at(x - qwidth, y + qwidth);
                }
            }else{
                if(y >= 0) {
                    return nw->at(x + qwidth, y - qwidth);
                }else{
                    return sw->at(x + qwidth, y + qwidth);
                }
            }
        }
    }
    
    inline void print() {
        int width = 1 << level;
        int hwidth = 1 << (level - 1);
        for(int y = width - 1; y >= 0; --y) {
            for(int x = 0; x < width; ++x) {
                if(x == hwidth && y == hwidth)
                    printf("r");
                else printf("%c", at(x - hwidth, y - hwidth) * 3 + ' ');
            }
            printf("\n");
        }
    }
    
    void set(const Node* node)
    {
        data = node->data;
        population = node->population;
        hash = node->hash;
        
        if(!isLeaf()) {
            nw->set(node->nw);
            ne->set(node->ne);
            sw->set(node->sw);
            se->set(node->se);
        }
    }
    
    inline void updatePopulation()
    {
        if(!isLeaf()) {
            population = nw->population + ne->population + sw->population + se->population;
        }
    }
    
    void set(Node* _nw, Node* _ne, Node* _sw, Node* _se)
    {
        nw = _nw;
        ne = _ne;
        sw = _sw;
        se = _se;
        
        level = _nw->level + 1;
        updatePopulation();
        
        computeHash();
    }
    
    inline bool isLeaf() const
    {
        return level == 1;
    }
    
    void computeHash() {
        if(isLeaf()) {
            hash = data;
            return;
        }
        
        hash = (11 * nw->hash) + (13 * ne->hash) + (31 * sw->hash) + (se->hash * 101);
    }
    
    void computeAllHashs() {
        if(population == 0) {
            hash = 0;
            return;
        }
        
        if(!isLeaf()) {
            nw->computeAllHashs();
            ne->computeAllHashs();
            sw->computeAllHashs();
            se->computeAllHashs();
        }
        
        computeHash();
    }
    
    void assign_cnt(Node* n) {
        assert(level == n->level + 1);
        
        if(n->isLeaf()) {
            
            nw->set(0, n->at(3));
            
            ne->set(1, n->at(2));
            
            sw->set(2, n->at(1));
            
            se->set(3, n->at(0));
            
            updatePopulation();;
            
        }else{
            
            nw->se = n->nw;
            
            ne->sw = n->ne;
            
            sw->ne = n->sw;
            
            se->nw = n->se;
            
            nw->updatePopulation();
            ne->updatePopulation();
            sw->updatePopulation();
            se->updatePopulation();
            
            updatePopulation();
        }
    }
    
    Node* apply(const LifeRule& rule, NodePool* pool);
};

template <>
struct std::hash<Node*>
{
    inline size_t operator () (const Node* node) const
    {
        return node->hash;
    }
};

template <>
struct std::equal_to<Node*>
{
    bool operator () (const Node* a, const Node* b) const
    {
        if(a->level != b->level)
            return false;
        
        if(a->population != b->population)
            return false;
        
        if(a->isLeaf())
            return a->data == b->data;
        
        return operator() (a->nw, b->nw) && operator() (a->ne, b->ne) && operator() (a->sw, b->sw) && operator() (a->se, b->se);
    }
};
