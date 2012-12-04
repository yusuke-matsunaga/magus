#ifndef DTPG_H
#define DTPG_H

/// @file Dtpg.h
/// @brief Dtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "dtpg_nsdef.h"
#include "TvBuff.h"
#include "ym_utils/USTime.h"
#include "ym_utils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

class TestVector;
class SaFault;

//////////////////////////////////////////////////////////////////////
/// @class DtpgStats Dtpg.h "Dtpg.h"
/// @brief DTPG の statistics data を表すクラス
//////////////////////////////////////////////////////////////////////
class DtpgStats
{
  friend class Dtpg;

public:

  /// @brief コンストラクタ
  DtpgStats();

  /// @brief デストラクタ
  ~DtpgStats();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 検出した故障数を得る．
  ymuint
  detected_faults() const;

  /// @brief 検出不能と判断した故障数を得る．
  ymuint
  untestable_faults() const;

  /// @brief アボートした故障数を得る．
  ymuint
  aborted_faults() const;

  /// @brief 生成したパタン数を得る．
  ymuint
  generated_patterns() const;

  /// @brief 起動回数を得る．
  ymuint
  call_count() const;

  /// @brief バックトラック数を得る．
  ymuint64
  backtrack_count() const;

  /// @brief 計算時間を得る．
  USTime
  time() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 検出した故障数
  ymuint32 mDetectNum;

  // 検出不能な故障数
  ymuint32 mUntestNum;

  // アボート故障数
  ymuint32 mAbortNum;

  // パタン数
  ymuint32 mPatNum;

  // 起動回数
  ymuint32 mDtpgNum;

  // バックトラック数
  ymuint64 mBtNum;

  // 計算時間
  USTime mTime;

};


//////////////////////////////////////////////////////////////////////
/// @class Dtpg Dtpg.h "Dtpg.h"
/// @brief DTPG を行うクラス
//////////////////////////////////////////////////////////////////////
class Dtpg
{
public:

  /// @brief コンストラクタ
  Dtpg(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~Dtpg();


public:

  /// @brief 未検出の故障に対してテストパタン生成を行う．
  /// @param[in] sim_mode シミュレーションモード
  void
  operator()(tSimMode sim_mode);

  /// @brief 直前の実行結果を返す．
  const DtpgStats&
  stats() const;


public:
  //////////////////////////////////////////////////////////////////////
  // バックトラック制限数に関するアクセス関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 現在のバックトラック制限数を得る．
  ymuint
  backtrack_limit() const;

  /// @brief バックトラック制限数を設定する．
  /// @param[in] limit 設定する制限数
  void
  set_backtrack_limit(ymuint limit);

  /// @brief デフォルトのバックトラック制限数を設定する．
  void
  set_default_backtrack_limit();


public:
  //////////////////////////////////////////////////////////////////////
  // SAT に関する制御
  //////////////////////////////////////////////////////////////////////

  /// @brief SAT エンジンの指定を行う．
  void
  set_sat_mode(int mode);


private:
  //////////////////////////////////////////////////////////////////////
  // 下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 未検出の故障に対してテストパタン生成を行う．
  /// @param[in] sim_mode シミュレーションモード
  void
  dtpg2(tSimMode sim_mode);

  /// @brief f に対するテストパタン生成を行う．
  /// @return パタンを返す．
  TestVector*
  dtpg_sat(SaFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // AtpgMgr
  AtpgMgr* mAtpgMgr;

  // SAT を用いてテストパタン生成を行うクラス
  DtpgSat* mDtpgSat;

  // テストベクタのバッファ
  TvBuff mTvBuff;

  // 対象の故障を保持するリスト
  vector<SaFault*> mCurFaults;

  // テストベクタを格納する作業領域
  TestVector* mTmpTv;

  // パタン生成用の乱数生成器
  RandGen mPatGen;

  // 故障選択用の乱数生成器
  RandGen mFselGen;

  // 実行結果
  DtpgStats mStats;

};

END_NAMESPACE_YM_SATPG

#endif // DTPG_H
