//
//  Stack.cpp
//  BTree
//
//  Created by Richard Szeto on 6/27/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include "Stack.hpp"

template <typename T>
Stack<T>::~Stack()
{
    while (!isEmpty())
    {
        pop();
    }
}

template <typename T>
void Stack<T>::push(const T &newData)
{
    root_ = new node { newData, root_ };
    elements_++;
}

template <typename T>
T& Stack<T>::top()
{
    return root_->node_data;
}

template<typename T>
void Stack<T>::pop()
{
    node* nodeToDelete = root_;
    root_ = root_->next;
    delete nodeToDelete;
    elements_--;
}

template <typename T>
int Stack<T>::getCount() const
{
    return elements_;
}

template <typename T>
bool Stack<T>::isEmpty() const
{
    return elements_ <= 0;
}
