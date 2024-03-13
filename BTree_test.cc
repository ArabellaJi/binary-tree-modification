/* Unit tests for BTree class: CPS 222 Project 4.
 * Copyright (c) 2017, 2018, 2019 - Russ Tuck
 */

#include "gtest/gtest.h"

#include <string>
using std::string;

#include "BTree.h"

#define BTREE_TMP_FILE "tmp_unittest.btree"
#define TEST_FILE "test.tree"

// Windows doesn't delete an open file, even when it's closed later.
// That's why it's helpful to delete the temp file at the beginning
// of every test, instead of just at the end.

TEST(empty, lookupEmpty) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_FALSE(bt.lookup("EMPTY", value));

  remove(BTREE_TMP_FILE);
}

TEST(root1, insert) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_FALSE(bt.lookup("Cat", value));

  bt.insert("Cat", "Charlene");
  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Charlene");

  EXPECT_FALSE(bt.lookup("MISSING", value));
  remove(BTREE_TMP_FILE);
}

TEST(root1, replace) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Cat", "Kitty");

  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Kitty");

  remove(BTREE_TMP_FILE);
}

TEST(empty, count0) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_EQ(bt.count(), 0);

  remove(BTREE_TMP_FILE);
}

TEST(root1, count1) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Cat", "Kitty");

  EXPECT_EQ(bt.count(), 1);

  remove(BTREE_TMP_FILE);
}

TEST(empty, depth) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_EQ(bt.depth("EMPTY"), -1);

  remove(BTREE_TMP_FILE);
}

TEST(root1, depth0) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  EXPECT_EQ(bt.depth("Cat"), 0);

  remove(BTREE_TMP_FILE);
}

TEST(empty, remove) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_FALSE(bt.remove("EMPTY"));

  remove(BTREE_TMP_FILE);
}

TEST(root1, removeToEmpty) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.remove("Cat");

  EXPECT_EQ(bt.count(), 0);
  EXPECT_EQ(bt.depth("EMPTY"), -1);

  remove(BTREE_TMP_FILE);
}

#if DEGREE == 7

