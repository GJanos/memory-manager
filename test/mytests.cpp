
#include "../product/MemoryManager.h"
#include "../product/MList.h"
#include <iostream>
#include "gtest/gtest.h"

class metaTest : public ::testing::Test
{
protected:
  virtual void SetUp()
  {
    mm = new MemoryManager();
  }

  virtual void TearDown()
  {
    // Code here will be called immediately after each test
    // (right before the destructor).
  }
  MemoryManager *mm = 0;
};

TEST_F(metaTest, reduce_memory_not_all)
{

  int ret = mm->claimMemory(20);
  EXPECT_EQ(mm->memStart, ret);
  EXPECT_EQ(nullptr, mm->freeList->head->after);
  EXPECT_EQ(nullptr, mm->freeList->head->before);
  EXPECT_EQ(mm->memStart + 20, mm->freeList->head->_addr);
  EXPECT_EQ(80, mm->freeList->head->_size);
}

TEST_F(metaTest, reduce_memory_all)
{
  int ret = mm->claimMemory(100);
  EXPECT_EQ(mm->memStart, ret);
  EXPECT_EQ(nullptr, mm->freeList->head);
}

TEST_F(metaTest, claim_memory)
{
  mm->print();
  int ret1 = mm->claimMemory(20);
  mm->print();
  int ret2 = mm->claimMemory(20);
  mm->print();
  int ret3 = mm->claimMemory(20);
  mm->print();
  EXPECT_EQ(mm->memStart, ret1);
  EXPECT_EQ(mm->memStart + 20, ret2);
  EXPECT_EQ(mm->memStart + 40, ret3);
  EXPECT_EQ(mm->memStart + 60, mm->freeList->head->_addr);
  EXPECT_EQ(40, mm->freeList->head->_size);
  EXPECT_EQ(mm->memStart, mm->allocList->head->_addr);
  EXPECT_EQ(20, mm->allocList->head->_size);
  EXPECT_EQ(mm->memStart + 20, mm->allocList->head->after->_addr);
  EXPECT_EQ(20, mm->allocList->head->after->_size);
  EXPECT_EQ(mm->memStart + 40, mm->allocList->head->after->after->_addr);
  EXPECT_EQ(20, mm->allocList->head->after->after->_size);
}

TEST_F(metaTest, valid_de_alloc)
{
  mm->claimMemory(30);
  mm->claimMemory(10);
  mm->print();
  int ret1 = mm->freeMemory(mm->memStart);
  mm->print();
  int ret2 = mm->freeMemory(mm->memStart + 30);
  mm->print();
  EXPECT_EQ(30, ret1);
  EXPECT_EQ(10, ret2);
}

TEST_F(metaTest, valid_de_alloc_longer)
{
  mm->claimMemory(30);
  mm->print();
  mm->claimMemory(30);
  mm->print();
  mm->claimMemory(30);
  mm->print();
  int ret1 = mm->freeMemory(mm->memStart);
  mm->print();
  mm->claimMemory(10);
  mm->print();
  int ret2 = mm->freeMemory(mm->memStart + 30);
  mm->print();
  mm->claimMemory(20);
  mm->print();
  int ret3 = mm->freeMemory(mm->memStart + 60);
  mm->print();
  int ret4 = mm->freeMemory(mm->memStart);
  mm->print();
  EXPECT_EQ(30, ret1);
  EXPECT_EQ(30, ret2);
  EXPECT_EQ(30, ret3);
  EXPECT_EQ(10, ret4);
}

TEST_F(metaTest, test_specific)
{
  mm->claimMemory(10);
  mm->print();
  mm->claimMemory(10);
  mm->print();
  mm->claimMemory(20);
  mm->print();
  int ret1 = mm->freeMemory(mm->memStart + 10);
  mm->print();
  mm->claimMemory(15);
  mm->print();
  mm->claimMemory(15);
  mm->print();
  int ret2 = mm->freeMemory(mm->memStart + 40);
  mm->print();
  EXPECT_EQ(10, ret1);
  EXPECT_EQ(15, ret2);
}
