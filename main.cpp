//
//  main.cpp
//  BTree
//
//  Created by Richard Szeto on 7/4/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include <iostream>

#include "BTree/BTree.cpp"
#include "Queue/Queue.cpp"
#include "Stack/Stack.cpp"

typedef long long LONG;

int main(int argc, const char * argv[])
{
    // need to create test cases for each scenario
    for (int i = 2; i <= 1000; i++)
    {
        std::cout << i << std::endl;
        
        BTree<LONG, LONG> bTree(i);
        
        for (LONG j = 0; j <= i * 100; j++)
        {
            bTree.insert(j, j);
            
            LONG outValue = 0;
            
            if (!bTree.find(j, outValue))
            {
                std::cout << "key: " << j << std::endl;
            }
        }
        
        for (LONG j = i * 100; j >= 0; j--)
        {
            LONG outValue = 0;
            
            if (!bTree.remove(j, outValue))
            {
                std::cout << "key: " << j << std::endl;
            }
        }
    }
    
    return 0;
}
