//
//  BTree.cpp
//  BTree
//
//  Created by Richard Szeto on 7/4/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#ifndef BTree_cpp
#define BTree_cpp

#include "BTree.hpp"

#include "Interval.hpp"

// Stack and Queue are templates, so implementation needs to be included
#include "../Stack/Stack.cpp"
#include "../Queue/Queue.cpp"

#include <cmath>

template <typename T, typename K>
BTree<T, K>::TreeNode::TreeNode(LONG kvpMaxSize) : kvpSize_(0), kvpMaxSize_(3)
{
    if (kvpMaxSize > 3)
    {
        kvpMaxSize_ = kvpMaxSize;
    }
    
    kvp_ = new KeyValuePair* [this->kvpMaxSize()];
    children_ = new TreeNode* [childrenMaxSize()];
    
    for (LONG kvpIndex = 0; kvpIndex < this->kvpMaxSize(); kvpIndex++)
    {
        kvp_[kvpIndex] = nullptr;
    }
    
    for (LONG childrenIndex = 0; childrenIndex < childrenMaxSize(); childrenIndex++)
    {
        children_[childrenIndex] = nullptr;
    }
}

template <typename T, typename K>
BTree<T, K>::TreeNode::~TreeNode()
{
    for (LONG childrenIndex = 0; childrenIndex < childrenMaxSize(); childrenIndex++)
    {
        if (children_[childrenIndex] != nullptr)
        {
            delete children_[childrenIndex];
            children_[childrenIndex] = nullptr;
        }
    }
    
    delete[] children_;
    children_ = nullptr;
    
    for (LONG kvpIndex = 0; kvpIndex < kvpMaxSize(); kvpIndex++)
    {
        if (kvp_[kvpIndex] != nullptr)
        {
            delete kvp_[kvpIndex];
            kvp_[kvpIndex] = nullptr;
        }
    }
    
    delete[] kvp_;
    kvp_ = nullptr;
    
    kvpSize_ = 0;
}

template <typename T, typename K>
typename BTree<T, K>::TreeNode* BTree<T, K>::TreeNode::left(LONG kvpIndex)
{
    if (kvpIndex < 0 || kvpIndex > kvpSize() - 1)
    {
        return nullptr;
    }
    
    if (kvp_[kvpIndex] == nullptr)
    {
        return nullptr;
    }
    
    return children_[kvpIndex];
}

template <typename T, typename K>
typename BTree<T, K>::TreeNode* BTree<T, K>::TreeNode::right(LONG kvpIndex)
{
    if (kvpIndex < 0 || kvpIndex > kvpSize() - 1)
    {
        return nullptr;
    }
    
    if (kvp_[kvpIndex] == nullptr)
    {
        return nullptr;
    }
    
    return children_[kvpIndex + 1];
}

template <typename T, typename K>
Interval BTree<T, K>::TreeNode::binarySearch(T key, LONG startIndex, LONG endIndex)
{
    if (startIndex > endIndex)
    {
        return Interval(endIndex, startIndex);
    }
    
    LONG offset = (LONG)ceil((endIndex - startIndex)/2.0);
    LONG middleIndex = startIndex + offset;
    
    if (kvp_[middleIndex] != nullptr)
    {
        if (key < kvp_[middleIndex]->key())
        {
            return binarySearch(key, startIndex, middleIndex - 1);
        }
        else if (key > kvp_[middleIndex]->key())
        {
            return binarySearch(key, middleIndex + 1, endIndex);
        }
        else
        {
            return Interval(middleIndex, middleIndex);
        }
    }
    else
    {
        return Interval(startIndex - 1, endIndex + 1);
    }
}

template <typename T, typename K>
const LONG BTree<T, K>::TreeNode::kvpMaxSize() const
{
    return kvpMaxSize_;
}

template <typename T, typename K>
const LONG BTree<T, K>::TreeNode::kvpSize() const
{
    return kvpSize_;
}

template <typename T, typename K>
const LONG BTree<T, K>::TreeNode::childrenMaxSize() const
{
    return kvpMaxSize() + 1;
}

template <typename T, typename K>
const LONG BTree<T, K>::TreeNode::childrenSize() const
{
    if (kvpSize() > 0)
    {
        return kvpSize() + 1;
    }
    
    return 0;
}

