
/// @file YmSatMS2.cc
/// @brief YmSatMS2 の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmSatMS2.h"


BEGIN_NAMESPACE_YM_SAT

#if 0
const
YmSatMS2::Params kDefaultParams(0.95, 0.999, false, 0.02, true, false, false);
#else
const
YmSatMS2::Params kDefaultParams(0.95, 0.999, false, 0.00, true, false, false);
#endif

//////////////////////////////////////////////////////////////////////
// YmSatMS2
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] option オプション文字列
YmSatMS2::YmSatMS2(const string& option) :
  YmSat(option),
  mParams(kDefaultParams)
{
  if ( option == "no_phase_cache" ) {
    mParams.mPhaseCache = false;
  }
}

// @brief デストラクタ
YmSatMS2::~YmSatMS2()
{
}

BEGIN_NONAMESPACE

// Luby restart strategy
double
luby(double y,
     int x)
{
  // なんのこっちゃわかんないコード
  int size;
  int seq;
  for (size = 1, seq = 0; size < x + 1; ++ seq, size = size * 2 + 1) ;

  while ( size - 1 != x ) {
    size = (size - 1) >> 1;
    -- seq;
    x = x % size;
  }

  return pow(y, seq);
}

END_NONAMESPACE

// @brief search() 用の条件パラメータの初期化を行う．
void
YmSatMS2::init_control_parameters()
{
  double restart_inc = 2.0;
  set_conflict_limit(static_cast<ymuint64>(luby(restart_inc, 0)) * 100);

  mLearntLimitD = clause_num() / 3.0;
  mLearntSizeAdjustConfl = 100.0;
  mLearntSizeAdjustInc = 1.5;
  mLearntSizeAdjustCount = static_cast<ymuint>(mLearntSizeAdjustConfl);
  set_learnt_limit(static_cast<ymuint64>(mLearntLimitD));
}

// @brief リスタート時に制御パラメータの更新を行う．
// @param[in] restart リスタート回数
void
YmSatMS2::update_on_restart(ymuint restart)
{
  double restart_inc = 2.0;
  set_conflict_limit(static_cast<ymuint64>(luby(restart_inc, restart)) * 100);
}

// @brief コンフリクト時に制御パラメータの更新を行う．
void
YmSatMS2::update_on_conflict()
{
  -- mLearntSizeAdjustCount;
  if ( mLearntSizeAdjustCount == 0 ) {
    mLearntSizeAdjustConfl *= mLearntSizeAdjustInc;
    mLearntSizeAdjustCount = static_cast<ymuint>(mLearntSizeAdjustConfl);
    mLearntLimitD *= 1.1;
    set_learnt_limit(static_cast<ymuint64>(mLearntLimitD));
  }
}

// 次の割り当てを選ぶ
Literal
YmSatMS2::next_decision()
{
  // 一定確率でランダムな変数を選ぶ．
  if ( mRandGen.real1() < mParams.mVarFreq && !var_heap().empty() ) {
    ymuint pos = mRandGen.int32() % variable_num();
    VarId vid(pos);
    if ( eval(vid) == kB3X ) {
      bool inv = mRandGen.real1() < 0.5;
      return Literal(vid, inv);
    }
  }

  while ( !var_heap().empty() ) {
    // activity の高い変数を取り出す．
    ymuint vindex = var_heap().pop_top();
    VarId vid(vindex);
    if ( eval(vid) != kB3X ) {
      // すでに確定していたらスキップする．
      // もちろん，ヒープからも取り除く．
      continue;
    }

    bool inv = false;
    if ( mParams.mPhaseCache ) {
      Bool3 val = old_val(vid);
      if ( val != kB3X ) {
	// 以前割り当てた極性を選ぶ
	if ( val == kB3False ) {
	  inv = true;
	}
	goto end;
      }
    }
    {
#if 0
      Literal plit(VarId(vindex), false);
      Literal nlit(VarId(vindex), true);
      if ( mParams.mWlPosi ) {
	// Watcher の多い方の極性を(わざと)選ぶ
	if ( watcher_list(nlit).num() >= watcher_list(plit).num() ) {
	  inv = true;
	}
      }
      else if ( mParams.mWlNega ) {
	// Watcher の少ない方の極性を選ぶ
	if ( watcher_list(nlit).num() < watcher_list(plit).num() ) {
	  inv = true;
	}
      }
      else {
	// mWlPosi/mWlNega が指定されていなかったらランダムに選ぶ．
	inv = mRandGen.real1() < 0.5;
      }
#else
      inv = true; // 意味はない．
#endif
    }
  end:
    return Literal(VarId(vindex), inv);
  }
  return kLiteralX;
}

BEGIN_NONAMESPACE
// reduce_learnt_clause で用いる SatClause の比較関数
class SatClauseLess
{
public:
  bool
  operator()(SatClause* a,
	     SatClause* b)
  {
    return a->activity() < b->activity();
  }
};
END_NONAMESPACE

// @brief 学習節の整理を行なう．
void
YmSatMS2::reduce_learnt_clause()
{
  vector<SatClause*>& lc_list = learnt_clause_list();

  ymuint n = lc_list.size();
  ymuint n2 = n / 2;

  // 足切りのための制限値
  double abs_limit = clause_bump() / n;

  // SatClauseLess を用いて学習節をソートする．
  sort(lc_list.begin(), lc_list.end(), SatClauseLess());

  // 前半の節は基本削除する．
  // 残す節は，
  // - binary clause (今の実装では SatClause の形にはなっていない)
  // - 現在の割当の理由となっている節
  ymuint wpos = 0;
  for (ymuint i = 0; i < n2; ++ i) {
    SatClause* clause = lc_list[i];
    if ( !is_locked(clause) ) {
      delete_clause(clause);
    }
    else {
      lc_list[wpos] = clause;
      ++ wpos;
    }
  }

  // 残りの節はアクティビティが規定値以下の節を削除する．
  // ただし，上と同じ例外はある．
  for (ymuint i = n2; i < n; ++ i) {
    SatClause* clause = lc_list[i];
    if ( !is_locked(clause) && clause->activity() < abs_limit ) {
      delete_clause(clause);
    }
    else {
      lc_list[wpos] = clause;
      ++ wpos;
    }
  }

  // vector を切り詰める．
  if ( wpos != lc_list.size() ) {
    lc_list.erase(lc_list.begin() + wpos, lc_list.end());
  }
}

END_NAMESPACE_YM_SAT
