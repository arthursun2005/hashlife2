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

struct LifeRule
{
    uint16_t born;
    uint16_t sustain;
    
    LifeRule() {
        sustain = 0b1100;
        born = 0b1000;
    }
    
    uint8_t at(char alive, char sum) const
    {
        if(alive != 0)
            return (sustain&(1 << sum)) >> sum;
        
        return (born&(1 << sum)) >> sum;
    }
};

#endif /* LifeRule_h */
