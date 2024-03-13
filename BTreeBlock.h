/*
 * BTreeBlock.h - declaration of the class BTreeBlock, used as part
 * of actually implementing a B Tree.  CPS222 project 4
 *
 * An object of class BTreeBlock is used to hold an in-memory copy of
 * one block of the BTree, and provides methods for manipulating it.
 *
 * The method BTreeFile::getBlock() is used to copy a block from the
 * disk file to a Block object; and the method BTreeFile::putBlock()
 * is used to copy a Block object back to the file.
 *
 * Although a block on disk can hold only DEGREE - 1 keys and
 * associated values, and can store DEGREE block numbers of child
 * blocks (see BTreeFile.h), an in-memory copy is allowed to hold one
 * more key, value, and child than this to facilitate splitting.  Of
 * course, an over-full in-memory copy will never be read as such from
 * disk, nor can it be transferred to disk before it is split.
 *
 * Copyright (c) 2001, 2002, 2003, 2015 - Russell C. Bjork
 * Updates Copyright (c) 2016, 2018, 2019 by Russ Tuck
 */

#ifndef BTREE_BLOCK_H
#define BTREE_BLOCK_H

#include <string>
#include "BTreeFile.h"


class BTreeBlock
{
 public:

  /* Constructor - create an empty block. */
  BTreeBlock();

  /* Test to see whether this is a leaf block.
   *
   * Return: true iff it is a leaf
   */
  bool isLeaf() const;


  /* Accessors for the various instance variables.
   *
   * Shared Parameter:
   *   index - index into array of keys, values, children
   *      For getKey() and getValue(), valid values of index lie
   *      in the range 0 .. number of keys - 1
   *      For getChild(), valid values of index lie in the range
   *      0 .. number of keys, since there is always one more child
   *      than keys.
   *
   * getNumberOfKeys() returns the number of keys in this block.
   * getKey() returns the key identified by index
   * getValue() returns the value identified by index
   * getChild() returns the child block number identified by index
   */
  unsigned getNumberOfKeys() const;
  string getKey(unsigned index) const;
  string getValue(unsigned index) const;
  BTreeFile::BlockNumber getChild(unsigned index) const;

  /* Mutators for the various instance variables
   *
   * Parameter:
   *   index - index into array of keys, values, children
   *     (same constraints on value of index as for get___ methods)
   *   key/value/child - new value to put in specified slot
   */
  void setNumberOfKeys(unsigned numberOfKeys);
  void setKey(unsigned index, string key);
  void setValue(unsigned index, string value);
  void setChild(unsigned index, BTreeFile::BlockNumber child);

  /* Find the position where a given key is, or belongs.
   *
   * Parameter:
   *   key - the key whose position is to be found
   *
   * Return:
   *   If the key occurs in the block, its index.
   *   If it does not occur, where it would belong. (If it is equal to the
   *     number of keys, this indicates it is greater than any key
   *     currently in the block.) If the block has children, this value
   *     is the index of the child to look for it in.
   */
  unsigned getPosition(string key);

  /* Insert a new key, value, and associated child into the block,
   * pushing existing entries to the right if need be, and incrementing
   * number of keys.  This may make the block over-full (too full to be
   * written to disk), and in need of splitting.
   *
   * Parameters:
   *   position - position where the new key and value go; the new child
   *       will go one to right of this.
   *   key, value, child - the new entry to insert. Child will go to the
   *       right of key and value
   */
  void insert(unsigned position, string key, string value,
              BTreeFile::BlockNumber child);

  /* Remove a key, value, and associated child from the block,
   * pulling remaining entries to the left if need be, and decrementing
   * number of keys. This may make the block too small to be written to disk, 
   * and in need of borrowing the key and the value from its neighbor or merging.
   */
  void remove(unsigned position);

  /* Test to see whether this block needs to be split following an insert.
   * A split will be needed if the block now contains too many keys.
   */
  bool splitNeeded() const;

  /* Split this block in two.  This should be done _after_ an
   * insertion that results in the block becoming overfull, so that
   * the newly-inserted entry ends up in the correct half.
   *
   * The left half of the original block is left in place, but the
   * count of the number of keys is reduced; the right half of the
   * original block is copied to a new block, and the number of keys
   * in that half is set appropriately.  The caller must provide an
   * empty block for the right half.
   *
   * The key and value to be promoted into the block's parent are
   * returned in pass-by-reference parameters.
   *
   * Parameters:
   *   promotedKey, promotedValue - variables into which the promoted
   *       key and value are placed.  They should be put in the parent
   *       block, or in a new root if there is no parent.
   *   rightHalf - block into which half the information is placed
   */
  void split(string & promotedKey, string & promotedValue,
             BTreeBlock & rightHalf);

 private:

  unsigned _numberOfKeys;
  string _key[DEGREE];
  string _value[DEGREE];
  BTreeFile::BlockNumber _child[DEGREE + 1];
};

# endif
