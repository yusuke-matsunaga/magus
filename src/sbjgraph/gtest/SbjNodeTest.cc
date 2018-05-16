
/// @file SbjNodeTest.cc
/// @brief SbjNodeTest の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "SbjNode.h"


BEGIN_NAMESPACE_YM_SBJ

TEST(SbjNodeTest, input1)
{
  int id = 123;
  bool bipol = true;
  int subid = 456;
  SbjNode* node = new SbjNode(id, bipol, subid);

  EXPECT_EQ( id, node->id() );
  EXPECT_EQ( SbjNodeType::Input, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_EQ( bipol, node->is_bipol() );
  EXPECT_FALSE( node->is_output() );
  EXPECT_FALSE( node->is_logic() );
  EXPECT_FALSE( node->is_and() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_EQ( subid, node->subid() );

  EXPECT_EQ( 0, node->fanout_num() );
  EXPECT_FALSE( node->pomark() );
  EXPECT_EQ( 0, node->level() );
}

TEST(SbjNodeTest, input2)
{
  int id = 123;
  bool bipol = false;
  int subid = 456;
  SbjNode* node = new SbjNode(id, bipol, subid);

  EXPECT_EQ( id, node->id() );
  EXPECT_EQ( SbjNodeType::Input, node->type() );
  EXPECT_TRUE( node->is_input() );
  EXPECT_EQ( bipol, node->is_bipol() );
  EXPECT_FALSE( node->is_output() );
  EXPECT_FALSE( node->is_logic() );
  EXPECT_FALSE( node->is_and() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_EQ( subid, node->subid() );

  EXPECT_EQ( 0, node->fanout_num() );
  EXPECT_FALSE( node->pomark() );
  EXPECT_EQ( 0, node->level() );
}

TEST(SbjNodeTest, output)
{
  SbjNode* input = new SbjNode(0, false, 0);

  int id = 987;
  int subid = 111;
  SbjHandle ihandle(input, false);
  SbjNode* node = new SbjNode(id, subid, ihandle);

  EXPECT_EQ( id, node->id() );
  EXPECT_EQ( SbjNodeType::Output, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_TRUE( node->is_output() );
  EXPECT_FALSE( node->is_logic() );
  EXPECT_FALSE( node->is_and() );
  EXPECT_FALSE( node->is_xor() );
  EXPECT_EQ( subid, node->subid() );

  EXPECT_EQ( ihandle, node->output_fanin_handle() );
  EXPECT_EQ( input, node->output_fanin() );
  EXPECT_EQ( false, node->output_fanin_inv() );

  EXPECT_EQ( 0, node->fanout_num() );
  EXPECT_FALSE( node->pomark() );
  EXPECT_EQ( 0, node->level() );

  EXPECT_TRUE( input->pomark() );
}

TEST(SbjNodeTest, and1)
{
  SbjNode* input1 = new SbjNode(0, false, 0);
  SbjNode* input2 = new SbjNode(1, false, 1);

  bool iinv1 = false;
  bool iinv2 = true;

  SbjHandle ihandle1(input1, iinv1);
  SbjHandle ihandle2(input2, iinv2);

  int id = 1024;
  SbjNode* node = new SbjNode(id, SbjNodeType::LogicAnd, ihandle1, ihandle2);

  EXPECT_EQ( id, node->id() );
  EXPECT_EQ( SbjNodeType::LogicAnd, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_FALSE( node->is_output() );
  EXPECT_TRUE( node->is_logic() );
  EXPECT_TRUE( node->is_and() );
  EXPECT_FALSE( node->is_xor() );

  EXPECT_EQ( ihandle1, node->fanin_handle(0) );
  EXPECT_EQ( ihandle1, node->fanin0_handle() );
  EXPECT_EQ( input1, node->fanin(0) );
  EXPECT_EQ( input1, node->fanin0() );
  EXPECT_EQ( iinv1, node->fanin_inv(0) );
  EXPECT_EQ( iinv1, node->fanin0_inv() );

  EXPECT_EQ( ihandle2, node->fanin_handle(1) );
  EXPECT_EQ( ihandle2, node->fanin1_handle() );
  EXPECT_EQ( input2, node->fanin(1) );
  EXPECT_EQ( input2, node->fanin1() );
  EXPECT_EQ( iinv2, node->fanin_inv(1) );
  EXPECT_EQ( iinv2, node->fanin1_inv() );

  EXPECT_EQ( 0, node->fanout_num() );
  EXPECT_FALSE( node->pomark() );
  EXPECT_EQ( 1, node->level() );

  EXPECT_EQ( 1, input1->fanout_num() );
  EXPECT_EQ( 1, input2->fanout_num() );
}

TEST(SbjNodeTest, xor1)
{
  SbjNode* input1 = new SbjNode(0, false, 0);
  SbjNode* input2 = new SbjNode(1, false, 1);

  bool iinv1 = false;
  bool iinv2 = true;

  SbjHandle ihandle1(input1, iinv1);
  SbjHandle ihandle2(input2, iinv2);

  int id = 1024;
  SbjNode* node = new SbjNode(id, SbjNodeType::LogicXor, ihandle1, ihandle2);

  EXPECT_EQ( id, node->id() );
  EXPECT_EQ( SbjNodeType::LogicXor, node->type() );
  EXPECT_FALSE( node->is_input() );
  EXPECT_FALSE( node->is_output() );
  EXPECT_TRUE( node->is_logic() );
  EXPECT_FALSE( node->is_and() );
  EXPECT_TRUE( node->is_xor() );

  EXPECT_EQ( ihandle1, node->fanin_handle(0) );
  EXPECT_EQ( ihandle1, node->fanin0_handle() );
  EXPECT_EQ( input1, node->fanin(0) );
  EXPECT_EQ( input1, node->fanin0() );
  EXPECT_EQ( iinv1, node->fanin_inv(0) );
  EXPECT_EQ( iinv1, node->fanin0_inv() );

  EXPECT_EQ( ihandle2, node->fanin_handle(1) );
  EXPECT_EQ( ihandle2, node->fanin1_handle() );
  EXPECT_EQ( input2, node->fanin(1) );
  EXPECT_EQ( input2, node->fanin1() );
  EXPECT_EQ( iinv2, node->fanin_inv(1) );
  EXPECT_EQ( iinv2, node->fanin1_inv() );

  EXPECT_EQ( 0, node->fanout_num() );
  EXPECT_FALSE( node->pomark() );
  EXPECT_EQ( 1, node->level() );

  EXPECT_EQ( 1, input1->fanout_num() );
  EXPECT_EQ( 1, input2->fanout_num() );
}

END_NAMESPACE_YM_SBJ