template <typename T, typename K>
const LONG BTree<T, K>::TreeNode::height() const
{
    LONG height = 1;
    
    Queue<TreeNode*> siblingQueue;
    
    for (LONG childrenIndex = 0; childrenIndex < this->childrenSize(); childrenIndex++)
    {
        if (this->children_[childrenIndex] != nullptr)
        {
            siblingQueue.enqueue(this->children_[childrenIndex]);
        }
    }
    
    while (!siblingQueue.isEmpty())
    {
        int queueSize = siblingQueue.getCount();
        
        while (queueSize > 0)
        {
            TreeNode* current = siblingQueue.top();
            siblingQueue.dequeue();
            
            for (LONG childrenIndex = 0; childrenIndex < current->childrenSize(); childrenIndex++)
            {
                if (current->children_[childrenIndex] != nullptr)
                {
                    siblingQueue.enqueue(current->children_[childrenIndex]);
                }
            }
            
            queueSize--;
        }
        
        height++;
    }
    
    return height;
}

template <typename T, typename K>
BTree<T, K>::~BTree()
{
    if (root_ != nullptr)
    {
        delete root_;
        root_ = nullptr;
    }
    
    count_ = 0;
}

template <typename T, typename K>
bool BTree<T, K>::findForInsert(const T &newKey, TreeNode *&current, Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack)
{
    while (true)
    {
        kvpIndexInterval = current->binarySearch(newKey, 0, current->kvpSize() - 1);
        
        if (kvpIndexInterval.lowerBound() == kvpIndexInterval.upperBound())
        {
            return false;
        }
        else
        {
            TreeNode *next = nullptr;
            
            if (kvpIndexInterval.lowerBound() < 0)
            {
                next = current->left(kvpIndexInterval.upperBound());
            }
            else
            {
                next = current->right(kvpIndexInterval.lowerBound());
            }
            
            if (next != nullptr)
            {
                ancestryStack.push(current);
                current = next;
            }
            else
            {
                break;
            }
        }
    }
    
    return true;
}

template <typename T, typename K>
void BTree<T, K>::balanceAfterInsert(TreeNode *&current, Stack<TreeNode*> &ancestryStack)
{
    while (current->kvpSize() >= current->kvpMaxSize())
    {
        LONG middleIndex = (LONG)ceil((current->kvpSize() - 1)/2.0);
        
        TreeNode* newNode = new TreeNode(kvpMaxSize_);
        
        for (LONG kvpIndex = current->kvpSize() - 1; kvpIndex > middleIndex; kvpIndex--)
        {
            newNode->kvp_[kvpIndex - middleIndex - 1] = current->kvp_[kvpIndex];
            current->kvp_[kvpIndex] = nullptr;
            newNode->kvpSize_++;
            current->kvpSize_--;
            
            newNode->children_[kvpIndex - middleIndex] = current->children_[kvpIndex + 1];
            current->children_[kvpIndex + 1] = nullptr;
        }
        
        newNode->children_[0] = current->children_[middleIndex + 1];
        current->children_[middleIndex + 1] = nullptr;
        
        KeyValuePair* kvpTemp = current->kvp_[middleIndex];
        current->kvp_[middleIndex] = nullptr;
        current->kvpSize_--;
        
        if (!ancestryStack.isEmpty())
        {
            TreeNode *parentOfCurrent = ancestryStack.top();
            ancestryStack.pop();
            
            Interval kvpParentIndexInterval = parentOfCurrent->binarySearch(kvpTemp->key(), 0, parentOfCurrent->kvpSize() - 1);
            
            for (LONG kvpIndex = parentOfCurrent->kvpSize(); kvpIndex > kvpParentIndexInterval.upperBound(); kvpIndex--)
            {
                parentOfCurrent->kvp_[kvpIndex] = parentOfCurrent->kvp_[kvpIndex - 1];
                parentOfCurrent->kvp_[kvpIndex - 1] = nullptr;
                
                parentOfCurrent->children_[kvpIndex + 1] = parentOfCurrent->children_[kvpIndex];
                parentOfCurrent->children_[kvpIndex] = nullptr;
            }
            
            parentOfCurrent->kvp_[kvpParentIndexInterval.upperBound()] = kvpTemp;
            kvpTemp = nullptr;
            parentOfCurrent->kvpSize_++;
            parentOfCurrent->children_[kvpParentIndexInterval.upperBound() + 1] = newNode;
            newNode = nullptr;
            
            current = parentOfCurrent;
        }
        else
        {
            TreeNode* newRoot = new TreeNode(kvpMaxSize_);
            newRoot->kvp_[0] = kvpTemp;
            kvpTemp = nullptr;
            newRoot->kvpSize_++;
            
            newRoot->children_[0] = root_;
            root_ = nullptr;
            newRoot->children_[1] = newNode;
            newNode = nullptr;
            root_ = newRoot;
            newRoot = nullptr;
            
            break;
        }
    }
}

