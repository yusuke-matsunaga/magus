#ifndef RTPGIMPL_H
#define RTPGIMPL_H

/// @file RtpgImpl.h
/// @brief RtpgImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Rtpg.h"
#include "YmUtils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class RtpgImpl RtpgImpl.h "RtpgImpl.h"
/// @brief Fsim を使う Rtpg
//////////////////////////////////////////////////////////////////////
class RtpgImpl :
  public Rtpg
{
public:

  /// @brief コンストラクタ
  RtpgImpl();

  /// @brief デストラクタ
  virtual
  ~RtpgImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 乱数生成器を初期化する．
  /// @param[in] seed 乱数の種
  virtual
  void
  init(ymuint32 seed);

  /// @brief RTPGを行なう．
  /// @param[in] fault_list 故障リスト
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
  /// @param[in] max_i 故障検出できないシミュレーション回数の上限
  /// @param[in] max_pat 最大のパタン数
  /// @param[out] det_fault_list 検出された故障のリスト
  /// @param[out] tvlist テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(const vector<const TpgFault*>& fault_list,
      TvMgr& tvmgr,
      Fsim& fsim,
      ymuint min_f,
      ymuint max_i,
      ymuint max_pat,
      vector<const TpgFault*>& det_fault_list,
      vector<TestVector*>& tvlist,
      RtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数生成器
  RandGen mRandGen;

};

END_NAMESPACE_YM_SATPG

#endif // RTPGIMPL_H
