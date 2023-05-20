#ifndef __TEST_EMPTY_H
#define __TEST_EMPTY_H

#include <gtest/gtest.h>

#include "empty_class.hpp"

using namespace testing;

TEST(TestSorting, TestMergeSort)
{
  std::vector<int> v = {44, 25, 10, 31, 25, 48, 37, 43, 18, 48, 27};
  ProjectLibrary::MergeSort<int>(v, 0, v.size()-1);
  std::vector<int> sortedV = {48,48,44,43,37,31,27,25,25,18,10};
  EXPECT_EQ(v, sortedV);
}

#endif // __TEST_EMPTY_H
