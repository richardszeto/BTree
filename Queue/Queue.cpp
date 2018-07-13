//
//  Queue.cpp
//  BTree
//
//  Created by Richard Szeto on 6/27/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#include "Queue.hpp"

template <typename T>
Queue<T>::~Queue()
{
    while (!isEmpty())
    {
        dequeue();
    }
}

template <typename T>
void Queue<T>::enqueue(const T &newData)
{
    if (head_ == nullptr)
    {
        tail_ = head_ = new node { newData, nullptr };
    }
    else
    {
        tail_ = tail_->next = new node { newData, nullptr };
    }
    
    elements_++;
}

template <typename T>
T& Queue<T>::top()
{
    return head_->node_data;
}

template <typename T>
void Queue<T>::dequeue()
{
    if (elements_ == 1)
    {
        delete head_;
        tail_ = head_ = nullptr;
    }
    else
    {
        node* nodeToDelete = head_;
        head_ = head_->next;
        delete nodeToDelete;
    }
    
    elements_--;
}

template <typename T>
int Queue<T>::getCount() const
{
    return elements_;
}

template <typename T>
bool Queue<T>::isEmpty() const
{
    return elements_ <= 0;
}
