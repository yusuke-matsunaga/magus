
/// @file equiv_test.cc
/// @brief equiv_test の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2018 Yusuke Matsunaga
/// All rights reserved.


#include "gtest/gtest.h"
#include "EquivMgr.h"
#include "ym/BnNetwork.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_MAGUS

TEST(EquivTest, EquivTest)
{
  BnNetwork network1;
  BnNetwork network2;

  string filename1 = "C499.blif";
  string path1 = DATAPATH + filename1;
  bool stat1 = read_blif(network1, path1);
  ASSERT_TRUE( stat1 );

  int ni = network1.input_num();
  int no = network1.output_num();
  vector<int> input1_list(ni);
  vector<int> output1_list(no);
  for ( int i: Range(ni) ) {
    input1_list[i] = network1.input_id(i);
  }
  for ( int i: Range(no) ) {
    output1_list[i] = network1.output_id(i);
  }

  string filename2 = "C1355.blif";
  string path2 = DATAPATH + filename2;
  bool stat2 = read_blif(network2, path2);
  ASSERT_TRUE( stat2 );
  ASSERT_TRUE( network2.input_num() == ni );
  ASSERT_TRUE( network2.output_num() == no );

  vector<int> input2_list(ni);
  vector<int> output2_list(no);
  for ( int i: Range(ni) ) {
    input2_list[i] = network2.input_id(i);
  }
  for ( int i: Range(no) ) {
    output2_list[i] = network2.output_id(i);
  }

  EquivMgr eqmgr(1000);
  vector<SatBool3> eq_stats;
  SatBool3 ans = eqmgr.check(network1, input1_list, output1_list,
			     network2, input2_list, output2_list,
			     eq_stats);
  EXPECT_EQ( SatBool3::True, ans );
  for ( int i: Range(no) ) {
    EXPECT_EQ( SatBool3::True, eq_stats[i] );
  }
}

END_NAMESPACE_MAGUS
