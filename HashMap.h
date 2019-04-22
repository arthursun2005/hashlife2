//
//  HashMap.h
//  HashLife2
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef HashMap_h
#define HashMap_h

#include <vector>
#include <array>

template <class Key, class Value, class _Hasher = std::hash<Key>, class _Pred = std::equal_to<Key>, size_t N = 0x1000000>
class HashMap
{
    
protected:
    
    _Hasher hasher;
    
    _Pred judge;
    
    using Pair = std::pair<Key, Value>;
    
    using List = std::vector<Pair>;
    
    std::array<List, N> table;
    
public:
    
    size_t count = 0;
    
    HashMap() {
    }
    
    ~HashMap() {
    }
    
    bool erase(const Key& key) {
        size_t position = hasher(key)%N;
        
        List& k = table[position];
        
        for (auto it = k.begin(); it != k.end(); ++it) {
            if(judge(key, it->first)) {
                --count;
                k.erase(it);
                return true;
            }
        }
        
        return false;
    }
    
    bool insert(const Pair& pair) {
        size_t position = hasher(pair.first)%N;
        
        List& k = table[position];
        
        for(Pair& x : k) {
            if(judge(pair.first, x.first)) {
                return true;
            }
        }
        
        k.push_back(pair);
        
        ++count;
        
        return false;
    }
    
    bool find(const Key& key, Value* value) {
        size_t position = hasher(key)%N;
        List& k = table[position];
        
        for(Pair& x : k) {
            if(judge(key, x.first)) {
                *value = x.second;
                return true;
            }
        }
        
        return false;
    }
    
};


#endif /* HashMap_h */
