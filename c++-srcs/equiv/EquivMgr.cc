
/// @file EquivMgr.cc
/// @brief EquivMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "EquivMgr.h"
#include "FraigMgr.h"
#include "ym/BnNetwork.h"
#include "ym/BnNode.h"
#include "ym/Range.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// クラス EquivMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] sig_size シグネチャのサイズ
// @param[in] solver_type SAT-solver の種類を表すオブジェクト
EquivMgr::EquivMgr(
  SizeType sig_size,
  const SatSolverType& solver_type
) : mSigSize{sig_size},
    mSolverType{solver_type}
{
}

// @brief 2つの回路が等価かどうか調べる．
EquivResult
EquivMgr::check(
  const BnNetwork& network1,
  const BnNetwork& network2,
  bool match_by_name
)
{
  SizeType ni = network1.input_num();
  if ( network2.input_num() != ni ) {
    return SatBool3::False;
  }
  SizeType no = network1.output_num();
  if ( network2.output_num() != no ) {
    return SatBool3::False;
  }

  vector<int> input2_list(ni);
  vector<int> output2_list(no);
  if ( match_by_name ) {
    // 名前による対応
    // 入力名をキーにして network1 の入力位置を格納する辞書
    unordered_map<string, SizeType> input_map;
    for ( SizeType i = 0; i < ni; ++ i ) {
      SizeType id = network1.input_id(i);
      auto& node = network1.node(id);
      input_map.emplace(node.name(), i);
    }
    // 入力の対応付を行う．
    for ( SizeType i = 0; i < ni; ++ i ) {
      SizeType id = network2.input_id(i);
      auto& node = network2.node(id);
      auto name = node.name();
      if ( input_map.count(name) == 0 ) {
	// 入力名が未対応
	return EquivResult{SatBool3::False, vector<SatBool3>(no, SatBool3::False)};
      }
      input2_list[i] = input_map.at(name);
    }
    // 出力名をキーにして network1 の出力位置を格納する辞書
    unordered_map<string, SizeType> output_map;
    for ( SizeType i = 0; i < no; ++ i ) {
      SizeType id = network1.output_id(i);
      auto& node = network1.node(id);
      output_map.emplace(node.name(), i);
    }
    // 出力の対応付を行う．
    for ( SizeType i = 0; i < no; ++ i ) {
      SizeType id = network2.output_id(i);
      auto& node = network2.node(id);
      auto name = node.name();
      if ( output_map.count(name) == 0 ) {
	// 出力名が未対応
	return EquivResult{SatBool3::False, vector<SatBool3>(no, SatBool3::False)};
      }
      output2_list[i] = output_map.at(name);
    }
  }
  else {
    // 順序による対応
    for ( int i: Range(ni) ) {
      input2_list[i] = i;
    }
    for ( int i: Range(no) ) {
      output2_list[i] = i;
    }
  }

  return check(network1, network2, input2_list, output2_list);
}

// @brief 2つの回路が等価かどうか調べる．
EquivResult
EquivMgr::check(
  const BnNetwork& network1,
  const BnNetwork& network2,
  const vector<int>& input2_list,
  const vector<int>& output2_list
)
{
  // 最初に入出力数が等しいか調べる．
  SizeType ni = network1.input_num();
  ASSERT_COND( network2.input_num() == ni );
  ASSERT_COND( input2_list.size() == ni );

  SizeType no = network1.output_num();
  ASSERT_COND( network2.output_num() == no );
  ASSERT_COND( output2_list.size() == no );

  // FraigMgr を初期化する．
  FraigMgr fraig_mgr(mSigSize, mSolverType);

  // 外部入力に対応する FraigHandle を作る．
  vector<FraigHandle> input1_handles(ni);
  for ( auto i: Range(ni) ) {
    input1_handles[i] = fraig_mgr.make_input();
  }

  // network1 に対応する Fraig を作る．
  auto output1_handles = fraig_mgr.import_subnetwork(network1, input1_handles);

  // network2 に対応する Fraig を作る．
  vector<FraigHandle> input2_handles(ni);
  for ( int i: Range(ni) ) {
    int pos1 = input2_list[i];
    input2_handles[i] = input1_handles[pos1];
  }
  auto output2_handles = fraig_mgr.import_subnetwork(network2, input2_handles);

  // 各出力の等価検証を行う．
  vector<SatBool3> eq_stats(no);
  SatBool3 stat = SatBool3::True;
  for ( auto i: Range(no) ) {
    if ( log_level() > 2 ) {
      log_out() << "Checking Output#" << (i + 1) << " / " << no << endl;
    }

    auto h1 = output1_handles[output2_list[i]];
    auto h2 = output2_handles[i];

    SatBool3 stat1;
    if ( h1 == h2 ) {
      stat1 = SatBool3::True;
    }
    else {
      stat1 = fraig_mgr.check_equiv(h1, h2);
    }
    eq_stats[i] = stat1;
    if ( stat1 == SatBool3::X ) {
      stat = SatBool3::X;
    }
    else if ( stat1 == SatBool3::False ) {
      if ( stat == SatBool3::True ) {
	stat = SatBool3::False;
      }
    }
    if ( log_level() > 2 ) {
      log_out() << "  => ";
      switch ( stat1 ) {
      case SatBool3::True:  log_out() << "Equivalent" << endl; break;
      case SatBool3::False: log_out() << "Not Equivalent" << endl; break;
      case SatBool3::X:     log_out() << "Unknown" << endl; break;
      }
    }
  }

  if ( log_level() > 1 ) {
    fraig_mgr.dump_stats(log_out());
  }

  return EquivResult(stat, eq_stats);
}

END_NAMESPACE_MAGUS
