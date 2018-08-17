//
//  main.cpp
//  BTree
//
//  Created by Richard Szeto on 7/4/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include <iostream>
#include "gtest/gtest.h"

// BTree is a template, so implementation needs to be included
#include "./BTree/BTree.cpp"


typedef long long LONG;

using namespace std;

int main(int argc, const char * argv[])
{
    testing::InitGoogleTest(&argc, (char **)argv);
    int result = RUN_ALL_TESTS();
    
    BTree<int, int> bt(1000);
    clock_t insertionStart;
    double insertionDuration;
    
    insertionStart = clock();
    for (int i = 0; i < 1000000; ++i) {
        bt.insert(i, i);
    }
    insertionDuration = (clock() - insertionStart) / (double)CLOCKS_PER_SEC;
    cout << "Insert 1,000,000 elements in ascending order." << endl;
    cout << "insertionDuration: " << insertionDuration << "." << endl;
    
    int outValue;
    clock_t findStart;
    double findDuration;
    
    findStart = clock();
    bt.find(999999, outValue);
    findDuration = (clock() - findStart) / (double)CLOCKS_PER_SEC;
    cout << "Find 1,000,000th element." << endl;
    cout << "findDuration: " << findDuration << "." << endl;
    
    return result;
}