template <typename T, typename K>
bool BTree<T, K>::insert(const T &newKey, const K &newValue)
{
    if (root_ == nullptr)
    {
        root_ = new TreeNode(kvpMaxSize_);
        root_->kvp_[0] = new KeyValuePair(newKey, newValue);
        root_->kvpSize_++;
        
        count_++;
        
        return true;
    }
    
    TreeNode *current = root_;
    
    Stack<TreeNode*> ancestryStack;
    
    Interval kvpIndexInterval;
    
    if (!findForInsert(newKey, current, kvpIndexInterval, ancestryStack))
    {
        return false;
    }
    
    // make room in node for insertion
    for (LONG kvpIndex = current->kvpSize(); kvpIndex > kvpIndexInterval.upperBound(); kvpIndex--)
    {
        current->kvp_[kvpIndex] = current->kvp_[kvpIndex - 1];
        current->kvp_[kvpIndex - 1] = nullptr;
        
        current->children_[kvpIndex + 1] = current->children_[kvpIndex];
        current->children_[kvpIndex] = nullptr;
    }
    
    // insert kvp in node
    current->kvp_[kvpIndexInterval.upperBound()] = new KeyValuePair(newKey, newValue);
    current->kvpSize_++;
    
    count_++;
    
    balanceAfterInsert(current, ancestryStack);
    
    return true;
}

template <typename T, typename K>
bool BTree<T, K>::replace(const T &key, const K &newValue)
{
    if (root_ == nullptr)
    {
        return false;
    }
    
    TreeNode *current = root_;
    
    while (current != nullptr)
    {
        Interval kvpIndexInterval = current->binarySearch(key, 0, current->kvpSize_ - 1);
        
        if (kvpIndexInterval.lowerBound() == kvpIndexInterval.upperBound())
        {
            current->kvp_[kvpIndexInterval.lowerBound()]->value_ = newValue;
            
            return true;
        }
        else
        {
            if (kvpIndexInterval.lowerBound() < 0)
            {
                current = current->left(kvpIndexInterval.upperBound());
            }
            else
            {
                current = current->right(kvpIndexInterval.lowerBound());
            }
        }
    }
    
    return false;
}

template <typename T, typename K>
void BTree<T, K>::borrowFromLeft(TreeNode *&current, TreeNode *&leftSibling, TreeNode *&parentOfCurrent, const Interval &kvpParentIndexInterval)
{
    // shift kvp right by 1 for current
    for (LONG kvpIndex = current->kvpSize(); kvpIndex > 0; kvpIndex--)
    {
        current->kvp_[kvpIndex] = current->kvp_[kvpIndex - 1];
        current->kvp_[kvpIndex - 1] = nullptr;
    }
    
    // shift children right by 1 for current
    if (current->childrenSize() <= 0)
    {
        current->children_[1] = current->children_[0];
        current->children_[0] = nullptr;
    }
    else
    {
        for (LONG childrenIndex = current->childrenSize(); childrenIndex > 0; childrenIndex--)
        {
            current->children_[childrenIndex] = current->children_[childrenIndex - 1];
            current->children_[childrenIndex - 1] = nullptr;
        }
    }
    
    // current left most kvp borrows parent shared with left sibling
    current->kvp_[0] = parentOfCurrent->kvp_[kvpParentIndexInterval.lowerBound()];
    current->kvpSize_++;
    
    // shared parent kvp borrows left sibling right most kvp
    parentOfCurrent->kvp_[kvpParentIndexInterval.lowerBound()] = leftSibling->kvp_[leftSibling->kvpSize() - 1];
    leftSibling->kvp_[leftSibling->kvpSize() - 1] = nullptr;
    
    // current left most child borrows left sibling right most child
    current->children_[0] = leftSibling->children_[leftSibling->childrenSize() - 1];
    leftSibling->children_[leftSibling->childrenSize() - 1] = nullptr;
    
    leftSibling->kvpSize_--;
}

