# Global compile options - warnings are useful
COPTS = ["-Wall", "-Wextra", "--std=c++11"]

# Student code
cc_library(
  name = "BTree",
  srcs = ["BTree.cc"],
  hdrs = ["BTree.h"],
  deps = [":BTreeFile", ":BTreeBlock"],
  copts = COPTS,
)

# Provided code
cc_library(
  name = "BTreeFile",
  srcs = ["BTreeFile.cc"],
  hdrs = ["BTreeFile.h"],
  deps = [":BTreeBlock"],
  copts = COPTS,
)

cc_library(
  name = "BTreeBlock",
  srcs = ["BTreeBlock.cc"],
  hdrs = ["BTreeBlock.h", "BTreeFile.h"],
  copts = COPTS,
)

# Provided tests
cc_test(
  name = "project4_test",
  size = "small",
  srcs = ["BTree_test.cc"],
  deps = [":BTree", ":BTreeFile", ":BTreeBlock",
          "@com_google_googletest//:gtest_main"],
  copts = COPTS,
)

# Optional binary for running code manually
cc_binary(
  name = "project4",
  srcs = ["project4.cc"],
  deps = [":BTree", ":BTreeFile", ":BTreeBlock"],
  copts = COPTS,
)
