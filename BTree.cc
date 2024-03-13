/*
 * BTree.cc - implementation of methods of class BTree as declared in BTree.h.
 *
 * Several of the methods in this file must be implemented by students for
 * CPS 222 project 4
 *
 * If this file is compiled with the symbol PROFESSOR_VERSION defined,
 * it expects a file of that name to exist, and uses the code from
 * that file in place of selected dummy methods left to be written by students.
 *
 * Starting code Copyright (c) 2001, 2002, 2003 - Russell C. Bjork
 * Updates copyright (c) 2016, 2019 - Russ Tuck
 *
 * Student Author: Wenlan Ji
 */

#include <iostream>
#include <cmath>
using std::cout;

#include "BTree.h"
#include "BTreeFile.h"
#include "BTreeBlock.h"

BTree::BTree(string name)
  : _file(* new BTreeFile(name))
{ }

#ifndef PROFESSOR_VERSION

bool BTree::lookup(string key, string & value) const {
  BTreeFile::BlockNumber root = _file.getRoot();
  BTreeBlock block;
  // if there is no root
  if (root == 0) {
    return false;
  }
  // if there is a root
  else {
    // get the root
    _file.getBlock(root, block);
    // if the block is not a leaf
    while (!block.isLeaf()) {
      // check if the key is in this level
      for (unsigned i = 0; i < block.getNumberOfKeys(); i++) {
        if (key == block.getKey(i)) {
          value = block.getValue(i);
          return true;
        }
      }
      // go to the child
      BTreeFile::BlockNumber number = block.getChild(block.getPosition(key));
      _file.getBlock(number, block);
    }
    // if the block is a leaf
    for (unsigned i = 0; i < block.getNumberOfKeys(); i++) {
      if (key == block.getKey(i)) {
        value = block.getValue(i);
        return true;
      }
    }
    // the key is not in this tree
    return false;
  }
}

void BTree::insert(string key, string value) {
  BTreeFile::BlockNumber root = _file.getRoot();
  BTreeBlock block;
  BTreeFile::BlockNumber number = root;
  // if there is no root
  if (root == 0) {
    number = _file.allocateBlock();
    _file.setRoot(number);
    block.insert(block.getPosition(key), key, value, 0);
    block.setChild(0, 0);
    _file.putBlock(number, block);
  }
  // if there is a root
  else {
    string promotedKey;
    string promotedValue;
    // rightNumber is the block number of the newly splited block,
    // 0 means the block does not split
    BTreeFile::BlockNumber rightNumber = insertHelper(key, value, number, 
                                                      promotedKey, promotedValue);
    // if the block splits
    if (rightNumber != 0) {
      // set a new root
      BTreeBlock newRootBlock;
      BTreeFile::BlockNumber newRootNumber = _file.allocateBlock();
      _file.setRoot(newRootNumber);
      // insert the promoted and key and value into the root
      newRootBlock.insert(newRootBlock.getPosition(promotedKey), 
                        promotedKey, promotedValue, rightNumber);
      newRootBlock.setChild(0, number);
      _file.putBlock(newRootNumber, newRootBlock);
    }
  }
}

BTreeFile::BlockNumber BTree::insertHelper(string key, string value,
                                          BTreeFile::BlockNumber root,
                                          string &promotedKey,
                                          string &promotedValue) {
  // get the root
  BTreeBlock block;
  BTreeFile::BlockNumber number = root;
  _file.getBlock(number, block);
  // while the block is not a leaf, find the leaf
  if (! block.isLeaf()) {
    // if the key is already in this block
    if (key == block.getKey(block.getPosition(key))) {
      block.setValue(block.getPosition(key), value);
      _file.putBlock(number, block);
      return 0;
    }
    // go to the child
    BTreeFile::BlockNumber childNumber = block.getChild(block.getPosition(key));
    // recursively call insertHelper() on the child
    BTreeFile::BlockNumber rightNumber = insertHelper(key, value, childNumber,
                                                      promotedKey, promotedValue);
    // if the block splits
    if (rightNumber != 0) {
      block.insert(block.getPosition(promotedKey), 
                  promotedKey, promotedValue, rightNumber);
      _file.putBlock(number, block);
    }
  }
  // if the leaf is found, insert into it
  else {
    // if the key is already in this block
    if (key == block.getKey(block.getPosition(key))) {
      block.setValue(block.getPosition(key), value);
      _file.putBlock(number, block);
      return 0;
    }
    block.insert(block.getPosition(key), key, value, 0);
    _file.putBlock(number, block);
  }
  // if the block needs to be splited, split it
  BTreeFile::BlockNumber rightNumber = 0;
  if (block.splitNeeded()) {
    BTreeBlock rightBlock;
    rightNumber = _file.allocateBlock();
    block.split(promotedKey, promotedValue, rightBlock);
    _file.putBlock(number, block);
    _file.putBlock(rightNumber, rightBlock);
  }
  return rightNumber;
}

