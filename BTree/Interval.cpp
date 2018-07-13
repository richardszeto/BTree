//
//  Interval.cpp
//  BTree
//
//  Created by Richard Szeto on 7/11/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

typedef long long LONG;

#include "Interval.hpp"

Interval::Interval() : lowerBound_(0), upperBound_(0)
{
    // intentionally left empty
}

Interval::Interval(LONG newLowerBound, LONG newUpperBound) : lowerBound_(newLowerBound), upperBound_(newUpperBound)
{
    // intentionally left empty
}

Interval::Interval(const Interval& other)
{
    this->lowerBound_ = other.lowerBound();
    this->upperBound_ = other.upperBound();
}

const LONG Interval::lowerBound() const
{
    return lowerBound_;
}

const LONG Interval::upperBound() const
{
    return upperBound_;
}
