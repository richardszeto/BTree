//
//  QueueUnitTest.cpp
//  BTree
//
//  Created by Richard Szeto on 8/16/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include "QueueUnitTest.hpp"

TEST(Queue, Enqueue) {
    Queue<int> queue;
    EXPECT_EQ(queue.getCount(), 0);
    queue.enqueue(1);
    EXPECT_EQ(queue.getCount(), 1);
    queue.enqueue(2);
    EXPECT_EQ(queue.getCount(), 2);
    queue.enqueue(3);
    EXPECT_EQ(queue.getCount(), 3);
    EXPECT_EQ(queue.top(), 1);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 2);
    EXPECT_EQ(queue.top(), 2);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 1);
    EXPECT_EQ(queue.top(), 3);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 0);
}

TEST(Queue, Top) {
    Queue<int> queue;
    EXPECT_EQ(queue.getCount(), 0);
    queue.enqueue(1);
    EXPECT_EQ(queue.getCount(), 1);
    queue.enqueue(2);
    EXPECT_EQ(queue.getCount(), 2);
    queue.enqueue(3);
    EXPECT_EQ(queue.getCount(), 3);
    EXPECT_EQ(queue.top(), 1);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 2);
    EXPECT_EQ(queue.top(), 2);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 1);
    EXPECT_EQ(queue.top(), 3);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 0);
}

TEST(Queue, Dequeue) {
    Queue<int> queue;
    EXPECT_EQ(queue.getCount(), 0);
    queue.enqueue(1);
    EXPECT_EQ(queue.getCount(), 1);
    queue.enqueue(2);
    EXPECT_EQ(queue.getCount(), 2);
    queue.enqueue(3);
    EXPECT_EQ(queue.getCount(), 3);
    EXPECT_EQ(queue.top(), 1);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 2);
    EXPECT_EQ(queue.top(), 2);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 1);
    EXPECT_EQ(queue.top(), 3);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 0);
}

TEST(Queue, GetCount) {
    Queue<int> queue;
    EXPECT_EQ(queue.getCount(), 0);
    queue.enqueue(1);
    EXPECT_EQ(queue.getCount(), 1);
    queue.enqueue(2);
    EXPECT_EQ(queue.getCount(), 2);
    queue.enqueue(3);
    EXPECT_EQ(queue.getCount(), 3);
    EXPECT_EQ(queue.top(), 1);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 2);
    EXPECT_EQ(queue.top(), 2);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 1);
    EXPECT_EQ(queue.top(), 3);
    queue.dequeue();
    EXPECT_EQ(queue.getCount(), 0);
}

TEST(Queue, IsEmpty) {
    Queue<int> queue;
    EXPECT_TRUE(queue.isEmpty());
    queue.enqueue(1);
    EXPECT_FALSE(queue.isEmpty());
    queue.enqueue(2);
    EXPECT_FALSE(queue.isEmpty());
    queue.enqueue(3);
    EXPECT_FALSE(queue.isEmpty());
    EXPECT_EQ(queue.top(), 1);
    queue.dequeue();
    EXPECT_FALSE(queue.isEmpty());
    EXPECT_EQ(queue.top(), 2);
    queue.dequeue();
    EXPECT_FALSE(queue.isEmpty());
    EXPECT_EQ(queue.top(), 3);
    queue.dequeue();
    EXPECT_TRUE(queue.isEmpty());
}
