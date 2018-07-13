//
//  BTree.hpp
//  BTree
//
//  Created by Richard Szeto on 7/4/18.
//  Copyright © 2018 Richard Szeto. All rights reserved.
//

#ifndef BTree_hpp
#define BTree_hpp

typedef long long LONG;

#include <stdio.h>

#include "Interval.hpp"
#include "../Stack/Stack.hpp"

template <typename T, typename K>
class BTree
{
    class KeyValuePair
    {
    private:
        T key_;
        K value_;
        
    public:
        KeyValuePair(T newKey, K newValue) : key_(newKey), value_(newValue) {}
        const T key() const { return key_; }
        const K value() const { return value_; }
        
        friend class BTree<T, K>;
        friend class TreeNode;
    };
    class TreeNode
    {
    private:
        LONG kvpMaxSize_;
        LONG kvpSize_;
        KeyValuePair** kvp_;
        TreeNode** children_;
        Interval binarySearch(T key, LONG startIndex, LONG endIndex);
        
    public:
        TreeNode(LONG kvpMaxSize);
        virtual ~TreeNode();
        TreeNode* left(LONG kvpIndex);
        TreeNode * right(LONG kvpIndex);
        const LONG kvpMaxSize() const;
        const LONG kvpSize() const;
        const LONG childrenMaxSize() const;
        const LONG childrenSize() const;
        const LONG height() const;
        
        friend class BTree<T, K>;
    };
    
private:
    TreeNode* root_;
    LONG count_;
    LONG kvpMaxSize_;
    bool findForInsert(const T &newKey, TreeNode *&current, Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack);
    void balanceAfterInsert(TreeNode *&current, Stack<TreeNode*> &ancestryStack);
    bool findForRemove(const T &key, TreeNode *&current, Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack);
    void deleteFromRoot(TreeNode *&current, const Interval& kvpIndexInterval);
    bool deleteFromNonLeaf(TreeNode *&current, Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack);
    bool deleteFromLeaf(TreeNode *&current, const Interval &kvpIndexInterval, Stack<TreeNode*> &ancestryStack);
    void borrowFromLeft(TreeNode *&current, TreeNode *&leftSibling, TreeNode *&parentOfCurrent, const Interval &kvpParentIndexInterval);
    void borrowFromRight(TreeNode *&current, TreeNode *&rightSibling, TreeNode *& parentOfCurrent, const Interval &kvpParentIndexInterval);
    void merge(TreeNode *&leftChild, TreeNode *&rightChild, TreeNode *&parent, LONG sharedParentIndex);
    
public:
    BTree() : root_(nullptr), count_(0), kvpMaxSize_(3) {}
    BTree(int kvpMaxSize) : root_(nullptr), count_(0), kvpMaxSize_(3)
    {
        if (kvpMaxSize > 2)
        {
            kvpMaxSize_ = kvpMaxSize + 1;
        }
    }
    virtual ~BTree();
    bool insert(const T &newKey, const K &newValue);
    bool replace(const T &key, const K &newValue);
    bool remove(const T &key, K &outValue);
    bool find(const T &key, K &outValue);
    const LONG height() const;
    const LONG count() const;
    const bool isEmpty() const;
};

#endif /* BTree_hpp */