int BTree::count() const {
  int keyNumber = 0;
  BTreeFile::BlockNumber root = _file.getRoot();
  // if there is a root
  if (root != 0) {
    keyNumber += countSubtree(root);
  }
  return keyNumber;
}

int BTree::countSubtree(BTreeFile::BlockNumber blockNum) const {
  BTreeBlock block;
  _file.getBlock(blockNum, block);
  int keyNumber = block.getNumberOfKeys();
  // if the block is not a leaf
  if (!block.isLeaf()) {
    // this following for loop recursively goes through all the blocks
    // and finds the number of the keys in the subtree
    int subtreeBlockNumber = block.getNumberOfKeys() + 1;
    for (int i = 0; i < subtreeBlockNumber; i++) {
      int subNumber = countSubtree(block.getChild(i));
      keyNumber += subNumber;
    }
  }
  return keyNumber;
}

int BTree::depth(string key) const {
  int depth = -1;
  BTreeFile::BlockNumber root = _file.getRoot();
  BTreeBlock block;
  // if there is a root
  if (root != 0) {
    _file.getBlock(root, block);
    depth++;
    // while the block is not a leaf
    while (!block.isLeaf()) {
      // if the key is found
      if (key == block.getKey(block.getPosition(key))) {
        return depth;
      }
      // go to the child
      BTreeFile::BlockNumber childNumber = block.getChild(block.getPosition(key));
      _file.getBlock(childNumber, block);
      depth++;
    }
  }
  return depth;
}

