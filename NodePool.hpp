//
//  NodePool.hpp
//  HashLife2
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef NodePool_hpp
#define NodePool_hpp

#include "Node.h"
#include <vector>
#include <stack>

class NodePool
{
    
protected:
    
    std::vector< std::stack<Node*> > fullNodes;
    
    std::stack<Node*> references;
    
    std::stack<Node*> nodes;
    
    void addFull(Node* n);
    
    Node* getFull(int level);
    
public:
    
    NodePool() {}
    
    ~NodePool();
    
    inline size_t bytes() const
    {
        return nodes.size() * sizeof(Node);
    }
    
    Node* alloc(const Node* node);
    
    Node* alloc(int level);
    
    Node* alloc(Node*, Node*, Node*, Node*);
    
    void destory(Node* node);
    
    void free(Node* node);
};

#endif /* NodePool_hpp */