TEST(root6, insert) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);
  EXPECT_FALSE(bt.lookup("Cat", value));
  EXPECT_FALSE(bt.lookup("Fox", value));
  EXPECT_FALSE(bt.lookup("Frog", value));

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Buffalo", "Bill");

  EXPECT_FALSE(bt.lookup("Frog", value));
  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Charlene");
  EXPECT_TRUE(bt.lookup("Buffalo", value));
  EXPECT_EQ(value, "Bill");
  EXPECT_TRUE(bt.lookup("Dog", value));
  EXPECT_EQ(value, "Donna");
  EXPECT_TRUE(bt.lookup("Fox", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Aardvark", value));
  EXPECT_EQ(value, "Anthony");
  EXPECT_TRUE(bt.lookup("Elephant", value));
  EXPECT_EQ(value, "Emily");

  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(root6, remove) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Buffalo", "Bill");

  bt.remove("Cat");
  bt.remove("Dog");
  bt.remove("Elephant");
  bt.remove("Buffalo");

  EXPECT_FALSE(bt.lookup("Frog", value));
  EXPECT_FALSE(bt.lookup("Cat", value));
  EXPECT_FALSE(bt.lookup("Buffalo", value));
  EXPECT_FALSE(bt.lookup("Dog", value));
  EXPECT_TRUE(bt.lookup("Fox", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Aardvark", value));
  EXPECT_EQ(value, "Anthony");
  EXPECT_FALSE(bt.lookup("Elephant", value));

  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(root6, count) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);
  EXPECT_FALSE(bt.lookup("Cat", value));
  EXPECT_FALSE(bt.lookup("Fox", value));
  EXPECT_FALSE(bt.lookup("Frog", value));

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Buffalo", "Bill");

  EXPECT_EQ(bt.count(), 6);

  remove(BTREE_TMP_FILE);
}

TEST(twonode, depth0to1) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Buffalo", "Bill");

  // Should all still fit in root node
  EXPECT_EQ(bt.depth("Cat"), 0);
  EXPECT_EQ(bt.depth("Buffalo"), 0);
  EXPECT_EQ(bt.depth("Dog"), 0);
  EXPECT_EQ(bt.depth("Fox"), 0);
  EXPECT_EQ(bt.depth("Aardvark"), 0);
  EXPECT_EQ(bt.depth("Elephant"), 0);

  // bt.print();  // uncomment if needed to debug test

  // Should cause root to split, creating new level
  bt.insert("Zebra", "Zelda");

  EXPECT_EQ(bt.depth("Aardvark"), 1);
  EXPECT_EQ(bt.depth("Buffalo"), 1);
  EXPECT_EQ(bt.depth("Cat"), 1);
  EXPECT_EQ(bt.depth("Dog"), 0);
  EXPECT_EQ(bt.depth("Elephant"), 1);
  EXPECT_EQ(bt.depth("Fox"), 1);
  EXPECT_EQ(bt.depth("Zebra"), 1);

  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(twolevel, insert15) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_FALSE(bt.lookup("Cat", value));
  EXPECT_FALSE(bt.lookup("Fox", value));
  EXPECT_FALSE(bt.lookup("Abalone", value));
  EXPECT_FALSE(bt.lookup("Frog", value));

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kangaroo", "Karl");


  EXPECT_TRUE(bt.lookup("Gopher", value));
  EXPECT_EQ(value, "Gertrude");
  EXPECT_TRUE(bt.lookup("Albacore", value));
  EXPECT_EQ(value, "Arianne");
  EXPECT_TRUE(bt.lookup("Bison", value));
  EXPECT_EQ(value, "Boris");
  EXPECT_TRUE(bt.lookup("Cow", value));
  EXPECT_EQ(value, "Clarabelle");
  EXPECT_TRUE(bt.lookup("Horse", value));
  EXPECT_EQ(value, "Horace");
  EXPECT_TRUE(bt.lookup("Jackal", value));
  EXPECT_EQ(value, "Jolene");
  EXPECT_TRUE(bt.lookup("Kangaroo", value));
  EXPECT_EQ(value, "Karl");

  EXPECT_FALSE(bt.lookup("Frog", value));

  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Charlene");
  EXPECT_TRUE(bt.lookup("Buffalo", value));
  EXPECT_EQ(value, "Bill");
  EXPECT_TRUE(bt.lookup("Dog", value));
  EXPECT_EQ(value, "Donna");
  EXPECT_TRUE(bt.lookup("Fox", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Aardvark", value));
  EXPECT_EQ(value, "Anthony");
  EXPECT_TRUE(bt.lookup("Elephant", value));
  EXPECT_EQ(value, "Emily");
  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(twolevel, remove) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  EXPECT_FALSE(bt.lookup("Cat", value));
  EXPECT_FALSE(bt.lookup("Fox", value));
  EXPECT_FALSE(bt.lookup("Abalone", value));
  EXPECT_FALSE(bt.lookup("Frog", value));

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");

  bt.remove("Aardvark");
  bt.remove("Albacore");
  bt.remove("Cow");
  bt.remove("Elephant");
  bt.remove("Horse");
  bt.remove("Jackal");

  EXPECT_TRUE(bt.lookup("Gopher", value));
  EXPECT_EQ(value, "Gertrude");
  EXPECT_FALSE(bt.lookup("Albacore", value));
  EXPECT_TRUE(bt.lookup("Bison", value));
  EXPECT_EQ(value, "Boris");
  EXPECT_FALSE(bt.lookup("Cow", value));
  EXPECT_FALSE(bt.lookup("Horse", value));
  EXPECT_FALSE(bt.lookup("Jackal", value));

  EXPECT_FALSE(bt.lookup("Frog", value));

  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Charlene");
  EXPECT_TRUE(bt.lookup("Buffalo", value));
  EXPECT_EQ(value, "Bill");
  EXPECT_TRUE(bt.lookup("Dog", value));
  EXPECT_EQ(value, "Donna");
  EXPECT_TRUE(bt.lookup("Fox", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_FALSE(bt.lookup("Aardvark", value));
  EXPECT_FALSE(bt.lookup("Elephant", value));
  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(twolevel, count) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kangaroo", "Karl");

  EXPECT_EQ(bt.count(), 15);

  bt.insert("Cat", "C");
  bt.insert("Aardvark", "A");
  bt.insert("Dog", "D");
  bt.insert("Elephant", "E");
  bt.insert("Fox", "F");
  bt.insert("Gopher", "G");
  bt.insert("Buffalo", "B");
  bt.insert("Albacore", "A");
  bt.insert("Bison", "B");
  bt.insert("Cow", "C");
  bt.insert("Abalone", "A");
  bt.insert("Horse", "H");
  bt.insert("Iguana", "I");
  bt.insert("Jackal", "J");
  bt.insert("Kangaroo", "K");

  EXPECT_EQ(bt.count(), 15);

  remove(BTREE_TMP_FILE);
}

TEST(threelevel, depth1to2) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kanagaroo", "Karl");
  bt.insert("Ant", "Albert");
  bt.insert("Anteater", "Alicia");
  bt.insert("Bat", "Barry");
  bt.insert("Bear", "Beatrice");
  bt.insert("Camel", "Carlos");
  bt.insert("Canary", "Caroline");
  bt.insert("Catbird", "Charles");
  bt.insert("Chickadee", "Chester");
  bt.insert("Monkey", "Mary");
  bt.insert("Mouse", "Mickey");
  bt.insert("Llama", "Larry");
  bt.insert("Lion", "Leo");
  bt.insert("Emu", "Emil");
  bt.insert("Goat", "Geraldine");
  bt.insert("Flea", "Francine");
  bt.insert("Frog", "Fred");

  // Just before node split at depth 1 causes a new root level
  EXPECT_EQ(bt.depth("Abalone"), 1);
  EXPECT_EQ(bt.depth("Albacore"), 1);
  EXPECT_EQ(bt.depth("Ant"), 1);
  EXPECT_EQ(bt.depth("Anteater"), 1);
  EXPECT_EQ(bt.depth("Bat"), 1);
  EXPECT_EQ(bt.depth("Bear"), 1);
  EXPECT_EQ(bt.depth("Bison"), 0);
  EXPECT_EQ(bt.depth("Buffalo"), 1);
  EXPECT_EQ(bt.depth("Camel"), 1);
  EXPECT_EQ(bt.depth("Canary"), 1);
  EXPECT_EQ(bt.depth("Cat"), 0);
  EXPECT_EQ(bt.depth("Catbird"), 1);
  EXPECT_EQ(bt.depth("Chickadee"), 1);
  EXPECT_EQ(bt.depth("Cow"), 1);
  EXPECT_EQ(bt.depth("Dog"), 0);
  EXPECT_EQ(bt.depth("Elephant"), 1);
  EXPECT_EQ(bt.depth("Emu"), 1);
  EXPECT_EQ(bt.depth("Flea"), 1);
  EXPECT_EQ(bt.depth("Fox"), 0);
  EXPECT_EQ(bt.depth("Frog"), 1);
  EXPECT_EQ(bt.depth("Goat"), 1);
  EXPECT_EQ(bt.depth("Gopher"), 1);
  EXPECT_EQ(bt.depth("Horse"), 0);
  EXPECT_EQ(bt.depth("Iguana"), 1);
  EXPECT_EQ(bt.depth("Jackal"), 1);
  EXPECT_EQ(bt.depth("Kanagaroo"), 1);
  EXPECT_EQ(bt.depth("Lion"), 0);
  EXPECT_EQ(bt.depth("Llama"), 1);
  EXPECT_EQ(bt.depth("Monkey"), 1);
  EXPECT_EQ(bt.depth("Mouse"), 1);

  //bt.print();  // uncomment if needed to debug test

  // Split that node and create a new level
  bt.insert("Aardvark", "Anthony");

  // nice even 3-level tree (depth 0 to 2)
  EXPECT_EQ(bt.depth("Aardvark"), 2);
  EXPECT_EQ(bt.depth("Abalone"), 2);
  EXPECT_EQ(bt.depth("Albacore"), 2);
  EXPECT_EQ(bt.depth("Ant"), 1);
  EXPECT_EQ(bt.depth("Anteater"), 2);
  EXPECT_EQ(bt.depth("Bat"), 2);
  EXPECT_EQ(bt.depth("Bear"), 2);
  EXPECT_EQ(bt.depth("Bison"), 1);
  EXPECT_EQ(bt.depth("Buffalo"), 2);
  EXPECT_EQ(bt.depth("Camel"), 2);
  EXPECT_EQ(bt.depth("Canary"), 2);
  EXPECT_EQ(bt.depth("Cat"), 1);
  EXPECT_EQ(bt.depth("Catbird"), 2);
  EXPECT_EQ(bt.depth("Chickadee"), 2);
  EXPECT_EQ(bt.depth("Cow"), 2);
  EXPECT_EQ(bt.depth("Dog"), 0);
  EXPECT_EQ(bt.depth("Elephant"), 2);
  EXPECT_EQ(bt.depth("Emu"), 2);
  EXPECT_EQ(bt.depth("Flea"), 2);
  EXPECT_EQ(bt.depth("Fox"), 1);
  EXPECT_EQ(bt.depth("Frog"), 2);
  EXPECT_EQ(bt.depth("Goat"), 2);
  EXPECT_EQ(bt.depth("Gopher"), 2);
  EXPECT_EQ(bt.depth("Horse"), 1);
  EXPECT_EQ(bt.depth("Iguana"), 2);
  EXPECT_EQ(bt.depth("Jackal"), 2);
  EXPECT_EQ(bt.depth("Kanagaroo"), 2);
  EXPECT_EQ(bt.depth("Lion"), 1);
  EXPECT_EQ(bt.depth("Llama"), 2);
  EXPECT_EQ(bt.depth("Monkey"), 2);
  EXPECT_EQ(bt.depth("Mouse"), 2);

  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(threelevel, insert31) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kanagaroo", "Karl");
  bt.insert("Ant", "Albert");
  bt.insert("Anteater", "Alicia");
  bt.insert("Bat", "Barry");
  bt.insert("Bear", "Beatrice");
  bt.insert("Camel", "Carlos");
  bt.insert("Canary", "Caroline");
  bt.insert("Catbird", "Charles");
  bt.insert("Chickadee", "Chester");
  bt.insert("Monkey", "Mary");
  bt.insert("Mouse", "Minnie");
  bt.insert("Llama", "Larry");
  bt.insert("Lion", "Leo");
  bt.insert("Emu", "Emil");
  bt.insert("Goat", "Geraldine");
  bt.insert("Flea", "Francine");
  bt.insert("Frog", "Fred");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Mouse", "Mickey");     // Duplicate key, update value

  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Charlene");
  EXPECT_TRUE(bt.lookup("Dog", value));
  EXPECT_EQ(value, "Donna");
  EXPECT_TRUE(bt.lookup("Elephant", value));
  EXPECT_EQ(value, "Emily");
  EXPECT_TRUE(bt.lookup("Fox", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Gopher", value));
  EXPECT_EQ(value, "Gertrude");
  EXPECT_TRUE(bt.lookup("Buffalo", value));
  EXPECT_EQ(value, "Bill");
  EXPECT_TRUE(bt.lookup("Albacore", value));
  EXPECT_EQ(value, "Arianne");
  EXPECT_TRUE(bt.lookup("Bison", value));
  EXPECT_EQ(value, "Boris");
  EXPECT_TRUE(bt.lookup("Cow", value));
  EXPECT_EQ(value, "Clarabelle");
  EXPECT_TRUE(bt.lookup("Abalone", value));
  EXPECT_EQ(value, "Antoinette");
  EXPECT_TRUE(bt.lookup("Horse", value));
  EXPECT_EQ(value, "Horace");
  EXPECT_TRUE(bt.lookup("Iguana", value));
  EXPECT_EQ(value, "Ignatius");
  EXPECT_TRUE(bt.lookup("Jackal", value));
  EXPECT_EQ(value, "Jolene");
  EXPECT_TRUE(bt.lookup("Kanagaroo", value));
  EXPECT_EQ(value, "Karl");
  EXPECT_TRUE(bt.lookup("Ant", value));
  EXPECT_EQ(value, "Albert");
  EXPECT_TRUE(bt.lookup("Anteater", value));
  EXPECT_EQ(value, "Alicia");
  EXPECT_TRUE(bt.lookup("Bat", value));
  EXPECT_EQ(value, "Barry");
  EXPECT_TRUE(bt.lookup("Bear", value));
  EXPECT_EQ(value, "Beatrice");
  EXPECT_TRUE(bt.lookup("Camel", value));
  EXPECT_EQ(value, "Carlos");
  EXPECT_TRUE(bt.lookup("Canary", value));
  EXPECT_EQ(value, "Caroline");
  EXPECT_TRUE(bt.lookup("Catbird", value));
  EXPECT_EQ(value, "Charles");
  EXPECT_TRUE(bt.lookup("Chickadee", value));
  EXPECT_EQ(value, "Chester");
  EXPECT_TRUE(bt.lookup("Monkey", value));
  EXPECT_EQ(value, "Mary");
  EXPECT_TRUE(bt.lookup("Mouse", value));
  EXPECT_EQ(value, "Mickey");
  EXPECT_TRUE(bt.lookup("Llama", value));
  EXPECT_EQ(value, "Larry");
  EXPECT_TRUE(bt.lookup("Lion", value));
  EXPECT_EQ(value, "Leo");
  EXPECT_TRUE(bt.lookup("Emu", value));
  EXPECT_EQ(value, "Emil");
  EXPECT_TRUE(bt.lookup("Goat", value));
  EXPECT_EQ(value, "Geraldine");
  EXPECT_TRUE(bt.lookup("Flea", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Frog", value));
  EXPECT_EQ(value, "Fred");
  EXPECT_TRUE(bt.lookup("Aardvark", value));
  EXPECT_EQ(value, "Anthony");

  EXPECT_FALSE(bt.lookup("Aardvarkeee", value));
  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

TEST(threelevel, count) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kangaroo", "Karl");
  bt.insert("Ant", "Albert");
  bt.insert("Anteater", "Alicia");
  bt.insert("Bat", "Barry");
  bt.insert("Bear", "Beatrice");
  bt.insert("Camel", "Carlos");
  bt.insert("Canary", "Caroline");
  bt.insert("Catbird", "Charles");
  bt.insert("Chickadee", "Chester");
  bt.insert("Monkey", "Mary");
  bt.insert("Mouse", "Minnie");
  bt.insert("Mouse", "Mickey");  // duplicate key, update value
  bt.insert("Llama", "Larry");
  bt.insert("Lion", "Leo");
  bt.insert("Emu", "Emil");
  bt.insert("Goat", "Geraldine");
  bt.insert("Flea", "Francine");
  bt.insert("Frog", "Fred");
  bt.insert("Aardvark", "Anthony");

  EXPECT_EQ(bt.count(), 31);

  bt.insert("Cat", "C");
  bt.insert("Aardvark", "A");
  bt.insert("Dog", "D");
  bt.insert("Elephant", "E");
  bt.insert("Fox", "F");
  bt.insert("Gopher", "G");
  bt.insert("Buffalo", "B");
  bt.insert("Albacore", "A");
  bt.insert("Bison", "B");
  bt.insert("Cow", "C");
  bt.insert("Abalone", "A");
  bt.insert("Horse", "H");
  bt.insert("Iguana", "I");
  bt.insert("Jackal", "J");
  bt.insert("Kangaroo", "K");
  bt.insert("Ant", "A");
  bt.insert("Anteater", "A");
  bt.insert("Bat", "B");
  bt.insert("Bear", "Bea");
  bt.insert("Camel", "Carl");
  bt.insert("Canary", "Carol");
  bt.insert("Catbird", "Charlie");
  bt.insert("Chickadee", "Ches");
  bt.insert("Monkey", "M");
  bt.insert("Llama", "L");
  bt.insert("Lion", "L");
  bt.insert("Emu", "E");
  bt.insert("Goat", "Gerald");
  bt.insert("Flea", "France");
  bt.insert("Frog", "F");
  bt.insert("Aardvark", "Ant");

  EXPECT_EQ(bt.count(), 31);
  // bt.print();  // uncomment if needed to debug test

  remove(BTREE_TMP_FILE);
}

TEST(threelevel, depth) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kanagaroo", "Karl");
  bt.insert("Ant", "Albert");
  bt.insert("Anteater", "Alicia");
  bt.insert("Bat", "Barry");
  bt.insert("Bear", "Beatrice");
  bt.insert("Camel", "Carlos");
  bt.insert("Canary", "Caroline");
  bt.insert("Catbird", "Charles");
  bt.insert("Chickadee", "Chester");
  bt.insert("Monkey", "Mary");
  bt.insert("Mouse", "Mickey");
  bt.insert("Llama", "Larry");
  bt.insert("Lion", "Leo");
  bt.insert("Emu", "Emil");
  bt.insert("Goat", "Geraldine");
  bt.insert("Flea", "Francine");
  bt.insert("Frog", "Fred");
  bt.insert("Aardvark", "Anthony");

  // nice even 3-level tree
  EXPECT_EQ(bt.depth("Aardvark"), 2);
  EXPECT_EQ(bt.depth("Abalone"), 2);
  EXPECT_EQ(bt.depth("Albacore"), 2);
  EXPECT_EQ(bt.depth("Ant"), 1);
  EXPECT_EQ(bt.depth("Anteater"), 2);
  EXPECT_EQ(bt.depth("Bat"), 2);
  EXPECT_EQ(bt.depth("Bear"), 2);
  EXPECT_EQ(bt.depth("Bison"), 1);
  EXPECT_EQ(bt.depth("Buffalo"), 2);
  EXPECT_EQ(bt.depth("Camel"), 2);
  EXPECT_EQ(bt.depth("Canary"), 2);
  EXPECT_EQ(bt.depth("Cat"), 1);
  EXPECT_EQ(bt.depth("Catbird"), 2);
  EXPECT_EQ(bt.depth("Chickadee"), 2);
  EXPECT_EQ(bt.depth("Cow"), 2);
  EXPECT_EQ(bt.depth("Dog"), 0);
  EXPECT_EQ(bt.depth("Elephant"), 2);
  EXPECT_EQ(bt.depth("Emu"), 2);
  EXPECT_EQ(bt.depth("Flea"), 2);
  EXPECT_EQ(bt.depth("Fox"), 1);
  EXPECT_EQ(bt.depth("Frog"), 2);
  EXPECT_EQ(bt.depth("Goat"), 2);
  EXPECT_EQ(bt.depth("Gopher"), 2);
  EXPECT_EQ(bt.depth("Horse"), 1);
  EXPECT_EQ(bt.depth("Iguana"), 2);
  EXPECT_EQ(bt.depth("Jackal"), 2);
  EXPECT_EQ(bt.depth("Kanagaroo"), 2);
  EXPECT_EQ(bt.depth("Lion"), 1);
  EXPECT_EQ(bt.depth("Llama"), 2);
  EXPECT_EQ(bt.depth("Monkey"), 2);
  EXPECT_EQ(bt.depth("Mouse"), 2);

  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}


// Insert enough keys to promote 2nd key into root of 3-level tree.
// This covers splitting a non-leaf without adding a new level.
TEST(threelevel, insertLots) {
  string value;
  remove(BTREE_TMP_FILE);    // delete any old version (just in case)
  BTree bt(BTREE_TMP_FILE);

  bt.insert("Cat", "Charlene");
  bt.insert("Dog", "Donna");
  bt.insert("Elephant", "Emily");
  bt.insert("Fox", "Francine");
  bt.insert("Gopher", "Gertrude");
  bt.insert("Buffalo", "Bill");
  bt.insert("Albacore", "Arianne");
  bt.insert("Bison", "Boris");
  bt.insert("Cow", "Clarabelle");
  bt.insert("Abalone", "Antoinette");
  bt.insert("Horse", "Horace");
  bt.insert("Iguana", "Ignatius");
  bt.insert("Jackal", "Jolene");
  bt.insert("Kanagaroo", "Karl");
  bt.insert("Ant", "Albert");
  bt.insert("Anteater", "Alicia");
  bt.insert("Bat", "Barry");
  bt.insert("Bear", "Beatrice");
  bt.insert("Camel", "Carlos");
  bt.insert("Canary", "Caroline");
  bt.insert("Catbird", "Charles");
  bt.insert("Chickadee", "Chester");
  bt.insert("Monkey", "Mary");
  bt.insert("Mouse", "Minnie");
  bt.insert("Llama", "Larry");
  bt.insert("Lion", "Leo");
  bt.insert("Emu", "Emil");
  bt.insert("Goat", "Geraldine");
  bt.insert("Flea", "Francine");
  bt.insert("Frog", "Fred");
  bt.insert("Aardvark", "Anthony");
  bt.insert("Mouse", "Mickey");     // Duplicate key, update value

  bt.insert("zCat", "zCharlene");
  bt.insert("zDog", "zDonna");
  bt.insert("zElephant", "zEmily");
  bt.insert("zFox", "zFrancine");
  bt.insert("zGopher", "zGertrude");
  bt.insert("zBuffalo", "zBill");
  bt.insert("zAlbacore", "zArianne");
  bt.insert("zBison", "zBoris");
  bt.insert("zCow", "zClarabelle");
  bt.insert("zAbalone", "zAntoinette");
  bt.insert("zHorse", "zHorace");
  bt.insert("zIguana", "zIgnatius");
  bt.insert("zJackal", "zJolene");
  bt.insert("zKanagaroo", "zKarl");
  bt.insert("zAnt", "zAlbert");
  bt.insert("zAnteater", "zAlicia");
  bt.insert("zBat", "zBarry");
  bt.insert("zBear", "zBeatrice");
  bt.insert("zCamel", "zCarlos");
  bt.insert("zCanary", "zCaroline");
  bt.insert("zCatbird", "zCharles");
  bt.insert("zChickadee", "zChester");
  bt.insert("zMonkey", "zMary");
  bt.insert("zMouse", "zMinnie");
  bt.insert("zLlama", "zLarry");
  bt.insert("zLion", "zLeo");
  bt.insert("zEmu", "zEmil");
  bt.insert("zGoat", "zGeraldine");
  bt.insert("zFlea", "zFrancine");
  bt.insert("zFrog", "zFred");
  bt.insert("zAardvark", "zAnthony");
  bt.insert("zMouse", "zMickey");     // Duplicate key, update value

  EXPECT_TRUE(bt.lookup("Cat", value));
  EXPECT_EQ(value, "Charlene");
  EXPECT_TRUE(bt.lookup("Dog", value));
  EXPECT_EQ(value, "Donna");
  EXPECT_TRUE(bt.lookup("Elephant", value));
  EXPECT_EQ(value, "Emily");
  EXPECT_TRUE(bt.lookup("Fox", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Gopher", value));
  EXPECT_EQ(value, "Gertrude");
  EXPECT_TRUE(bt.lookup("Buffalo", value));
  EXPECT_EQ(value, "Bill");
  EXPECT_TRUE(bt.lookup("Albacore", value));
  EXPECT_EQ(value, "Arianne");
  EXPECT_TRUE(bt.lookup("Bison", value));
  EXPECT_EQ(value, "Boris");
  EXPECT_TRUE(bt.lookup("Cow", value));
  EXPECT_EQ(value, "Clarabelle");
  EXPECT_TRUE(bt.lookup("Abalone", value));
  EXPECT_EQ(value, "Antoinette");
  EXPECT_TRUE(bt.lookup("Horse", value));
  EXPECT_EQ(value, "Horace");
  EXPECT_TRUE(bt.lookup("Iguana", value));
  EXPECT_EQ(value, "Ignatius");
  EXPECT_TRUE(bt.lookup("Jackal", value));
  EXPECT_EQ(value, "Jolene");
  EXPECT_TRUE(bt.lookup("Kanagaroo", value));
  EXPECT_EQ(value, "Karl");
  EXPECT_TRUE(bt.lookup("Ant", value));
  EXPECT_EQ(value, "Albert");
  EXPECT_TRUE(bt.lookup("Anteater", value));
  EXPECT_EQ(value, "Alicia");
  EXPECT_TRUE(bt.lookup("Bat", value));
  EXPECT_EQ(value, "Barry");
  EXPECT_TRUE(bt.lookup("Bear", value));
  EXPECT_EQ(value, "Beatrice");
  EXPECT_TRUE(bt.lookup("Camel", value));
  EXPECT_EQ(value, "Carlos");
  EXPECT_TRUE(bt.lookup("Canary", value));
  EXPECT_EQ(value, "Caroline");
  EXPECT_TRUE(bt.lookup("Catbird", value));
  EXPECT_EQ(value, "Charles");
  EXPECT_TRUE(bt.lookup("Chickadee", value));
  EXPECT_EQ(value, "Chester");
  EXPECT_TRUE(bt.lookup("Monkey", value));
  EXPECT_EQ(value, "Mary");
  EXPECT_TRUE(bt.lookup("Mouse", value));
  EXPECT_EQ(value, "Mickey");
  EXPECT_TRUE(bt.lookup("Llama", value));
  EXPECT_EQ(value, "Larry");
  EXPECT_TRUE(bt.lookup("Lion", value));
  EXPECT_EQ(value, "Leo");
  EXPECT_TRUE(bt.lookup("Emu", value));
  EXPECT_EQ(value, "Emil");
  EXPECT_TRUE(bt.lookup("Goat", value));
  EXPECT_EQ(value, "Geraldine");
  EXPECT_TRUE(bt.lookup("Flea", value));
  EXPECT_EQ(value, "Francine");
  EXPECT_TRUE(bt.lookup("Frog", value));
  EXPECT_EQ(value, "Fred");
  EXPECT_TRUE(bt.lookup("Aardvark", value));
  EXPECT_EQ(value, "Anthony");

  EXPECT_TRUE(bt.lookup("zCat", value));
  EXPECT_EQ(value, "zCharlene");
  EXPECT_TRUE(bt.lookup("zDog", value));
  EXPECT_EQ(value, "zDonna");
  EXPECT_TRUE(bt.lookup("zElephant", value));
  EXPECT_EQ(value, "zEmily");
  EXPECT_TRUE(bt.lookup("zFox", value));
  EXPECT_EQ(value, "zFrancine");
  EXPECT_TRUE(bt.lookup("zGopher", value));
  EXPECT_EQ(value, "zGertrude");
  EXPECT_TRUE(bt.lookup("zBuffalo", value));
  EXPECT_EQ(value, "zBill");
  EXPECT_TRUE(bt.lookup("zAlbacore", value));
  EXPECT_EQ(value, "zArianne");
  EXPECT_TRUE(bt.lookup("zBison", value));
  EXPECT_EQ(value, "zBoris");
  EXPECT_TRUE(bt.lookup("zCow", value));
  EXPECT_EQ(value, "zClarabelle");
  EXPECT_TRUE(bt.lookup("zAbalone", value));
  EXPECT_EQ(value, "zAntoinette");
  EXPECT_TRUE(bt.lookup("zHorse", value));
  EXPECT_EQ(value, "zHorace");
  EXPECT_TRUE(bt.lookup("zIguana", value));
  EXPECT_EQ(value, "zIgnatius");
  EXPECT_TRUE(bt.lookup("zJackal", value));
  EXPECT_EQ(value, "zJolene");
  EXPECT_TRUE(bt.lookup("zKanagaroo", value));
  EXPECT_EQ(value, "zKarl");
  EXPECT_TRUE(bt.lookup("zAnt", value));
  EXPECT_EQ(value, "zAlbert");
  EXPECT_TRUE(bt.lookup("zAnteater", value));
  EXPECT_EQ(value, "zAlicia");
  EXPECT_TRUE(bt.lookup("zBat", value));
  EXPECT_EQ(value, "zBarry");
  EXPECT_TRUE(bt.lookup("zBear", value));
  EXPECT_EQ(value, "zBeatrice");
  EXPECT_TRUE(bt.lookup("zCamel", value));
  EXPECT_EQ(value, "zCarlos");
  EXPECT_TRUE(bt.lookup("zCanary", value));
  EXPECT_EQ(value, "zCaroline");
  EXPECT_TRUE(bt.lookup("zCatbird", value));
  EXPECT_EQ(value, "zCharles");
  EXPECT_TRUE(bt.lookup("zChickadee", value));
  EXPECT_EQ(value, "zChester");
  EXPECT_TRUE(bt.lookup("zMonkey", value));
  EXPECT_EQ(value, "zMary");
  EXPECT_TRUE(bt.lookup("zMouse", value));
  EXPECT_EQ(value, "zMickey");
  EXPECT_TRUE(bt.lookup("zLlama", value));
  EXPECT_EQ(value, "zLarry");
  EXPECT_TRUE(bt.lookup("zLion", value));
  EXPECT_EQ(value, "zLeo");
  EXPECT_TRUE(bt.lookup("zEmu", value));
  EXPECT_EQ(value, "zEmil");
  EXPECT_TRUE(bt.lookup("zGoat", value));
  EXPECT_EQ(value, "zGeraldine");
  EXPECT_TRUE(bt.lookup("zFlea", value));
  EXPECT_EQ(value, "zFrancine");
  EXPECT_TRUE(bt.lookup("zFrog", value));
  EXPECT_EQ(value, "zFred");
  EXPECT_TRUE(bt.lookup("zAardvark", value));
  EXPECT_EQ(value, "zAnthony");

  EXPECT_FALSE(bt.lookup("Aardvarkeee", value));

  EXPECT_EQ(bt.count(), 62);

  EXPECT_EQ(bt.depth("Aardvark"), 2);
  EXPECT_EQ(bt.depth("Abalone"), 2);
  EXPECT_EQ(bt.depth("Albacore"), 2);
  EXPECT_EQ(bt.depth("Ant"), 1);
  EXPECT_EQ(bt.depth("Anteater"), 2);
  EXPECT_EQ(bt.depth("Bat"), 2);
  EXPECT_EQ(bt.depth("Bear"), 2);
  EXPECT_EQ(bt.depth("Bison"), 1);
  EXPECT_EQ(bt.depth("Buffalo"), 2);
  EXPECT_EQ(bt.depth("Camel"), 2);
  EXPECT_EQ(bt.depth("Canary"), 2);
  EXPECT_EQ(bt.depth("Cat"), 1);
  EXPECT_EQ(bt.depth("Catbird"), 2);
  EXPECT_EQ(bt.depth("Chickadee"), 2);
  EXPECT_EQ(bt.depth("Cow"), 2);
  EXPECT_EQ(bt.depth("Dog"), 0);
  EXPECT_EQ(bt.depth("Elephant"), 2);
  EXPECT_EQ(bt.depth("Emu"), 2);
  EXPECT_EQ(bt.depth("Flea"), 2);
  EXPECT_EQ(bt.depth("Fox"), 1);
  EXPECT_EQ(bt.depth("Frog"), 2);
  EXPECT_EQ(bt.depth("Goat"), 2);
  EXPECT_EQ(bt.depth("Gopher"), 2);
  EXPECT_EQ(bt.depth("Horse"), 1);
  EXPECT_EQ(bt.depth("Iguana"), 2);
  EXPECT_EQ(bt.depth("Jackal"), 2);
  EXPECT_EQ(bt.depth("Kanagaroo"), 2);
  EXPECT_EQ(bt.depth("Lion"), 1);
  EXPECT_EQ(bt.depth("Llama"), 2);
  EXPECT_EQ(bt.depth("Monkey"), 2);
  EXPECT_EQ(bt.depth("Mouse"), 2);

  EXPECT_EQ(bt.depth("zAardvark"), 2);

  EXPECT_EQ(bt.depth("zAbalone"), 0);
  EXPECT_EQ(bt.depth("zAlbacore"), 2);
  EXPECT_EQ(bt.depth("zAnt"), 2);
  EXPECT_EQ(bt.depth("zAnteater"), 2);
  EXPECT_EQ(bt.depth("zBat"), 1);
  EXPECT_EQ(bt.depth("zBear"), 2);
  EXPECT_EQ(bt.depth("zBison"), 2);
  EXPECT_EQ(bt.depth("zBuffalo"), 2);
  EXPECT_EQ(bt.depth("zCamel"), 2);
  EXPECT_EQ(bt.depth("zCanary"), 2);
  EXPECT_EQ(bt.depth("zCat"), 1);
  EXPECT_EQ(bt.depth("zCatbird"), 2);
  EXPECT_EQ(bt.depth("zChickadee"), 2);
  EXPECT_EQ(bt.depth("zCow"), 2);
  EXPECT_EQ(bt.depth("zDog"), 1);
  EXPECT_EQ(bt.depth("zElephant"), 2);
  EXPECT_EQ(bt.depth("zEmu"), 2);
  EXPECT_EQ(bt.depth("zFlea"), 2);
  EXPECT_EQ(bt.depth("zFox"), 1);
  EXPECT_EQ(bt.depth("zFrog"), 2);
  EXPECT_EQ(bt.depth("zGoat"), 2);
  EXPECT_EQ(bt.depth("zGopher"), 2);
  EXPECT_EQ(bt.depth("zHorse"), 2);
  EXPECT_EQ(bt.depth("zIguana"), 2);
  EXPECT_EQ(bt.depth("zJackal"), 1);
  EXPECT_EQ(bt.depth("zKanagaroo"), 2);
  EXPECT_EQ(bt.depth("zLion"), 2);
  EXPECT_EQ(bt.depth("zLlama"), 2);
  EXPECT_EQ(bt.depth("zMonkey"), 2);
  EXPECT_EQ(bt.depth("zMouse"), 2);

  // bt.print();  // uncomment if needed to debug test
  remove(BTREE_TMP_FILE);
}

/* Comment out tests that use data file, because Bazel
   doesn't let test access them.

// Test count, depth, and insert without using insert.
// Use pre-built test tree instead.
// Test tree was built from threelevel.in

TEST(testtree, count) {
BTree bt(TEST_FILE);
bt.print();  // uncomment if needed to debug test
EXPECT_EQ(bt.count(), 31);
}

TEST(testtree, depth) {
BTree bt(TEST_FILE);
//bt.print();  // uncomment if needed to debug test

// nice almost even 3-level tree
EXPECT_EQ(bt.depth("Aardvark"), 2);
EXPECT_EQ(bt.depth("Abalone"), 2);
EXPECT_EQ(bt.depth("Albacore"), 2);
EXPECT_EQ(bt.depth("Ant"), 1);
EXPECT_EQ(bt.depth("Anteater"), 2);
EXPECT_EQ(bt.depth("Bat"), 2);
EXPECT_EQ(bt.depth("Bear"), 2);
EXPECT_EQ(bt.depth("Bison"), 1);
EXPECT_EQ(bt.depth("Buffalo"), 2);
EXPECT_EQ(bt.depth("Camel"), 2);
EXPECT_EQ(bt.depth("Canary"), 2);
EXPECT_EQ(bt.depth("Cat"), 1);
EXPECT_EQ(bt.depth("Catbird"), 2);
EXPECT_EQ(bt.depth("Chickadee"), 2);
EXPECT_EQ(bt.depth("Cow"), 2);
EXPECT_EQ(bt.depth("Dog"), 0);
EXPECT_EQ(bt.depth("Elephant"), 2);
EXPECT_EQ(bt.depth("Emu"), 2);
EXPECT_EQ(bt.depth("Flea"), 2);
EXPECT_EQ(bt.depth("Fox"), 1);
EXPECT_EQ(bt.depth("Frog"), 2);
EXPECT_EQ(bt.depth("Goat"), 2);
EXPECT_EQ(bt.depth("Gopher"), 2);
EXPECT_EQ(bt.depth("Horse"), 1);
EXPECT_EQ(bt.depth("Iguana"), 2);
EXPECT_EQ(bt.depth("Jackal"), 2);
EXPECT_EQ(bt.depth("Kanagaroo"), 2);
EXPECT_EQ(bt.depth("Lion"), 1);
EXPECT_EQ(bt.depth("Llama"), 2);
EXPECT_EQ(bt.depth("Monkey"), 2);
EXPECT_EQ(bt.depth("Mouse"), 2);

EXPECT_EQ(bt.depth("NoSuchAnimal"), -1);
}

TEST(testtree, lookup) {
BTree bt(TEST_FILE);
//bt.print();  // uncomment if needed to debug test

string value;
// nice almost even 3-level tree
EXPECT_TRUE(bt.lookup("Cat", value));
EXPECT_EQ(value, "Charlene");
EXPECT_TRUE(bt.lookup("Dog", value));
EXPECT_EQ(value, "Donna");
EXPECT_TRUE(bt.lookup("Elephant", value));
EXPECT_EQ(value, "Emily");
EXPECT_TRUE(bt.lookup("Fox", value));
EXPECT_EQ(value, "Francine");
EXPECT_TRUE(bt.lookup("Gopher", value));
EXPECT_EQ(value, "Gertrude");
EXPECT_TRUE(bt.lookup("Buffalo", value));
EXPECT_EQ(value, "Bill");
EXPECT_TRUE(bt.lookup("Albacore", value));
EXPECT_EQ(value, "Arianne");
EXPECT_TRUE(bt.lookup("Bison", value));
EXPECT_EQ(value, "Boris");
EXPECT_TRUE(bt.lookup("Cow", value));
EXPECT_EQ(value, "Clarabelle");
EXPECT_TRUE(bt.lookup("Abalone", value));
EXPECT_EQ(value, "Antoinette");
EXPECT_TRUE(bt.lookup("Horse", value));
EXPECT_EQ(value, "Horace");
EXPECT_TRUE(bt.lookup("Iguana", value));
EXPECT_EQ(value, "Ignatius");
EXPECT_TRUE(bt.lookup("Jackal", value));
EXPECT_EQ(value, "Jolene");
EXPECT_TRUE(bt.lookup("Kanagaroo", value));
EXPECT_EQ(value, "Karl");
EXPECT_TRUE(bt.lookup("Ant", value));
EXPECT_EQ(value, "Albert");
EXPECT_TRUE(bt.lookup("Anteater", value));
EXPECT_EQ(value, "Alicia");
EXPECT_TRUE(bt.lookup("Bat", value));
EXPECT_EQ(value, "Barry");
EXPECT_TRUE(bt.lookup("Bear", value));
EXPECT_EQ(value, "Beatrice");
EXPECT_TRUE(bt.lookup("Camel", value));
EXPECT_EQ(value, "Carlos");
EXPECT_TRUE(bt.lookup("Canary", value));
EXPECT_EQ(value, "Caroline");
EXPECT_TRUE(bt.lookup("Catbird", value));
EXPECT_EQ(value, "Charles");
EXPECT_TRUE(bt.lookup("Chickadee", value));
EXPECT_EQ(value, "Chester");
EXPECT_TRUE(bt.lookup("Monkey", value));
EXPECT_EQ(value, "Mary");
EXPECT_TRUE(bt.lookup("Mouse", value));
EXPECT_EQ(value, "Mickey");
EXPECT_TRUE(bt.lookup("Llama", value));
EXPECT_EQ(value, "Larry");
EXPECT_TRUE(bt.lookup("Lion", value));
EXPECT_EQ(value, "Leo");
EXPECT_TRUE(bt.lookup("Emu", value));
EXPECT_EQ(value, "Emil");
EXPECT_TRUE(bt.lookup("Goat", value));
EXPECT_EQ(value, "Geraldine");
EXPECT_TRUE(bt.lookup("Flea", value));
EXPECT_EQ(value, "Francine");
EXPECT_TRUE(bt.lookup("Frog", value));
EXPECT_EQ(value, "Fred");
EXPECT_TRUE(bt.lookup("Aardvark", value));
EXPECT_EQ(value, "Anthony");

EXPECT_FALSE(bt.lookup("Aardvarkeee", value));
EXPECT_FALSE(bt.lookup("NoSuchAnimal", value));
}
*/

#endif
