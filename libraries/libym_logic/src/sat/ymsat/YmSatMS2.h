#ifndef YMSATMS2_H
#define YMSATMS2_H

/// @file YmSatMS2.h
/// @brief YmSatMS2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmSat.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class YmSatMS2 YmSatMS2.h "YmSatMS2.h"
/// @brief MiniSat2 もどきの YmSat
//////////////////////////////////////////////////////////////////////
class YmSatMS2 :
  public YmSat
{
public:
  //////////////////////////////////////////////////////////////////////
  /// @class YmSatMS2::Params YmSat.h "YmSat.h"
  /// @brief YmSatMS2 の挙動を制御するパラメータ
  //////////////////////////////////////////////////////////////////////
  struct Params :
    public YmSat::Params
  {

    /// @brief 変数選択用の閾値
    double mVarFreq;

    /// @brief phase-cache ヒューリスティックを使うとき true
    bool mPhaseCache;

    /// @brief watcher list の多い極性を選ぶヒューリスティックを使うとき true
    bool mWlPosi;

    /// @brief watcher list の少ない極性を選ぶヒューリスティックを使うとき true
    bool mWlNega;

    /// @brief コンストラクタ
    Params() :
      mVarFreq(0.0),
      mPhaseCache(true),
      mWlPosi(false),
      mWlNega(false)
    {
    }

    /// @brief 値を指定したコンストラクタ
    Params(double var_decay,
	   double clause_decay,
	   bool use_lbd,
	   double var_freq,
	   bool phase_cache,
	   bool wl_posi,
	   bool wl_nega) :
      YmSat::Params(var_decay, clause_decay, use_lbd),
      mVarFreq(var_freq),
      mPhaseCache(phase_cache),
      mWlPosi(wl_posi),
      mWlNega(!wl_posi && wl_nega)
    {
    }

  };


public:

  /// @brief コンストラクタ
  /// @param[in] option オプション文字列
  YmSatMS2(const string& option = string());

  /// @brief デストラクタ
  virtual
  ~YmSatMS2();


public:
  //////////////////////////////////////////////////////////////////////
  // SatSolver で定義されている仮想関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // 実装用のプライベート関数
  //////////////////////////////////////////////////////////////////////

  /// @brief search() 用の条件パラメータの初期化を行う．
  virtual
  void
  init_control_parameters();

  /// @brief リスタート時に制御パラメータの更新を行う．
  /// @param[in] restart リスタート回数
  virtual
  void
  update_on_restart(ymuint restart);

  /// @brief コンフリクト時に制御パラメータの更新を行う．
  virtual
  void
  update_on_conflict();

  /// @brief 次の割り当てを選ぶ．
  /// @note 割り当てられる変数がない場合には kLiteralX を返す．
  virtual
  Literal
  next_decision();

  /// @brief 学習節の整理を行なう．
  ///
  /// 実際には条件が成り立った時のみ整理する．
  virtual
  void
  reduce_learnt_clause();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 制御用のパラメータ
  Params mParams;

  // 変数選択用乱数発生器
  RandGen mRandGen;

  // mLearntLimit の計算用の値
  double mLearntLimitD;

  // mLearntLimit 用のパラメータ
  // mLearntSizeAdjustCount の初期値
  double mLearntSizeAdjustConfl;

  // mLearntLimit 用のパラメータ
  // mLearntSizeAdjustConfl の増加量
  double mLearntSizeAdjustInc;

  // mLearntLimit 用のパラメータ
  // 矛盾の数がこの回数になった時に mLearntLimit を更新する．
  ymuint64 mLearntSizeAdjustCount;

};

END_NAMESPACE_YM_SAT

#endif // YMSATMS2_H
