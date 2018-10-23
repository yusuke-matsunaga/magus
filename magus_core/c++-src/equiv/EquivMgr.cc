
/// @file EquivMgr.cc
/// @brief EquivMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "EquivMgr.h"
#include "ym/FraigMgr.h"
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
EquivMgr::EquivMgr(int sig_size,
		   const SatSolverType& solver_type) :
  mSigSize(sig_size),
  mSolverType(solver_type),
  mLogLevel(0),
  mLogOutP(nullptr),
  mLoopLimit(10)
{
}

// @brief デストラクタ
EquivMgr::~EquivMgr()
{
}

// @brief 2つの回路が等価かどうか調べる．
// @param[in] network1 対象の回路1
// @param[in] network2 対象の回路2
// @param[out] eq_stats 各出力ごとの等価検証の結果
//
// 入力と出力の対応関係は順序で対応させる．
EquivResult
EquivMgr::check(const BnNetwork& network1,
		const BnNetwork& network2)
{
  int ni = network1.input_num();
  if ( network2.input_num() != ni ) {
    return SatBool3::False;
  }
  int no = network1.output_num();
  if ( network2.output_num() != no ) {
    return SatBool3::False;
  }

  vector<int> input2_list(ni);
  for ( int i: Range(ni) ) {
    input2_list[i] = i;
  }

  vector<int> output2_list(no);
  for ( int i: Range(no) ) {
    output2_list[i] = i;
  }

  return check(network1, network2, input2_list, output2_list);
}

// @brief 2つの回路が等価かどうか調べる．
// @param[in] network1 対象の回路1
// @param[in] network2 対象の回路2
// @param[in] input2_list network2の入力順序を表すリスト
// @param[in] output2_list network2の出力順序を表すリスト
//
// input2_list, output2_list ともに，network2 の i 番目の
// 入力(出力)が network1 の何番目の入力(出力)に対応しているか
// を示す．
EquivResult
EquivMgr::check(const BnNetwork& network1,
		const BnNetwork& network2,
		const vector<int>& input2_list,
		const vector<int>& output2_list)
{
  // 最初に入出力数が等しいか調べる．
  int ni = network1.input_num();
  ASSERT_COND( network2.input_num() == ni );
  ASSERT_COND( input2_list.size() == ni );

  int no = network1.output_num();
  ASSERT_COND( network2.output_num() == no );
  ASSERT_COND( output2_list.size() == no );

  // FraigMgr を初期化する．
  FraigMgr fraig_mgr(mSigSize, mSolverType);

  // 外部入力に対応する FraigHandle を作る．
  vector<FraigHandle> input_handles(ni);
  for ( auto i: Range(ni) ) {
    input_handles[i] = fraig_mgr.make_input();
  }

  // network1 に対応する Fraig を作る．
  vector<FraigHandle> output_handles1(no);
  fraig_mgr.import_subnetwork(network1, input_handles, output_handles1);

  // network2 に対応する Fraig を作る．
  vector<FraigHandle> output_handles2(no);
  {
    vector<FraigHandle> input2_handles(ni);
    for ( int i: Range(ni) ) {
      int pos1 = input2_list[i];
      input2_handles[i] = input_handles[pos1];
    }
    fraig_mgr.import_subnetwork(network2, input2_handles, output_handles2);
  }

  // 各出力の等価検証を行う．
  vector<SatBool3> eq_stats(no);
  SatBool3 stat = SatBool3::True;
  for ( auto i: Range(no) ) {
    if ( log_level() > 2 ) {
      log_out() << "Checking Output#" << (i + 1) << " / " << no << endl;
    }

    auto h1 = output_handles1[output2_list[i]];
    auto h2 = output_handles2[i];

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
