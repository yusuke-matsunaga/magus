
/// @file DgNode_test.cc
/// @brief DgNode_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2022 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "DgNode.h"
#include "DgEdge.h"
#include "DgMgr.h"
#include "ym/BddMgr.h"


BEGIN_NAMESPACE_DG

class DgNodeTest:
public ::testing::Test
{
public:

  void
  SetUp();

  void
  TearDown();

  BddMgr&
  bddmgr()
  {
    return mBddMgr;
  }

  DgMgr&
  mgr()
  {
    return mDgMgr;
  }

private:

  BddMgr mBddMgr;

  DgMgr mDgMgr;

};

void
DgNodeTest::SetUp()
{
}

void
DgNodeTest::TearDown()
{
}

TEST_F(DgNodeTest, make_lit1)
{
  auto edge = mgr().make_lit(0);

  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto lit = bddmgr().literal(VarId{0});
  EXPECT_TRUE( lit.is_identical(edge.global_func()) );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_FALSE( inv );

  EXPECT_TRUE( node->is_lit() );
  EXPECT_FALSE( node->is_or() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );
  EXPECT_EQ( 0, node->top() );
}

TEST_F(DgNodeTest, make_lit2)
{
  auto edge = mgr().make_lit(1);

  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto lit = bddmgr().literal(VarId{1});
  EXPECT_TRUE( lit.is_identical(edge.global_func()) );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_FALSE( inv );

  EXPECT_TRUE( node->is_lit() );
  EXPECT_FALSE( node->is_or() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );
  EXPECT_EQ( 1, node->top() );
}

TEST_F(DgNodeTest, make_lit3)
{
  auto edge = mgr().make_lit(2);

  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto lit = bddmgr().literal(VarId{2});
  EXPECT_TRUE( lit.is_identical(edge.global_func()) );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_FALSE( inv );

  EXPECT_TRUE( node->is_lit() );
  EXPECT_FALSE( node->is_or() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );
  EXPECT_EQ( 2, node->top() );
}

TEST_F(DgNodeTest, make_or)
{
  auto edge0 = mgr().make_lit(0);
  auto edge1 = mgr().make_lit(1);

  auto edge = mgr().make_or({edge0, edge1});

  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_FALSE( inv );

  EXPECT_FALSE( node->is_lit() );
  EXPECT_TRUE( node->is_or() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );

  EXPECT_EQ( 2, node->child_num() );

  auto cedge0 = node->child(0);
  EXPECT_FALSE( cedge0.inv() );
  auto cnode0 = cedge0.node();
  EXPECT_TRUE( cnode0->is_lit() );
  EXPECT_EQ( 0, cnode0->top() );

  auto cedge1 = node->child(1);
  EXPECT_FALSE( cedge1.inv() );
  auto cnode1 = cedge1.node();
  EXPECT_TRUE( cnode1->is_lit() );
  EXPECT_EQ( 1, cnode1->top() );
}

TEST_F(DgNodeTest, make_xor1)
{
  auto edge0 = mgr().make_lit(0);
  auto edge1 = mgr().make_lit(1);

  auto edge = mgr().make_xor({edge0, edge1});

  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_FALSE( inv );

  EXPECT_FALSE( node->is_lit() );
  EXPECT_FALSE( node->is_or() );
  EXPECT_TRUE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );

  EXPECT_EQ( 2, node->child_num() );

  auto cedge0 = node->child(0);
  EXPECT_FALSE( cedge0.inv() );
  auto cnode0 = cedge0.node();
  EXPECT_TRUE( cnode0->is_lit() );
  EXPECT_EQ( 0, cnode0->top() );

  auto cedge1 = node->child(1);
  EXPECT_FALSE( cedge1.inv() );
  auto cnode1 = cedge1.node();
  EXPECT_TRUE( cnode1->is_lit() );
  EXPECT_EQ( 1, cnode1->top() );
}

