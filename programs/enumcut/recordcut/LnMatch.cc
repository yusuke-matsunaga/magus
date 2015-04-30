
/// @file LnMatch.cc
/// @brief LnMatch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "LnMatch.h"


BEGIN_NAMESPACE_YM

BEGIN_NONAMESPACE

bool debug = false;

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス LnMatch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
LnMatch::LnMatch()
{
}

// @brief デストラクタ
LnMatch::~LnMatch()
{
  clear();
}

// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
// @param[in] func 対象の関数
// @param[in] solver GBM ソルバ
bool
LnMatch::match(const TvFunc& func,
	       GbmSolver& solver)
{
  ymuint ni = func.input_num();

  // まず関数の真のサポートを求める．
  vector<ymuint> sup_list;
  sup_list.reserve(ni);
  for (ymuint i = 0; i < ni; ++ i) {
    if ( func.check_sup(VarId(i)) ) {
      sup_list.push_back(i);
    }
  }

  TvFunc func1 = func;
  ymuint ni1 = sup_list.size();
  if ( debug ) {
    cout << " ni1 = " << ni1 << endl;
  }
  if ( ni1 < ni ) {
    // 冗長な入力があった．
    ymuint nexp1 = (1U << ni1);
    vector<int> new_tv(nexp1);
    for (ymuint b = 0; b < nexp1; ++ b) {
      ymuint pos = 0;
      for (ymuint i = 0; i < ni1; ++ i) {
	if ( b & (1U << i) ) {
	  pos += (1U << sup_list[i]);
	}
      }
      new_tv[b] = func.value(pos);
    }
    func1 = TvFunc(ni1, new_tv);
  }

  if ( ni1 < mMinInputs ) {
    // 自明
    ++ mTrivialNum;
    return true;
  }

  if ( ni1 > mMaxInputs ) {
    // ないと思うけど念の為
    return false;
  }

  vector<bool> conf_bits;
  vector<ymuint> iorder;

  const vector<RcfTempl*>& templ_list = mTemplList[ni1 - mMinInputs];
  for (ymuint i = 0; i < templ_list.size(); ++ i) {
    RcfTempl& templ = *templ_list[i];
    ymuint loop_count;
    bool stat = solver.solve(*templ.mNetwork, func1, conf_bits, iorder, loop_count);
    templ.mLoopCount += loop_count;
    if ( stat ) {
      if ( debug ) {
	cout << "Type A-4 Match" << endl;
	cout << "  iorder = ";
	for (ymuint i = 0; i < ni1; ++ i) {
	  cout << " " << iorder[i];
	}
	cout << endl;
      }
      ++ templ.mCount;
      return true;
    }
  }

  if ( debug ) {
    cout << "Not found" << endl;
  }
  ++ mFailNum;
  return false;
}

// @brief カウンタをリセットする．
void
LnMatch::reset_count()
{
  mTrivialNum = 0;
  mFailNum = 0;
  for (ymuint i = 0; i < mAllList.size(); ++ i) {
    mAllList[i]->mCount = 0;
  }
}

// @brief テンプレート数を得る．
ymuint
LnMatch::templ_num() const
{
  return mAllList.size();
}

// @brief テンプレートのカウンタ値を得る．
// @param[in] pos テンプレート番号 ( 0 <= pos < templ_num() )
ymuint
LnMatch::get_templ_count(ymuint pos) const
{
  ASSERT_COND( pos < templ_num() );
  return mAllList[pos]->mCount;
}

// @brief テンプレートのループカウンタ値を得る．
// @param[in] pos テンプレート番号 ( 0 <= pos < templ_num() )
ymuint
LnMatch::get_templ_loop_count(ymuint pos) const
{
  ASSERT_COND( pos < templ_num() );
  return mAllList[pos]->mLoopCount;
}

// @brief カウンタの値を得る．
void
LnMatch::get_count(ymuint& trival_num,
		   ymuint& fail_num)
{
  trival_num = mTrivialNum;
  fail_num = mFailNum;
}

// @brief 入力数の範囲を指定する．
void
LnMatch::set_input_range(ymuint min_inputs,
			 ymuint max_inputs)
{
  clear();
  mMinInputs = min_inputs;
  mMaxInputs = max_inputs;
  ymuint n = mMaxInputs - mMinInputs + 1;
  mTemplList.resize(n);
}

// @brief テンプレートを追加する．
void
LnMatch::add_templ(ymuint input_num,
		   RcfNetwork* network)
{
  ASSERT_COND( input_num >= mMinInputs );
  ASSERT_COND( input_num <= mMaxInputs );
  RcfTempl* templ = new RcfTempl;
  templ->mNetwork = network;
  templ->mCount = 0;
  templ->mLoopCount = 0;
  mAllList.push_back(templ);
  mTemplList[input_num - mMinInputs].push_back(templ);
}

// @brief 内部のデータを消去する．
void
LnMatch::clear()
{
  for (ymuint i = 0; i < mAllList.size(); ++ i) {
    delete mAllList[i]->mNetwork;
    delete mAllList[i];
  }
  mAllList.clear();
  mTemplList.clear();
}

END_NAMESPACE_YM
