//
//  StackUnitTest.cpp
//  BTree
//
//  Created by Richard Szeto on 8/16/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include "StackUnitTest.hpp"

TEST(Stack, Push) {
    Stack<int> stack;
    EXPECT_EQ(stack.getCount(), 0);
    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.getCount(), 3);
}

TEST(Stack, Top) {
    Stack<int> stack;
    EXPECT_EQ(stack.getCount(), 0);
    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.getCount(), 3);
    stack.pop();
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.pop();
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
}

TEST(Stack, Pop) {
    Stack<int> stack;
    EXPECT_EQ(stack.getCount(), 0);
    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.getCount(), 3);
    stack.pop();
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.pop();
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
}

TEST(Stack, GetCount) {
    Stack<int> stack;
    EXPECT_EQ(stack.getCount(), 0);
    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_EQ(stack.getCount(), 3);
    stack.pop();
    EXPECT_EQ(stack.top(), 2);
    EXPECT_EQ(stack.getCount(), 2);
    stack.pop();
    EXPECT_EQ(stack.top(), 1);
    EXPECT_EQ(stack.getCount(), 1);
}

TEST(Stack, IsEmpty) {
    Stack<int> stack;
    EXPECT_TRUE(stack.isEmpty());
    stack.push(1);
    EXPECT_EQ(stack.top(), 1);
    EXPECT_FALSE(stack.isEmpty());
    stack.push(2);
    EXPECT_EQ(stack.top(), 2);
    EXPECT_FALSE(stack.isEmpty());
    stack.push(3);
    EXPECT_EQ(stack.top(), 3);
    EXPECT_FALSE(stack.isEmpty());
    stack.pop();
    EXPECT_EQ(stack.top(), 2);
    EXPECT_FALSE(stack.isEmpty());
    stack.pop();
    EXPECT_EQ(stack.top(), 1);
    EXPECT_FALSE(stack.isEmpty());
    stack.pop();
    EXPECT_TRUE(stack.isEmpty());
}
