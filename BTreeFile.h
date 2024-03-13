/*
 * BTreeFile.h - declaration of the class BTreeFile, used as part
 * of actually implementing a B Tree.  CPS222 project 4
 *
 * This class manages a file storing BTreeBlock objects on disk.  In addition
 * to storing the blocks themselves, it stores the root block number.  It
 * also tracks which blocks are free, and provides allocateBlock and
 * deallocateBlock for getting and releasing free blocks.
 *
 * Blocks are numbered 1, 2, 3, ...  There is no block 0, so 0 has the same
 * function for a tree on disk as NULL and nullptr have for a tree in main
 * memory.  A const is defined for identifying it: BTreeFile::NONE.
 *
 * Any variable holding a block number must be declared as type BlockNumber.
 *
 * When a new BTreeFile is created, it has no root.  The user of this
 * class must create a root node BTreeBlock, get a BlockNumber for it
 * with allocateBlock(), and use setRoot() to make it the root.
 *
 * getBlock() and putBlock() are used to read and write BTreeBlocks
 * from and to the file.
 *
 * Copyright (c) 2001, 2002, 2003 - Russell C. Bjork
 * Updates copyright (c) 2016, 2019 by Russ Tuck
 */

#ifndef BTREE_FILE_H
#define BTREE_FILE_H

#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <iomanip>
using std::setw;

/* DEGREE: maximum number of children a node in the tree can have on disk.
 *    A node can have up to DEGREE - 1 keys and their values.
 *    This may be changed with an option on the compiler command line.
 */
#ifndef DEGREE
#define DEGREE 7
#endif

/* Maximum key length, in ASCII characters. */
#define MAX_KEY_LENGTH 15

/* Maximum value length, in ASCII characters. */
#define MAX_VALUE_LENGTH 31

// Declare BTreeBlock as a class, these classes mutually refer to each other.
class BTreeBlock;


/* Each BTree is associated with an object of this class, which provides
 * access to the disk file in which the BTree is stored.
 */
class BTreeFile
{
 public:

  /* Individual blocks in the file are referred to by block number
   * (1 .. size of file)
   */
  typedef unsigned BlockNumber;

  /* Define a constant for the null block. */
  static const BlockNumber NONE = 0;


  /* Constructor.  This actually creates the access path to the file.
   *
   * Parameter:
   *   name - has the same meaning as in the BTree constructor
   */
  BTreeFile(string name);


  /* Accessor returns name given to constructor. */
  string getName() const;


  /* Return: block number of root of the tree - 0 means the tree is empty
   *   (has no root)
   */
  BlockNumber getRoot() const;


  /* Set or change the root block of the tree
   *
   * Parameter:
   *   root - make this the new root block number.
   */
  void setRoot(BlockNumber root);


  /* Read a BTree block from disk into an in-memory block buffer.
   *
   * Parameters:
   *   number - block number of block in file to read;  must be in
   *      the range 1 .. size of the disk file (in blocks)
   *   block - an "empty" BTreeBlock object to be filled with the
   *      block from disk.  This is passed by reference, so getBlock
   *      can fill it with data from disk.
   *
   * Return: true if the operation succeeds, false if it fails
   *   (e.g. due to an invalid block number or some IO system error.)
   */
  bool getBlock(BlockNumber number, BTreeBlock & block);


  /* Write a BTree block from an in-memory block buffer to disk.
   *
   * Parameters:
   *   number - block number of block in file to write;  must be in
   *      the range 1 .. size of the disk file (in blocks)
   *   block - a "full" BTreeBlock object to be written to a
   *      block on disk.  This is passed by reference, for efficiency
   *      in order to avoid an in-memory copy, but is declared const
   *      because putBlock() will not modify it.
   *
   * Return: true if the operation succeeds, false if it fails
   *   (e.g. due to an invalid block number or some IO system error.)
   */
  bool putBlock(BlockNumber number, const BTreeBlock & block);


  /* Allocate a block to be used for a new node in the tree.
   *
   * Return: block number allocated
   */
  BlockNumber allocateBlock();


  /* Deallocate a block that was used for a node in the tree,
   * but is not needed anymore.
   *
   * Parameter:
   *   number - block number to be deallocated
   */
  void deallocateBlock(BlockNumber number);


  /* Print header information to cout. */
  void printHeaderInfo() const;

  /* Print statistics about operations to cout. */
  void printStats() const;

  /* Print information in a specific block to cout. If recursive is true,
   * level is printed out, and controls the indentation of the block.
   * Further, if recursive is true, the child blocks are printed out as
   * well.  If recursive is false, only the specified block is printed,
   * and level is ignored.  (This method is provided to support the
   * print() methods in BTree.h that in turn support testing/debugging/)
   */
  void printBlock(BlockNumber number, bool recursive, unsigned level);

  // Destructor - print out statistics
  ~BTreeFile();

 private:

  string _name;	                // File name
  unsigned _degree;		// Degree of tree
  BlockNumber _root;		// Number of root block
  BlockNumber _lastUsed;	// Number of the last block currently in use
  BlockNumber _free;		// Pointer to a linked list of recycled blocks
  FILE * _stream;		// The stdio stream actually used

  /* The following private class represents the way data is
   * physically stored on the disk.
   */
  class PhysicalBlock;

  /* Actually physically transfer data to/from disk.
   * Return true if successful.  (False typically indicates use
   * of an invalid block number, or some sort of IO system
   * failure.)
   */
  bool getBlock(BlockNumber number, PhysicalBlock & buffer);
  bool putBlock(BlockNumber number, const PhysicalBlock & buffer);

  /* Update copy of header on disk */
  void updateHeader();

  /* Counters to keep track of statistics for testing */
  unsigned _totalGets, _totalPuts,
    _headerGets, _headerPuts,
    _freeGets, _freePuts;
};

#endif
