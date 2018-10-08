
/// @file EquivMgr.cc
/// @brief EquivMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "EquivMgr.h"
#include "FraigMgr.h"
#include "Bn2FraigConv.h"
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
// @retval SatBool3::True 等価
// @retval SatBool3::False 非等価
// @retval SatBool3::X アボート(不明)
//
// 入力と出力の対応関係は順序で対応させる．
SatBool3
EquivMgr::check(const BnNetwork& network1,
		const BnNetwork& network2,
		vector<SatBool3>& eq_stats)
{
  int ni = network1.input_num();
  if ( network2.input_num() != ni ) {
    return SatBool3::False;
  }
  int no = network1.output_num();
  if ( network2.output_num() != no ) {
    return SatBool3::False;
  }

  vector<int> input1_list(ni);
  vector<int> input2_list(ni);
  for ( int i: Range(ni) ) {
    input1_list[i] = network1.input_id(i);
    input2_list[i] = network2.input_id(i);
  }

  vector<int> output1_list(no);
  vector<int> output2_list(no);
  for ( int i: Range(no) ) {
    output1_list[i] = network1.output_id(i);
    output2_list[i] = network2.output_id(i);
  }

  return check(network1, input1_list, output1_list,
	       network2, input2_list, output2_list,
	       eq_stats);
}

// @brief 2つの回路が等価かどうか調べる．
// @param[in] network1 対象の回路1
// @param[in] input1_list 入力ノード番号のリスト
// @param[in] output1_list 出力ノード番号のリスト
// @param[in] network2 対象の回路2
// @param[in] input2_list 入力ノード番号のリスト
// @param[in] output2_list 出力ノード番号のリスト
// @param[out] eq_stats 各出力ごとの等価検証の結果
// @retval SatBool3::True 等価
// @retval SatBool3::False 非等価
// @retval SatBool3::X アボート(不明)
SatBool3
EquivMgr::check(const BnNetwork& network1,
		const vector<int>& input1_list,
		const vector<int>& output1_list,
		const BnNetwork& network2,
		const vector<int>& input2_list,
		const vector<int>& output2_list,
		vector<SatBool3>& eq_stats)
{
  using namespace nsEquiv;

  // 最初に入出力数が等しいか調べる．
  int ni = input1_list.size();
  {
    ASSERT_COND( input2_list.size() == ni );
    ASSERT_COND( network1.input_num() == ni );
    ASSERT_COND( network2.input_num() == ni );
  }
  int no = output1_list.size();
  {
    ASSERT_COND( output2_list.size() == no );
    ASSERT_COND( network1.output_num() == no );
    ASSERT_COND( network2.output_num() == no );
  }

  // FraigMgr を初期化する．
  FraigMgr fraig_mgr(mSigSize, mSolverType);

  // 外部入力に対応する FraigHandle を作る．
  vector<FraigHandle> input_handles(ni);
  for ( auto i: Range(ni) ) {
    input_handles[i] = fraig_mgr.make_input();
  }


  // network1 に対応する Fraig を作る．
  Bn2FraigConv convert1(network1, fraig_mgr);
  vector<FraigHandle> output_handles1(no);
  convert1(input1_list, output1_list, input_handles, output_handles1);

  // network2 に対応する Fraig を作る．
  Bn2FraigConv convert2(network2, fraig_mgr);
  vector<FraigHandle> output_handles2(no);
  convert2(input2_list, output2_list, input_handles, output_handles2);

  // 各出力の等価検証を行う．
  eq_stats.clear();
  eq_stats.resize(no);
  SatBool3 stat = SatBool3::True;
  for ( auto i: Range(no) ) {
    if ( log_level() > 2 ) {
      log_out() << "Checking Output#" << (i + 1) << " / " << no << endl;
    }

    auto h1 = output_handles1[i];
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

  return stat;
}

// @brief ログレベルを設定する．
// @param[in] level 設定するログレベル
void
EquivMgr::set_loglevel(int level)
{
  mLogLevel = level;
}

// @brief ログ出力用ストリームを設定する．
// @param[in] out ストリームのポインタ
void
EquivMgr::set_logstream(ostream* out)
{
  mLogOutP = out;
}

// @brief ランダムシミュレーション制御用のパラメータを設定する．
// @param[in] loop_limit 変化のない状態がこの回数連続したら止める．
void
EquivMgr::set_loop_limit(int loop_limit)
{
  mLoopLimit = loop_limit;
}

// @brief ログレベルを返す．
int
EquivMgr::log_level() const
{
  return mLogLevel;
}

// @brief ログの出力先を返す．
ostream&
EquivMgr::log_out() const
{
  return *mLogOutP;
}

END_NAMESPACE_MAGUS
