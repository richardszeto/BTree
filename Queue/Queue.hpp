//
//  Queue.hpp
//  BTree
//
//  Created by Richard Szeto on 6/27/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#ifndef Queue_hpp
#define Queue_hpp

#include <iostream>

template <typename T>
class Queue
{
private:
    struct node
    {
        T node_data;
        node* next;
    };
    node* head_;
    node* tail_;
    int elements_;
    
public:
    Queue() : head_(nullptr), tail_(nullptr), elements_(0) {}
    virtual ~Queue();
    void enqueue(const T &newData);
    T& top();
    void dequeue();
    int getCount() const;
    bool isEmpty() const;
};

#endif /* Queue_hpp */
