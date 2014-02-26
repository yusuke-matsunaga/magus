#ifndef RTPGOLD_H
#define RTPGOLD_H

/// @file RtpgOld.h
/// @brief RtpgOld のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "Rtpg.h"
#include "utils/RandGen.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class RtpgOld RtpgOld.h "RtpgOld.h"
/// @brief FsimOld を使う Rtpg
//////////////////////////////////////////////////////////////////////
class RtpgOld :
  public Rtpg
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] tvlist テストベクタのリスト
  /// @param[in] fsim 故障シミュレータ
  RtpgOld(FaultMgr& fmgr,
	  TvMgr& tvmgr,
	  vector<TestVector*>& tvlist,
	  FsimOld& fsim);

  /// @brief デストラクタ
  virtual
  ~RtpgOld();


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
  /// @param[in] min_f 1回のシミュレーションで検出する故障数の下限
  /// @param[in] max_i 故障検出できないシミュレーション回数の上限
  /// @param[in] max_pat 最大のパタン数
  /// @param[in] stats 実行結果の情報を格納する変数
  virtual
  void
  run(ymuint min_f,
      ymuint max_i,
      ymuint max_pat,
      RtpgStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障マネージャ
  FaultMgr& mFaultMgr;

  // テストベクタマネージャ
  TvMgr& mTvMgr;

  // テストベクタのリスト
  vector<TestVector*>& mTvList;

  // 故障シミュレータ
  FsimOld& mFsim;

  // 乱数生成器
  RandGen mRandGen;

};

END_NAMESPACE_YM_SATPG

#endif // RTPGOLD_H