template <typename T, typename K>
void BTree<T, K>::borrowFromRight(TreeNode *&current, TreeNode *&rightSibling, TreeNode *& parentOfCurrent, const Interval &kvpParentIndexInterval)
{
    // current right most kvp borrows parent shared with right sibling
    current->kvp_[current->kvpSize()] = parentOfCurrent->kvp_[kvpParentIndexInterval.upperBound()];
    
    if (current->childrenSize() <= 0)
    {
        current->children_[1] = rightSibling->children_[0];
    }
    else
    {
        // current right most child borrows right sibling left most child
        current->children_[current->childrenSize()] = rightSibling->children_[0];
    }
    
    rightSibling->children_[0] = nullptr;
    current->kvpSize_++;
    rightSibling->kvpSize_--;
    
    // shared parent kvp borrows right sibling left most kvp
    parentOfCurrent->kvp_[kvpParentIndexInterval.upperBound()] = rightSibling->kvp_[0];
    rightSibling->kvp_[0] = nullptr;
    
    // shift kvp left by 1 for right sibling
    for (LONG kvpIndex = 0; kvpIndex < rightSibling->kvpSize(); kvpIndex++)
    {
        rightSibling->kvp_[kvpIndex] = rightSibling->kvp_[kvpIndex + 1];
        rightSibling->kvp_[kvpIndex + 1] = nullptr;
    }
    
    // shift children left by 1 for right sibling
    for (LONG childrenIndex = 0; childrenIndex < rightSibling->childrenSize(); childrenIndex++)
    {
        rightSibling->children_[childrenIndex] = rightSibling->children_[childrenIndex + 1];
        rightSibling->children_[childrenIndex + 1] = nullptr;
    }
}

template <typename T, typename K>
void BTree<T, K>::merge(TreeNode *&leftChild, TreeNode *&rightChild, TreeNode *&parent, LONG sharedParentIndex)
{
    // left sibling right most kvp takes parent shared with current
    leftChild->kvp_[leftChild->kvpSize()] = parent->kvp_[sharedParentIndex];
    leftChild->kvpSize_++;
    
    parent->kvp_[sharedParentIndex] = nullptr;
    parent->kvpSize_--;
    
    // merge current with left sibling
    for (LONG kvpIndex = 0; kvpIndex < rightChild->kvpSize(); kvpIndex++)
    {
        leftChild->kvp_[leftChild->kvpSize()] = rightChild->kvp_[kvpIndex];
        rightChild->kvp_[kvpIndex] = nullptr;
        leftChild->children_[leftChild->childrenSize() - 1] = rightChild->children_[kvpIndex];
        rightChild->children_[kvpIndex] = nullptr;
        leftChild->kvpSize_++;
    }
    
    if (rightChild->kvpSize() > 0)
    {
        leftChild->children_[leftChild->childrenSize() - 1] = rightChild->children_[rightChild->childrenSize() - 1];
        rightChild->children_[rightChild->childrenSize() - 1] = nullptr;
    }
    else
    {
        leftChild->children_[leftChild->childrenSize() - 1] = rightChild->children_[0];
        rightChild->children_[0] = nullptr;
    }
    
    // left sibling parent current
    // remove current node
    delete rightChild;
    rightChild = nullptr;
    
    parent->children_[sharedParentIndex + 1] = nullptr;
    
    for (LONG kvpIndex = sharedParentIndex; kvpIndex < parent->kvpSize(); kvpIndex++)
    {
        parent->kvp_[kvpIndex] = parent->kvp_[kvpIndex + 1];
        parent->kvp_[kvpIndex + 1] = nullptr;
    }
    
    for (LONG childrenIndex = sharedParentIndex + 1; childrenIndex < parent->childrenSize(); childrenIndex++)
    {
        parent->children_[childrenIndex] = parent->children_[childrenIndex + 1];
        parent->children_[childrenIndex + 1] = nullptr;
    }
}

template <typename T, typename K>
void BTree<T, K>::deleteFromRoot(TreeNode *&current, const Interval &kvpIndexInterval)
{
    // root has only 1 key
    if (root_->kvpSize() == 1)
    {
        delete root_;
        root_ = nullptr;
    }
    // root has more than 1 key
    else
    {
        delete root_->kvp_[kvpIndexInterval.lowerBound()];
        root_->kvp_[kvpIndexInterval.lowerBound()] = nullptr;
        
        root_->kvpSize_--;
        
        for (LONG kvpIndex = kvpIndexInterval.lowerBound(); kvpIndex < root_->kvpSize(); kvpIndex++)
        {
            root_->kvp_[kvpIndex] = root_->kvp_[kvpIndex + 1];
            root_->kvp_[kvpIndex + 1] = nullptr;
        }
    }
    
    count_--;
}

