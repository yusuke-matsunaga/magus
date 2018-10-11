
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
  BnNetwork network1;
  BnNetwork network2;

  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  bool stat1 = read_blif(network1, path1);
  ASSERT_TRUE( stat1 );

  int ni = network1.input_num();
  int no = network1.output_num();

  string filename2 = "C1355.blif";
  string path2 = DATAPATH + filename2;
  bool stat2 = read_blif(network2, path2);
  ASSERT_TRUE( stat2 );
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
  BnNetwork network1;
  BnNetwork network2;

  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  bool stat1 = read_blif(network1, path1);
  ASSERT_TRUE( stat1 );

  int ni = network1.input_num();
  int no = network1.output_num();

  string filename2 = "C1355.blif";
  string path2 = DATAPATH + filename2;
  bool stat2 = read_blif(network2, path2);
  ASSERT_TRUE( stat2 );
  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  vector<pair<int, int>> input_pair_list(ni);
  for ( int i: Range(ni) ) {
    int id1 = network1.input_id(i);
    int id2 = network2.input_id(i);
    input_pair_list[i] = make_pair(id1, id2);
  }
  vector<pair<int, int>> output_pair_list(no);
  for ( int i: Range(no) ) {
    int id1 = network1.output_id(i);
    int id2 = network2.output_id(i);
    output_pair_list[i] = make_pair(id1, id2);
  }

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, input_pair_list, output_pair_list);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( int i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

TEST(EquivTest, EquivTest3)
{
  BnNetwork network1;
  BnNetwork network2;

  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  bool stat1 = read_blif(network1, path1);
  ASSERT_TRUE( stat1 );

  int ni = network1.input_num();
  int no = network1.output_num();

  string filename2 = "C499_reordered.blif";
  string path2 = DATAPATH + filename2;
  bool stat2 = read_blif(network2, path2);
  ASSERT_TRUE( stat2 );
  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  HashMap<string, int> input_map1;
  for ( int id1: network1.input_id_list() ) {
    auto node = network1.node(id1);
    input_map1.add(node->name(), id1);
  }
  HashMap<string, int> output_map1;
  for ( int id1: network1.output_id_list() ) {
    auto node = network1.node(id1);
    output_map1.add(node->name(), id1);
  }

  vector<pair<int, int>> input_pair_list;
  input_pair_list.reserve(ni);
  for ( int id2: network2.input_id_list() ) {
    auto node = network2.node(id2);
    int id1;
    bool stat = input_map1.find(node->name(), id1);
    ASSERT_TRUE( stat );

    input_pair_list.push_back(make_pair(id1, id2));
  }
  vector<pair<int, int>> output_pair_list;
  output_pair_list.reserve(no);
  for ( int id2: network2.output_id_list() ) {
    auto node = network2.node(id2);
    int id1;
    bool stat = output_map1.find(node->name(), id1);
    ASSERT_TRUE( stat );

    output_pair_list.push_back(make_pair(id1, id2));
  }

  EquivMgr eqmgr;
  EquivResult ans = eqmgr.check(network1, network2, input_pair_list, output_pair_list);
  EXPECT_EQ( SatBool3::True, ans.result() );
  for ( int i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, ans.output_results()[i] );
  }
}

END_NAMESPACE_MAGUS
