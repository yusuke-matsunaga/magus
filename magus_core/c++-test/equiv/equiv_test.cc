
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
  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  int ni = network1.input_num();
  int no = network1.output_num();

  string filename2 = "C1355.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( int i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest2)
{
  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  int ni = network1.input_num();
  int no = network1.output_num();

  string filename2 = "C1355.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  vector<int> input2_list(ni);
  for ( int i: Range(ni) ) {
    input2_list[i] = i;
  }
  vector<int> output2_list(no);
  for ( int i: Range(no) ) {
    output2_list[i] = i;
  }

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, input2_list, output2_list);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( int i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest3)
{
  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  BnNetwork network1 = BnNetwork::read_blif(path1);
  ASSERT_TRUE( network1.node_num() != 0 );

  int ni = network1.input_num();
  int no = network1.output_num();

  string filename2 = "C499_reordered.blif";
  string path2 = DATAPATH + filename2;
  BnNetwork network2 = BnNetwork::read_blif(path2);
  ASSERT_TRUE( network2.node_num() != 0 );

  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  // network1 の入力ノードの名前のハッシュ表を作る．
  unordered_map<string, int> input_map;
  for ( int pos: Range(ni) ) {
    int id1 = network1.input_id(pos);
    auto& node = network1.node(id1);
    input_map.emplace(node.name(), pos);
  }

  // network1 の出力ノードの名前のハッシュ表を作る．
  unordered_map<string, int> output_map;
  for ( int pos: Range(no) ) {
    int id1 = network1.output_id(pos);
    auto& node = network1.node(id1);
    output_map.emplace(node.name(), pos);
  }

  vector<int> input2_list(ni);
  for ( int pos2: Range(ni) ) {
    int id2 = network2.input_id(pos2);
    auto& node2 = network2.node(id2);
    int pos1 = input_map.at(node2.name());
    input2_list[pos2] = pos1;
  }

  vector<int> output2_list(no);
  for ( int pos2: Range(no) ) {
    int id2 = network2.output_id(pos2);
    auto& node2 = network2.node(id2);
    int pos1 = output_map.at(node2.name());
    output2_list[pos2] = pos1;
  }

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, input2_list, output2_list);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( int i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

END_NAMESPACE_MAGUS
