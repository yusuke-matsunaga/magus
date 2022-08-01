
/// @file equiv_test.cc
/// @brief equiv_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.

#include "gtest/gtest.h"
#include "EquivMgr.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_MAGUS

TEST(EquivTest, EquivTest1)
{
  string filename1 = "blif/C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  SizeType ni = network1.input_num();
  SizeType no = network1.output_num();

  string filename2 = "blif/C1355.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( SizeType i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest2)
{
  string filename1 = "blif/C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  SizeType ni = network1.input_num();
  SizeType no = network1.output_num();

  string filename2 = "blif/C1355.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  vector<SizeType> input2_list(ni);
  for ( SizeType i: Range(ni) ) {
    input2_list[i] = i;
  }
  vector<SizeType> output2_list(no);
  for ( SizeType i: Range(no) ) {
    output2_list[i] = i;
  }

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, input2_list, output2_list);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( SizeType i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest3)
{
  string filename1 = "blif/C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  SizeType ni = network1.input_num();
  SizeType no = network1.output_num();

  string filename2 = "blif/C499_reordered.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  // network1 の入力ノードの名前のハッシュ表を作る．
  unordered_map<string, SizeType> input_map;
  for ( SizeType pos: Range(ni) ) {
    SizeType id1 = network1.input_id(pos);
    auto& node = network1.node(id1);
    input_map.emplace(node.name(), pos);
  }

  // network1 の出力ノードの名前のハッシュ表を作る．
  unordered_map<string, SizeType> output_map;
  for ( SizeType pos: Range(no) ) {
    SizeType id1 = network1.output_id(pos);
    auto& node = network1.node(id1);
    output_map.emplace(node.name(), pos);
  }

  vector<SizeType> input2_list(ni);
  for ( SizeType pos2: Range(ni) ) {
    SizeType id2 = network2.input_id(pos2);
    auto& node2 = network2.node(id2);
    SizeType pos1 = input_map.at(node2.name());
    input2_list[pos2] = pos1;
  }

  vector<SizeType> output2_list(no);
  for ( SizeType pos2: Range(no) ) {
    SizeType id2 = network2.output_id(pos2);
    auto& node2 = network2.node(id2);
    SizeType pos1 = output_map.at(node2.name());
    output2_list[pos2] = pos1;
  }

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, input2_list, output2_list);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( SizeType i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest4)
{
  string filename1 = "blif/C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  SizeType ni = network1.input_num();
  SizeType no = network1.output_num();

  string filename2 = "blif/C499_reordered.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, true);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( SizeType i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest5)
{
  string filename1 = "truth/ex00.truth";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_truth(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  SizeType ni = network1.input_num();
  SizeType no = network1.output_num();

  string filename2 = "aig/ex00.aig";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_aig(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( SizeType i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

END_NAMESPACE_MAGUS