TEST_F(DgNodeTest, make_xor2)
{
  auto edge0 = mgr().make_lit(0);
  auto edge1 = mgr().make_lit(1);

  auto edge = mgr().make_xor({~edge0, edge1});

  auto lit0 = edge0.global_func();
  auto lit1 = edge1.global_func();
  auto xor2 = ~lit0 ^ lit1;
  EXPECT_EQ( xor2, edge.global_func() );

  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_TRUE( inv );

  EXPECT_FALSE( node->is_lit() );
  EXPECT_FALSE( node->is_or() );
  EXPECT_TRUE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );

  EXPECT_EQ( 2, node->child_num() );

  auto cedge0 = node->child(0);
  EXPECT_FALSE( cedge0.inv() );
  auto cnode0 = cedge0.node();
  EXPECT_TRUE( cnode0->is_lit() );
  EXPECT_EQ( 0, cnode0->top() );

  auto cedge1 = node->child(1);
  EXPECT_FALSE( cedge1.inv() );
  auto cnode1 = cedge1.node();
  EXPECT_TRUE( cnode1->is_lit() );
  EXPECT_EQ( 1, cnode1->top() );
}

TEST_F(DgNodeTest, make_xor3)
{
  auto edge0 = mgr().make_lit(0);
  auto edge1 = mgr().make_lit(1);
  auto edge2 = mgr().make_lit(2);

  auto edge = mgr().make_xor({edge0, edge1, edge2});

  auto lit0 = edge0.global_func();
  auto lit1 = edge1.global_func();
  auto lit2 = edge2.global_func();
  auto xor3 = (lit0 ^ lit1) ^ lit2;
  EXPECT_EQ( xor3, edge.global_func() );
  EXPECT_FALSE( edge.is_zero() );
  EXPECT_FALSE( edge.is_one() );
  EXPECT_FALSE( edge.is_const() );

  auto node = edge.node();
  auto inv = edge.inv();
  EXPECT_FALSE( inv );

  EXPECT_FALSE( node->is_lit() );
  EXPECT_FALSE( node->is_or() );
  EXPECT_TRUE( node->is_xor() );
  EXPECT_FALSE( node->is_cplx() );

  EXPECT_EQ( 3, node->child_num() );

  auto cedge0 = node->child(0);
  EXPECT_FALSE( cedge0.inv() );
  auto cnode0 = cedge0.node();
  EXPECT_TRUE( cnode0->is_lit() );
  EXPECT_EQ( 0, cnode0->top() );

  auto cedge1 = node->child(1);
  EXPECT_FALSE( cedge1.inv() );
  auto cnode1 = cedge1.node();
  EXPECT_TRUE( cnode1->is_lit() );
  EXPECT_EQ( 1, cnode1->top() );

  auto cedge2 = node->child(2);
  EXPECT_FALSE( cedge2.inv() );
  auto cnode2 = cedge2.node();
  EXPECT_TRUE( cnode2->is_lit() );
  EXPECT_EQ( 2, cnode2->top() );
}

TEST_F(DgNodeTest, cplx1)
{
  auto bdd = bddmgr().from_truth("0100");
  auto edge = mgr().decomp(bdd);
  EXPECT_TRUE( bdd.is_identical(edge.global_func()) );
}

TEST_F(DgNodeTest, cplx2)
{
  auto bdd = bddmgr().from_truth("01000101");
  auto edge = mgr().decomp(bdd);
  EXPECT_TRUE( bdd.is_identical(edge.global_func()) );
}

TEST_F(DgNodeTest, cplx3)
{
  auto bdd = bddmgr().from_truth("10101001");
  auto edge = mgr().decomp(bdd);
  EXPECT_TRUE( bdd.is_identical(edge.global_func()) );
}

TEST_F(DgNodeTest, cplx4)
{
  auto bdd = bddmgr().from_truth("1111110111111100");
  auto edge = mgr().decomp(bdd);
  EXPECT_TRUE( bdd.is_identical(edge.global_func()) );
}

TEST_F(DgNodeTest, cplx5)
{
  auto bdd = bddmgr().from_truth("1111100000001000");
  auto edge = mgr().decomp(bdd);
  EXPECT_TRUE( bdd.is_identical(edge.global_func()) );
}

END_NAMESPACE_DG
