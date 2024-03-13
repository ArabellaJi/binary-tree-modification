/*
 * BTree.h - declarations for the class BTree.	CPS 222, Project 4.
 *
 * This class represents a B-Tree of key/value pairs.  Both keys and values
 * are strings.  The primary methods for managing a BTree's contents are:
 *   insert()
 *   lookup()
 *   remove()
 * which all operate on key/value pairs.
 *
 * In addition, these methods are used for tests, to examine the BTree's
 * structure:
 *   count()
 *   depth()
 * There are also various print methods, primarily for debugging.
 *
 * The nodes of the tree are BTreeBlock objects.  All of the BTreeBlocks in
 * a BTree are stored in a single BTreeFile.  The BTreeFile holds the
 * permanent contents of every node of the tree.  So each BTreeBlock must
 * be read from the BTreeFile before being used, and written back to the
 * BTreeFile after it is changed.  Every method of BTree must start and
 * end with all blocks safely stored in the BTreeFile.
 *
 * Copyright (c) 2001, 2002, 2003 - Russell C. Bjork
 * Updates copyright (c) 2016, 2019 - Russ Tuck
 */

#ifndef BTREE_H
#define BTREE_H

#include <string>
#include "BTreeFile.h"
#include "BTreeBlock.h"

class BTree
{
 public:

  /* Constructor
   *
   * Parameter:
   *   name - file name (which may be a relative or absolute path).
   *     If a file of this name exists, it is opened and the tree
   *     stored in it is accessed; else a new file containing an
   *     empty tree is created.
   */
  BTree(string name);


  /* Lookup a key.  If found, pass its value back in the "value" parameter
   * and return true.  Otherwise return false.
   *
   * Parameters:
   *   key - look for this key in the tree.
   *   value - passed in as a placeholder, to be filled in with
   *     the key's actual value if the key is found.
   *
   * Return: true if key is found, and false otherwise
   */
  bool lookup(string key, string & value) const;


  /* Insert a new key, value pair into this structure.  If there is
   * already a node with this key, replace its old value with this
   * new value.
   *
   * Parameters:
   *   key - Key to add. It must not contain any embedded spaces or
   *     control characters.
   *   value - Value to associate with the key.
   */
  void insert(string key, string value);


  /* Count the keys in the tree.  It's only used by unit tests,
   * but is required for checking that there aren't extra nodes.
   *
   * Return: the number of keys in the tree
   */
  int count() const;


  /* Return the depth of the node holding a particular key. It's only
   * used by unit tests, but is required for checking that the tree
   * structure is correct. (Its implementation is very similar to lookup.
   * So write and test lookup first.)
   *
   * Parameter:
   *   key - key of node to find depth of
   *
   * Return: the depth of the node containing the key, or -1.
   *  -1 means the key is not present.
   *   0 means the key is in the root node.
   *   1...  Positive values count levels below the root.
   *
   * (Note that while the root is at depth 0, it is called level 1
   * when the tree is printed.)
   */
  int depth(string key) const;


  /* Remove a key and its associated value.  Return true if
   * found and removed, false if not.
   *
   * Parameter:
   *   key - key to remove.
   *
   * Return: true if key is found, and false otherwise
   */
  bool remove(string key);


  /* Print the contents of the entire tree to cout for testing/debugging
   * purposes.
   */
  void print() const;

  /* Print the contents of a block to cout for testing/debugging purposes.
   *
   * Parameter:
   *   blockNumber - block to print
   */
  void print(BTreeFile::BlockNumber blockNumber) const;

  /* Print info about the the tree (primarily for debugging).
   */
  void printInfo() const;

  /* Print stats about file-level operations on the tree.
   */
  void printStats() const;


  /* Destructor
   */
  ~BTree();

 private:
  /* countSubtree() is a recursive helper for count().
   *
   * Parameter:
   *   blockNum - block number of subtree to count keys of
   *
   * Return: the number of keys in the subtree rooted at the specified
   *   block number.
   */
  int countSubtree(BTreeFile::BlockNumber blockNum) const;

  /* Private recursive helper for insert()
   * Inserts key and value into the subtree rooted at root (a block number).
   * If this results in a split of the root, 
   * "return" the promoted key and value in the output arguments,
   * and actually returns the new right child block number.
   * If there is no split in the root, return 0 for no new block.
   */
  BTreeFile::BlockNumber insertHelper(string key, string value,
                                        BTreeFile::BlockNumber root,
                                        string &promotedKey,
                                        string &promotedValue);

  void findSibling(BTreeFile::BlockNumber parentNumber);

  BTreeFile & _file;
};

#endif