bool BTree::remove(string key) {
  BTreeFile::BlockNumber root = _file.getRoot();
  BTreeFile::BlockNumber number = root;
  BTreeBlock rootBlock;
  BTreeBlock block;
  BTreeBlock parentBlock;
  string value;
  // if there is no root
  if (!lookup(key, value)) {
    return false;
  }
  // if there is a root
  else {
    _file.getBlock(number, block);
    _file.getBlock(root, rootBlock);
    // while the key is not in this level
    while (key != block.getKey(block.getPosition(key))) {
      // save the original block as parentBlock
      parentBlock = block;
      // go to the child
      number = block.getChild(block.getPosition(key));
      _file.getBlock(number, block);
    }
    // if the block is a leaf
    if (block.isLeaf()) {
      // if the block is the root
      if (number == root) {
        block.remove(block.getPosition(key));
        _file.putBlock(number, block);
      }
      // if the block is not the root
      else {
        // if removing the key will not need to borrow key from neighbor
        if (block.getNumberOfKeys() > floor(DEGREE / 2)) {
          block.remove(block.getPosition(key));
          _file.putBlock(number, block);
        }
        // if removing the key need to borrow key from its sibling,
        // and the block has both left and right siblings
        else if (parentBlock.getPosition(key) > 0 && parentBlock.getPosition(key) 
                 < parentBlock.getNumberOfKeys()) {
          // find the left sibling
          unsigned leftSibPosition = parentBlock.getPosition(key) - 1;
          BTreeFile::BlockNumber leftSibNumber = parentBlock.getChild(leftSibPosition);
          BTreeBlock leftSibBlock;
          _file.getBlock(leftSibNumber, leftSibBlock);
          string leftSibKey = leftSibBlock.getKey(leftSibBlock.getNumberOfKeys() - 1);
          string leftSibValue = leftSibBlock.getValue(leftSibBlock.getNumberOfKeys() - 1);
          // find the right sibling
          unsigned rightSibPosition = parentBlock.getPosition(key) + 1;
          BTreeFile::BlockNumber rightSibNumber = parentBlock.getChild(rightSibPosition);
          BTreeBlock rightSibBlock;
          _file.getBlock(rightSibNumber, rightSibBlock);
          string rightSibKey = rightSibBlock.getKey(0);
          string rightSibValue = rightSibBlock.getValue(0);
          // if borrowing key from the left sibling
          if (leftSibBlock.getNumberOfKeys() > floor(DEGREE / 2)) {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(leftSibKey));
            string parentValue = parentBlock.getValue(parentBlock.getPosition(leftSibKey));
            leftSibBlock.remove(leftSibBlock.getPosition(leftSibKey));
            unsigned parentPosition = parentBlock.getPosition(parentKey);
            parentBlock.setKey(parentPosition, leftSibKey);
            parentBlock.setValue(parentPosition, leftSibValue);
            block.insert(block.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            _file.putBlock(leftSibNumber, leftSibBlock);
          }
          // if borrowing key from the right sibling
          else if (rightSibBlock.getNumberOfKeys() > floor(DEGREE / 2)) {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(rightSibKey) - 1);
            string parentValue = parentBlock.getValue(parentBlock.getPosition(rightSibKey));
            rightSibBlock.remove(rightSibBlock.getPosition(rightSibKey));
            unsigned parentPosition = parentBlock.getPosition(parentKey);
            parentBlock.setKey(parentPosition, rightSibKey);
            parentBlock.setValue(parentPosition, rightSibValue);
            block.insert(block.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            _file.putBlock(rightSibNumber, rightSibBlock);
          }
          // if removing the key need to merge two key
          // we are going to put all keys from the current block into its left block
          else {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(leftSibKey));
            string parentValue = parentBlock.getValue(parentBlock.getPosition(leftSibKey));
            leftSibBlock.insert(leftSibBlock.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            for (unsigned i = 0; i < block.getNumberOfKeys(); i++) {
              string mergeKey = block.getKey(i);
              string mergeValue = block.getValue(i);
              leftSibBlock.insert(leftSibBlock.getPosition(mergeKey), mergeKey, mergeValue, 0);
            }
            parentBlock.remove(parentBlock.getPosition(leftSibKey));
            _file.putBlock(leftSibNumber, leftSibBlock);
            if (parentBlock.getNumberOfKeys() == 0) {
              _file.deallocateBlock(root);
              _file.setRoot(leftSibNumber);
            }
          }
        }
        // if removing the key need to borrow key from its sibling,
        // and the block only has right sibling
        else if (parentBlock.getPosition(key) == 0) {
          // find the right sibling
          unsigned rightSibPosition = parentBlock.getPosition(key) + 1;
          BTreeFile::BlockNumber rightSibNumber = parentBlock.getChild(rightSibPosition);
          BTreeBlock rightSibBlock;
          _file.getBlock(rightSibNumber, rightSibBlock);
          string rightSibKey = rightSibBlock.getKey(0);
          string rightSibValue = rightSibBlock.getValue(0);
          // if borrowing key from the right sibling
          if (rightSibBlock.getNumberOfKeys() > floor(DEGREE / 2)) {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(rightSibKey) - 1);
            string parentValue = parentBlock.getValue(parentBlock.getPosition(rightSibKey));
            rightSibBlock.remove(rightSibBlock.getPosition(rightSibKey));
            unsigned parentPosition = parentBlock.getPosition(parentKey);
            parentBlock.setKey(parentPosition, rightSibKey);
            parentBlock.setValue(parentPosition, rightSibValue);
            block.insert(block.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            _file.putBlock(rightSibNumber, rightSibBlock);
          }
          // if removing the key need to merge two key
          // we are going to put all keys from the current block into its right block
          else {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(key));
            string parentValue = parentBlock.getValue(parentBlock.getPosition(key));
            rightSibBlock.insert(rightSibBlock.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            for (unsigned i = 0; i < block.getNumberOfKeys(); i++) {
              string mergeKey = block.getKey(i);
              string mergeValue = block.getValue(i);
              rightSibBlock.insert(rightSibBlock.getPosition(mergeKey), mergeKey, mergeValue, 0);
            }
            parentBlock.remove(parentBlock.getPosition(key));
            parentBlock.setChild(0, rightSibNumber);
            _file.putBlock(rightSibNumber, rightSibBlock);
            if (parentBlock.getNumberOfKeys() == 0) {
              _file.deallocateBlock(root);
              _file.setRoot(rightSibNumber);
            }
          }
        }
        // if removing the key need to borrow key from its sibling,
        // and the block only has left sibling
        else if (parentBlock.getPosition(key) == parentBlock.getNumberOfKeys()) {
          // find the left sibling
          unsigned leftSibPosition = parentBlock.getPosition(key) - 1;
          BTreeFile::BlockNumber leftSibNumber = parentBlock.getChild(leftSibPosition);
          BTreeBlock leftSibBlock;
          _file.getBlock(leftSibNumber, leftSibBlock);
          string leftSibKey = leftSibBlock.getKey(leftSibBlock.getNumberOfKeys() - 1);
          string leftSibValue = leftSibBlock.getValue(leftSibBlock.getNumberOfKeys() - 1);
          // if borrowing key from the left sibling
          if (leftSibBlock.getNumberOfKeys() > floor(DEGREE / 2)) {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(leftSibKey));
            string parentValue = parentBlock.getValue(parentBlock.getPosition(leftSibKey));
            leftSibBlock.remove(leftSibBlock.getPosition(leftSibKey));
            unsigned parentPosition = parentBlock.getPosition(parentKey);
            parentBlock.setKey(parentPosition, leftSibKey);
            parentBlock.setValue(parentPosition, leftSibValue);
            block.insert(block.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            _file.putBlock(leftSibNumber, leftSibBlock);
          }
          // if removing the key need to merge two key
          // we are going to put all keys from the right block into its left block
          else {
            string parentKey = parentBlock.getKey(parentBlock.getPosition(leftSibKey));
            string parentValue = parentBlock.getValue(parentBlock.getPosition(leftSibKey));
            leftSibBlock.insert(leftSibBlock.getPosition(parentKey), parentKey, parentValue, 0);
            block.remove(block.getPosition(key));
            for (unsigned i = 0; i < block.getNumberOfKeys(); i++) {
              string mergeKey = block.getKey(i);
              string mergeValue = block.getValue(i);
              leftSibBlock.insert(leftSibBlock.getPosition(mergeKey), mergeKey, mergeValue, 0);
            }
            parentBlock.remove(parentBlock.getPosition(leftSibKey));
            _file.putBlock(leftSibNumber, leftSibBlock);
            if (parentBlock.getNumberOfKeys() == 0) {
              _file.deallocateBlock(root);
              _file.setRoot(leftSibNumber);
            }
          }
        }
        // put the current block and parent block back to the file
        _file.putBlock(number, block);
        _file.putBlock(root, parentBlock);
      }
    }
    // if the block is empty, deallocate its number
    if (block.getNumberOfKeys() == 0) {
      _file.deallocateBlock(number);
      _file.setRoot(0);
    }
    return true;
  }
}