template <typename T, typename K>
bool BTree<T, K>::deleteFromNonLeaf(TreeNode *&current, Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack)
{
    // find left most kvp in right subtree or
    // find right most kvp in left subtree
    // current kvp will use chosen kvp in subtree
    // delete chosen kvp in subtree
    
    TreeNode* next = current->children_[kvpIndexInterval.upperBound() + 1];
    
    while (next->children_[0] != nullptr)
    {
        next = next->children_[0];
    }
    
    ancestryStack.push(current);
    
    if (next->kvpSize() > (LONG)ceil((next->kvpMaxSize() - 2)/2.0))
    {
        next = current->children_[kvpIndexInterval.upperBound() + 1];
        
        while (next->children_[0] != nullptr)
        {
            ancestryStack.push(next);
            next = next->children_[0];
        }
        
        current->kvp_[kvpIndexInterval.lowerBound()]->key_ = next->kvp_[0]->key();
        current->kvp_[kvpIndexInterval.lowerBound()]->value_ = next->kvp_[0]->value();
        
        kvpIndexInterval = Interval(0, 0);
        
        current = next;
        
        if (deleteFromLeaf(current, kvpIndexInterval, ancestryStack))
        {
            return true;
        }
    }
    else
    {
        TreeNode * prev = current->children_[kvpIndexInterval.lowerBound()];
        
        while (prev->children_[prev->childrenSize() - 1] != nullptr)
        {
            ancestryStack.push(prev);
            prev = prev->children_[prev->childrenSize() - 1];
        }
        
        current->kvp_[kvpIndexInterval.lowerBound()]->key_ = prev->kvp_[prev->kvpSize() - 1]->key();
        current->kvp_[kvpIndexInterval.lowerBound()]->value_ = prev->kvp_[prev->kvpSize() - 1]->value();
        
        kvpIndexInterval = Interval(prev->kvpSize() - 1, prev->kvpSize() - 1);
        
        current = prev;
        
        if (deleteFromLeaf(current, kvpIndexInterval, ancestryStack))
        {
            return true;
        }
    }
    
    return false;
}

