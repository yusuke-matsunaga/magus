
/// @file SbjHandleTest.cc
/// @brief SbjHandleTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "SbjHandle.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_SBJ

TEST(SbjHandleTest, constructor0)
{
  SbjHandle handle;

  EXPECT_EQ( nullptr, handle.node() );
  EXPECT_EQ( false, handle.inv() );
}

TEST(SbjHandleTest, make_zero)
{
  SbjHandle handle = SbjHandle::make_zero();

  EXPECT_EQ( nullptr, handle.node() );
  EXPECT_FALSE( handle.inv() );
  EXPECT_TRUE( handle.is_const0() );
  EXPECT_FALSE( handle.is_const1() );

  SbjHandle handle1 = ~handle;
  EXPECT_FALSE( handle1.is_const0() );
  EXPECT_TRUE( handle1.is_const1() );

  SbjHandle handle2 = handle.normalize();
  EXPECT_EQ( handle, handle2 );
}

TEST(SbjHandleTest, make_one)
{
  SbjHandle handle = SbjHandle::make_one();

  EXPECT_EQ( nullptr, handle.node() );
  EXPECT_TRUE( handle.inv() );
  EXPECT_FALSE( handle.is_const0() );
  EXPECT_TRUE( handle.is_const1() );

  SbjHandle handle1 = ~handle;
  EXPECT_TRUE( handle1.is_const0() );
  EXPECT_FALSE( handle1.is_const1() );

  SbjHandle handle2 = handle.normalize();
  EXPECT_EQ( handle1, handle2 );
}

TEST(SbjHandleTest, node1)
{
  SbjNode* node = new SbjNode(0, false, 0);

  bool inv = false;
  SbjHandle handle(node, inv);

  EXPECT_EQ( node, handle.node() );
  EXPECT_EQ( inv, handle.inv() );
  EXPECT_FALSE( handle.is_const0() );
  EXPECT_FALSE( handle.is_const1() );

  SbjHandle handle1 = ~handle;
  EXPECT_EQ( node, handle1.node() );
  EXPECT_EQ( !inv, handle1.inv() );

  SbjHandle handle2 = handle.normalize();
  EXPECT_EQ( handle, handle2 );
}

TEST(SbjHandleTest, node2)
{
  SbjNode* node = new SbjNode(0, false, 0);

  bool inv = true;
  SbjHandle handle(node, inv);

  EXPECT_EQ( node, handle.node() );
  EXPECT_EQ( inv, handle.inv() );
  EXPECT_FALSE( handle.is_const0() );
  EXPECT_FALSE( handle.is_const1() );

  SbjHandle handle1 = ~handle;
  EXPECT_EQ( node, handle1.node() );
  EXPECT_EQ( !inv, handle1.inv() );

  SbjHandle handle2 = handle.normalize();
  EXPECT_EQ( handle1, handle2 );
}

END_NAMESPACE_SBJ
