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
#include <unordered_map>

struct Node;

template <>
struct std::hash<Node*>
{
    size_t operator () (const Node* node) const;
};

template <>
struct std::equal_to<Node*>
{
    bool operator () (const Node* a, const Node* b) const;
};

struct Node
{
    static std::unordered_map<Node*, Node*>* hashMap;
    
    Node* nw;
    Node* ne;
    Node* sw;
    Node* se;
    
    Node* result = NULL;
    
    const int level;
    
    const char data;
    
    const long population;
    
    const size_t hash;
    
    static void init() {
        hashMap = new std::unordered_map<Node*, Node*>();
    }
    
    static void free() {
        for(auto& k : *hashMap) {
            delete k.second;
        }
        delete hashMap;
    }
    
    Node(char nw, char ne, char sw, char se)  : level(1), population(nw + ne + sw + se), data((nw << 3) | (ne << 2) | (sw << 1) | (se << 0)), nw(NULL), ne(NULL), sw(NULL), se(NULL), hash(data) {
    }
    
    Node(int level) : level(level), population(0), data(0), hash(0) {
        if(!isLeaf()) {
            Node* empty = create(level - 1);
            
            nw = empty;
            ne = empty;
            sw = empty;
            se = empty;
        }else{
            nw = ne = sw = se = NULL;
        }
    }
    
    Node(Node* nw, Node* ne, Node* sw, Node* se)  : level(nw->level + 1), population(nw->population + ne->population + sw->population + se->population), data(0), nw(nw), ne(ne), sw(sw), se(se), hash(nw->hash + ne->hash * 11 + sw->hash * 511 + se->hash * 10007) {
    }
    
    ~Node() {
    }
    
    Node* find() {
        auto it = hashMap->find(this);
        if(it != hashMap->end()) {
            delete this;
            return it->second;
        }
        (*hashMap)[this] = this;
        return this;
    }
    
    inline static Node* create(int level) {
        return (new Node(level))->find();
    }
    
    inline static Node* create(char nw, char ne, char sw, char se) {
        return (new Node(nw, ne, sw, se))->find();
    }
    
    inline static Node* create(Node* nw, Node* ne, Node* sw, Node* se) {
        return (new Node(nw, ne, sw, se))->find();
    }
    
    inline char at(char i) const
    {
        return (data&(1 << i)) >> i;
    }
    
    Node* set(int x, int y, int k) const
    {
        if(isLeaf()) {
            char q[] = {at(3), at(2), at(1), at(0)};
            q[-2 * y + x + 1] = k;
            
            return create(q[0], q[1], q[2], q[3]);
        }
        
        int qwidth = 1 << (level - 2);
        Node* q[] = {nw, ne, sw, se};
        
        if(x >= 0) {
            if(y >= 0) {
                q[1] = ne->set(x - qwidth, y - qwidth, k);
            }else{
                q[3] = se->set(x - qwidth, y + qwidth, k);
            }
        }else{
            if(y >= 0) {
                q[0] = nw->set(x + qwidth, y - qwidth, k);
            }else{
                q[2] = sw->set(x + qwidth, y + qwidth, k);
            }
        }
        
        return create(q[0], q[1], q[2], q[3]);
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
    
    inline void print() const
    {
        int width = 1 << level;
        int hwidth = 1 << (level - 1);
        for(int y = width - 1; y >= 0; --y) {
            for(int x = 0; x < width; ++x) {
                printf("%c", at(x - hwidth, y - hwidth) * 3 + ' ');
            }
            printf("\n");
        }
    }
    
    inline bool isLeaf() const
    {
        return level == 1;
    }
    
    Node* step(const LifeRule& rule)
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
    
    Node* expand() {
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
    
    Node* apply(const LifeRule& rule);
};