template <typename T, typename K>
bool BTree<T, K>::deleteFromLeaf(TreeNode *&current, const Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack)
{
    T key = current->kvp_[kvpIndexInterval.lowerBound()]->key();
    
    // root node
    if (current == root_)
    {
        deleteFromRoot(current, kvpIndexInterval);
        
        return true;
    }
    // non-root node
    else
    {
        // removal needs borrowing
        if (current->kvpSize() == (LONG)ceil((current->kvpMaxSize() - 2)/2.0))
        {
            TreeNode *parentOfCurrent = ancestryStack.top();
            
            Interval kvpParentIndexInterval = parentOfCurrent->binarySearch(key, 0, parentOfCurrent->kvpSize() - 1);
            
            TreeNode* leftSibling = nullptr;
            TreeNode* rightSibling = nullptr;
            
            if (kvpParentIndexInterval.lowerBound() == kvpParentIndexInterval.upperBound())
            {
                kvpParentIndexInterval = Interval(kvpParentIndexInterval.lowerBound() - 1, kvpParentIndexInterval.upperBound());
            }
            
            if (kvpParentIndexInterval.lowerBound() > -1 &&
                parentOfCurrent->children_[kvpParentIndexInterval.lowerBound()] != nullptr)
            {
                leftSibling = parentOfCurrent->children_[kvpParentIndexInterval.lowerBound()];
            }
            
            if (kvpParentIndexInterval.upperBound() < parentOfCurrent->kvpSize() &&
                parentOfCurrent->children_[kvpParentIndexInterval.upperBound() + 1] != nullptr)
            {
                rightSibling = parentOfCurrent->children_[kvpParentIndexInterval.upperBound() + 1];
            }
            
            // try to see if we can borrow from left sibling first
            if (leftSibling != nullptr &&
                leftSibling->kvpSize() > (LONG)ceil((leftSibling->kvpMaxSize() - 2)/2.0))
            {
                delete current->kvp_[kvpIndexInterval.lowerBound()];
                current->kvp_[kvpIndexInterval.lowerBound()] = nullptr;
                current->kvpSize_--;
                
                for (LONG kvpIndex = kvpIndexInterval.lowerBound(); kvpIndex < current->kvpSize(); kvpIndex++)
                {
                    current->kvp_[kvpIndex] = current->kvp_[kvpIndex + 1];
                    current->kvp_[kvpIndex + 1] = nullptr;
                }
                
                count_--;
                
                borrowFromLeft(current, leftSibling, parentOfCurrent, kvpParentIndexInterval);
                
                return true;
            }
            // cannot borrow from left sibling
            // try to see if we can borrow from right sibling
            else if (rightSibling != nullptr &&
                     rightSibling->kvpSize() > (LONG)ceil((rightSibling->kvpMaxSize() - 2)/2.0))
            {
                delete current->kvp_[kvpIndexInterval.lowerBound()];
                current->kvp_[kvpIndexInterval.lowerBound()] = nullptr;
                current->kvpSize_--;
                
                for (LONG kvpIndex = kvpIndexInterval.lowerBound(); kvpIndex < current->kvpSize(); kvpIndex++)
                {
                    current->kvp_[kvpIndex] = current->kvp_[kvpIndex + 1];
                    current->kvp_[kvpIndex + 1] = nullptr;
                }
                
                count_--;
                
                borrowFromRight(current, rightSibling, parentOfCurrent, kvpParentIndexInterval);
                
                return true;
            }
            else
            {
                // cannot borrow from either siblings
                // merge node with a sibling
                delete current->kvp_[kvpIndexInterval.lowerBound()];
                current->kvp_[kvpIndexInterval.lowerBound()] = nullptr;
                
                current->kvpSize_--;
                
                for (LONG kvpIndex = kvpIndexInterval.lowerBound(); kvpIndex < current->kvpSize(); kvpIndex++)
                {
                    current->kvp_[kvpIndex] = current->kvp_[kvpIndex + 1];
                    current->kvp_[kvpIndex + 1] = nullptr;
                }
                
                count_--;
                
                // try to merge with left sibling
                if (leftSibling != nullptr)
                {
                    merge(leftSibling, current, parentOfCurrent, kvpParentIndexInterval.lowerBound());
                }
                // left sibling does not exist
                // try to merge with right sibling
                else if (rightSibling != nullptr)
                {
                    merge(current, rightSibling, parentOfCurrent, kvpParentIndexInterval.upperBound());
                }
            }
        }
        // removal does not need borrowing
        else
        {
            delete current->kvp_[kvpIndexInterval.lowerBound()];
            current->kvp_[kvpIndexInterval.lowerBound()] = nullptr;
            
            current->kvpSize_--;
            
            for (LONG kvpIndex = kvpIndexInterval.lowerBound(); kvpIndex < current->kvpSize(); kvpIndex++)
            {
                current->kvp_[kvpIndex] = current->kvp_[kvpIndex + 1];
                current->kvp_[kvpIndex + 1] = nullptr;
            }
            
            count_--;
            
            return true;
        }
    }
    
    return false;
}

template <typename T, typename K>
bool BTree<T, K>::findForRemove(const T &key, TreeNode *&current, Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack)
{
    while (true)
    {
        kvpIndexInterval = current->binarySearch(key, 0, current->kvpSize() - 1);
        
        if (kvpIndexInterval.lowerBound() == kvpIndexInterval.upperBound())
        {
            return true;
        }
        else
        {
            TreeNode *next = nullptr;
            
            if (kvpIndexInterval.lowerBound() < 0)
            {
                next = current->left(kvpIndexInterval.upperBound());
            }
            else
            {
                next = current->right(kvpIndexInterval.lowerBound());
            }
            
            if (next != nullptr)
            {
                ancestryStack.push(current);
                current = next;
            }
            else
            {
                return false;
            }
        }
    }
}

