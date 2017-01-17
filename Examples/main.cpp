//
//  main.cpp
//  throwaway
//
//  Created by Daniel Collier on 16/12/2016.
//  Copyright © 2016 Daniel Collier. All rights reserved.
//

#include <iostream>
#include "StaticArray.h"
#include "StaticFlatMap.h"
#include "ContainerUtils.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    // static array test
    //-----------------------------------------------------------------
    std::cout << "[Static Array]" << std::endl;
    nova::StaticArray<int, 100> dave;
    
    dave.fill(0);
    
    int o = 200;
    for(auto & i : dave) {
        i = o--;
    }

    nova::sort(dave);
    
    nova::print(dave);
    
    // static flat map test
    //-----------------------------------------------------------------
    std::cout << "[Static Flat Map]" << std::endl;
    nova::StaticFlatMap<int, 40> map;
    
    map.addObject(10);
    
    // loop over object pool
    for(auto i : map) {
        std::cout << i << std::endl;
    }
    
    // loop over static map
    for(auto i = map.beginStatic(); i != map.endStatic(); i++) {
        int object;
        if(nova::MapError::OKAY == map.getObject(i, object)) {
            std::cout << object << std::endl;
        }
    }    
   
    return 0;
}
