//
//  LifeRule.h
//  HashLife
//
//  Created by Arthur Sun on 4/19/19.
//  Copyright © 2019 Arthur Sun. All rights reserved.
//

#ifndef LifeRule_h
#define LifeRule_h

#include <cstdlib>
#include <string>

struct LifeRule
{
    uint16_t born;
    uint16_t sustain;
    
    LifeRule() {
        sustain = 0b1100;
        born = 0b1000;
    }
    
    void set(const char* str) {
        sustain = 0;
        born = 0;
        
        int n = (int)strlen(str);
        
        char editing = 'B';
        
        for (int i = 0; i < n; ++i) {
            char x = toupper(str[i]);
            int k = x - '0';
            
            if(x == 'B' || x == 'S')
                editing = x;
            
            if(k >= 0 && k <= 9) {
                if(editing == 'B') {
                    born |= 1 << k;
                }else if(editing == 'S') {
                    sustain |= 1 << k;
                }
            }
        }
    }
    
    uint8_t at(char alive, char sum) const
    {
        if(alive != 0) return (sustain&(1 << sum)) >> sum;
        
        return (born&(1 << sum)) >> sum;
    }
};

#endif /* LifeRule_h */