template <typename T, typename K>
bool BTree<T, K>::remove(const T &key, K &outValue)
{
    if (root_ == nullptr)
    {
        return false;
    }
    
    TreeNode *current = root_;
    
    Stack<TreeNode*> ancestryStack;
    
    Interval kvpIndexInterval;
    
    if (!findForRemove(key, current, kvpIndexInterval, ancestryStack))
    {
        return false;
    }
    
    outValue = current->kvp_[kvpIndexInterval.lowerBound()]->value();
    
    // non-leaf node
    if (current->children_[kvpIndexInterval.lowerBound()] != nullptr)
    {
        if (deleteFromNonLeaf(current, kvpIndexInterval, ancestryStack))
        {
            return true;
        }
    }
    // leaf node
    else
    {
        if (deleteFromLeaf(current, kvpIndexInterval, ancestryStack))
        {
            return true;
        }
    }
    
    while (!ancestryStack.isEmpty())
    {
        current = ancestryStack.top();
        ancestryStack.pop();
        
        if (current == root_)
        {
            if (current->kvpSize() < 1)
            {
                root_ = current->children_[0];
                current->children_[0] = nullptr;
                delete current;
                current = nullptr;
            }
            
            return true;
        }
        else if (current->kvpSize() < (LONG)ceil((current->kvpMaxSize() - 2)/2.0))
        {
            TreeNode* parentOfCurrent = ancestryStack.top();
            
            Interval kvpParentIndexInterval;
            
            for (LONG childrenIndex = 0; childrenIndex < parentOfCurrent->childrenMaxSize(); childrenIndex++)
            {
                if (parentOfCurrent->children_[childrenIndex] == current)
                {
                    kvpParentIndexInterval = Interval(childrenIndex - 1, childrenIndex);
                    break;
                }
            }
            
            TreeNode* leftSibling = nullptr;
            TreeNode* rightSibling = nullptr;
            
            if (kvpParentIndexInterval.lowerBound() > -1 &&
                parentOfCurrent->children_[kvpParentIndexInterval.lowerBound()] != nullptr)
            {
                leftSibling = parentOfCurrent->children_[kvpParentIndexInterval.lowerBound()];
            }
            
            if (kvpParentIndexInterval.upperBound() < parentOfCurrent->kvpMaxSize() &&
                parentOfCurrent->children_[kvpParentIndexInterval.upperBound() + 1] != nullptr)
            {
                rightSibling = parentOfCurrent->children_[kvpParentIndexInterval.upperBound() + 1];
            }
            
            // try to borrow from left sibling
            if (leftSibling != nullptr &&
                leftSibling->kvpSize() > (LONG)ceil((leftSibling->kvpMaxSize() - 2)/2.0))
            {
                borrowFromLeft(current, leftSibling, parentOfCurrent, kvpParentIndexInterval);
                
                continue;
            }
            // cannot borrow from left sibling
            // try to borrow from right sibling
            else if (rightSibling != nullptr &&
                     rightSibling->kvpSize() > (LONG)ceil((rightSibling->kvpMaxSize() - 2)/2.0))
            {
                borrowFromRight(current, rightSibling, parentOfCurrent, kvpParentIndexInterval);
                
                continue;
            }
            
            // cannot borrow from either
            // try to merge with left sibling
            if (leftSibling != nullptr)
            {
                merge(leftSibling, current, parentOfCurrent, kvpParentIndexInterval.lowerBound());
                
                continue;
            }
            // left sibling does not exist
            // merge with right sibling
            else if (rightSibling != nullptr)
            {
                merge(current, rightSibling, parentOfCurrent, kvpParentIndexInterval.upperBound());
                
                continue;
            }
        }
        else
        {
            return true;
        }
    }
    
    return true;
}

template <typename T, typename K>
bool BTree<T, K>::find(const T &key, K &outValue)
{
    if (root_ == nullptr)
    {
        return false;
    }
    
    TreeNode *current = root_;
    
    while (current != nullptr)
    {
        Interval kvpIndexArray = current->binarySearch(key, 0, current->kvpSize() - 1);

        if (kvpIndexArray.lowerBound() == kvpIndexArray.upperBound())
        {
            outValue = current->kvp_[kvpIndexArray.lowerBound()]->value();
            
            return true;
        }
        else
        {
            if (kvpIndexArray.lowerBound() < 0)
            {
                current = current->left(kvpIndexArray.upperBound());
            }
            else
            {
                current = current->right(kvpIndexArray.lowerBound());
            }
        }
    }
    
    return false;
}

template <typename T, typename K>
const LONG BTree<T, K>::height() const
{
    if (root_ == nullptr)
    {
        return 0;
    }
    
    return root_->height();
}

template <typename T, typename K>
const LONG BTree<T, K>::count() const
{
    return count_;
}

template <typename T, typename K>
const bool BTree<T, K>::isEmpty() const
{
    return count_ <= 0;
}

#endif
