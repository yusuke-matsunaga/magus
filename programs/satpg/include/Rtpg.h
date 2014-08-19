#ifndef RTPG_H
#define RTPG_H

/// @file Rtpg.h
/// @brief Rtpg のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Rtpg Rtpg.h "Rtpg.h"
/// @brief RTPG を行う基底クラス
//////////////////////////////////////////////////////////////////////
class Rtpg
{
public:

  /// @brief デストラクタ
  virtual
  ~Rtpg() {}

public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 乱数生成器を初期化する．
  /// @param[in] seed 乱数の種
  virtual
  void
  init(ymuint32 seed) = 0;

  /// @brief RTPGを行なう．
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
  /// @param[in] max_i 故障検出できないシミュレーション回数の上限
  /// @param[in] max_pat 最大のパタン数
  /// @param[in] tvlist テストベクタのリスト
  /// @param[in] stats 実行結果の情報を格納する変数
  virtual
  void
  run(FaultMgr& fmgr,
      TvMgr& tvmgr,
      Fsim& fsim,
      ymuint min_f,
      ymuint max_i,
      ymuint max_pat,
      vector<TestVector*>& tvlist,
      RtpgStats& stats) = 0;

};


/// @brief Rtpg のインスタンスを生成する．
extern
Rtpg*
new_Rtpg();

END_NAMESPACE_YM_SATPG

#endif // RTPG_H
