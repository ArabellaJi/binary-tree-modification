/*
 * BTreeBlock.cc - implementation of methods of class BTreeBlock
 * as declared in BTreeBlock.h
 *
 * Copyright (c) 2001, 2002, 2003, 2015 - Russell C. Bjork
 * Updates copyright (c) 2016, 2019 - Russ Tuck
 */

#include "BTreeBlock.h"
#include "BTreeFile.h"

BTreeBlock::BTreeBlock()
  : _numberOfKeys(0)
{ }

bool BTreeBlock::isLeaf() const {
  return _child[0] == BTreeFile::NONE;
}

unsigned BTreeBlock::getNumberOfKeys() const {
  return _numberOfKeys;
}

string BTreeBlock::getKey(unsigned index) const {
  return _key[index];
}

string BTreeBlock::getValue(unsigned index) const {
  return _value[index];
}

BTreeFile::BlockNumber BTreeBlock::getChild(unsigned index) const {
  return _child[index];
}

void BTreeBlock::setNumberOfKeys(unsigned numberOfKeys) {
  _numberOfKeys = numberOfKeys;
}

void BTreeBlock::setKey(unsigned index, string key) {
  _key[index] = key;
}

void BTreeBlock::setValue(unsigned index, string value) {
  _value[index] = value;
}

void BTreeBlock::setChild(unsigned index, BTreeFile::BlockNumber child) {
  _child[index] = child;
}

unsigned BTreeBlock::getPosition(string key) {
  unsigned position = 0;
  while (position < _numberOfKeys && _key[position] < key) {
    position ++;
  }
  return position;
}

void BTreeBlock::insert(unsigned position, string key, string value,
                        BTreeFile::BlockNumber child)
{
  for (unsigned newPosition = _numberOfKeys; newPosition > position;
       newPosition--) {
    _key[newPosition] = _key[newPosition - 1];
    _value[newPosition] = _value[newPosition - 1];
    _child[newPosition + 1] = _child[newPosition];
  }
  _key[position] = key;
  _value[position] = value;
  _child[position + 1] = child;
  _numberOfKeys ++;
}

void BTreeBlock::remove(unsigned position)
{
  for (unsigned newPosition = position + 1; newPosition < _numberOfKeys;
       ++newPosition) {
    _key[newPosition - 1] = _key[newPosition];
    _value[newPosition - 1] = _value[newPosition];
    _child[newPosition] = _child[newPosition + 1];
  }
  _numberOfKeys --;
}

bool BTreeBlock::splitNeeded() const {
  return _numberOfKeys >= DEGREE;
}

void BTreeBlock::split(string & promotedKey, string & promotedValue,
                       BTreeBlock & rightHalf) {
  int promotedPosition = DEGREE / 2;
  // Correct for whether DEGREE is odd or even - if odd equivalent to
  // ceil(DEGREE / 2) - 1; if even equivalent to ceil(DEGREE / 2)
  promotedKey = _key[promotedPosition];
  promotedValue = _value[promotedPosition];
  for (unsigned i = promotedPosition + 1; i < _numberOfKeys; i ++) {
    rightHalf._key[i - promotedPosition - 1] = _key[i];
    rightHalf._value[i - promotedPosition - 1] = _value[i];
    rightHalf._child[i - promotedPosition - 1] = _child[i];
  }
  rightHalf._numberOfKeys = _numberOfKeys - promotedPosition - 1;
  rightHalf._child[rightHalf._numberOfKeys] = _child[_numberOfKeys];
  _numberOfKeys = promotedPosition;
}
