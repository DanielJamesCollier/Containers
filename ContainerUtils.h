//
//  Algorithm.h
//  throwaway
//
//  Created by Daniel Collier on 16/01/2017.
//  Copyright © 2017 Daniel Collier. All rights reserved.
//

#ifndef ContainerUtils_h
#define ContainerUtils_h

#include "StaticArray.h"

namespace nova {

    // primary sort
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> inline void
    sort();
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> inline void
    sort(StaticArray<TYPE, SIZE> & array) {
        auto temp{0};
        
        for (int k = 0; k < array.size(); k++)
        {
            for (int i = 0; i < array.size() - 1; i++){
                
                if (array[i] > array[i + 1])
                {
                    temp = array[i];
                    array[i] = array[i + 1];
                    array[i + 1] = temp;
                }
            }
        }
    }

    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> inline void
    sort(StaticFlatMap<TYPE, SIZE> const & sfm) {
        
    }
    
    // primary print
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> inline void
    print();
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> inline void
    print(StaticArray<TYPE, SIZE> array) {
        for(auto & i : array) {
            std::cout << i << std::endl;
        }
    }
    
    //-----------------------------------------------------------------
    template<typename TYPE, size_t SIZE> inline void
    print(StaticFlatMap<TYPE, SIZE> array) {
    
    }

    
} // namespace nova
#endif /* ContainerUtils_h */
