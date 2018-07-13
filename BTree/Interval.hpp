//
//  Interval.hpp
//  BTree
//
//  Created by Richard Szeto on 7/11/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#ifndef Interval_hpp
#define Interval_hpp

#include <stdio.h>

typedef long long LONG;

class Interval
{
private:
    LONG lowerBound_;
    LONG upperBound_;
public:
    Interval();
    Interval(LONG newLowerBound, LONG newUpperBound);
    Interval(const Interval& other);
    const LONG lowerBound() const;
    const LONG upperBound() const;
};

#endif /* Interval_hpp */
