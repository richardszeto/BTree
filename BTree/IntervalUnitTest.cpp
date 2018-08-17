//
//  IntervalUnitTest.cpp
//  BTree
//
//  Created by Richard Szeto on 8/16/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include "IntervalUnitTest.hpp"

TEST(Interval, DefaultConstructor) {
    Interval interval;
    EXPECT_EQ(interval.lowerBound(), 0);
    EXPECT_EQ(interval.upperBound(), 0);
}

TEST(Interval, ParameterizedConstructor) {
    Interval interval(1, 5);
    EXPECT_EQ(interval.lowerBound(), 1);
    EXPECT_EQ(interval.upperBound(), 5);
    
    Interval interval2(-10, -20);
    EXPECT_EQ(interval2.lowerBound(), -10);
    EXPECT_EQ(interval2.upperBound(), -20);
}

TEST(Interval, CopyConstructor) {
    Interval interval(1, 6);
    EXPECT_EQ(interval.lowerBound(), 1);
    EXPECT_EQ(interval.upperBound(), 6);
    
    Interval copy(interval);
    EXPECT_EQ(copy.lowerBound(), 1);
    EXPECT_EQ(copy.upperBound(), 6);
}

TEST(Interval, Getters) {
    Interval interval(1, 5);
    EXPECT_EQ(interval.lowerBound(), 1);
    EXPECT_EQ(interval.upperBound(), 5);
    
    Interval interval2(-10, -20);
    EXPECT_EQ(interval2.lowerBound(), -10);
    EXPECT_EQ(interval2.upperBound(), -20);
}