list<string> BTree::findSibling(BTreeFile::BlockNumber parentNumber, unsigned sibBlockDiffer, unsigned keyPosition) {
  list<Vertex> keyValueList;
  BTreeBlock parentBlock;
  _file.getBlock(parentNumber, parentBlock);
  unsigned sibPosition = parentBlock.getPosition(key) + sibBlockDiffer;
  BTreeFile::BlockNumber sibNumber = parentBlock.getChild(sibPosition);
  BTreeBlock sibBlock;
  _file.getBlock(sibNumber, sibBlock);
  string sibKey = sibBlock.getKey(keyPosition);
  string sibValue = sibBlock.getValue(keyPosition);
  keyValueList.push_back(sibKey);
  keyValueList.push_back(sibValue);
  return keyValueList;
}

// Students change nothing below here.

#else

#define QUOTE(Q) #Q
#define INCLUDE_NAME(X) QUOTE(X)
#include INCLUDE_NAME(PROFESSOR_VERSION)

#endif

void BTree::print() const {
  cout << "BTree in file ";
  printInfo();
  cout << endl;

  BTreeFile::BlockNumber root = _file.getRoot();
  if (root == 0) {
    cout << "Empty tree" << endl;
  } else {
    _file.printBlock(root, true, 1);
  }
}


void BTree::print(BTreeFile::BlockNumber blockNumber) const {
  _file.printBlock(blockNumber, false, 1);
}


void BTree::printInfo() const {
  _file.printHeaderInfo();
}


void BTree::printStats() const {
  _file.printStats();
}


BTree::~BTree() {
  delete (& _file);
}
