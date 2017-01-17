//
//  AssertMacros.h
//  throwaway
//
//  Created by Daniel Collier on 17/01/2017.
//  Copyright © 2017 Daniel Collier. All rights reserved.
//

#ifndef AssertMacros_h
#define AssertMacros_h

#include <cassert>
#include <iostream>

#define assert_on_range(INDEX, MAX) { \
    if(INDEX > MAX) { \
        std::cout << "-----------------------------------------\n"; \
        std::cout << "ASSERTION FAILED : [index: " << INDEX << "] > " << "[max: " << MAX << "]" <<std::endl; \
        assert(0); \
    } \
} \


#endif /* AssertMacros_h */
