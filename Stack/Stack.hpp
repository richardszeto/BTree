//
//  Stack.hpp
//  BTree
//
//  Created by Richard Szeto on 6/27/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <iostream>

template <typename T>
class Stack
{
public:
    Stack() : elements_(0), root_(nullptr) {}
    virtual ~Stack();
    void push(const T &newData);
    T& top();
    void pop();
    int getCount() const;
    bool isEmpty() const;
    
private:
    struct node
    {
        T node_data;
        node* next;
    };
    int elements_;
    node* root_;
};

#endif /* Stack_hpp */
