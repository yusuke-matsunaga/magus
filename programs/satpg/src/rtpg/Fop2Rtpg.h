#ifndef FOP2RTPG_H
#define FOP2RTPG_H

/// @file Fop2Rtpg.h
/// @brief Fop2Rtpg のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Fop2Rtpg Fop2Rtpg.h "Fop2Rtpg.h"
/// @brief Rtpg で用いる FsimOp1
//////////////////////////////////////////////////////////////////////
class Fop2Rtpg :
  public FsimOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fmgr 故障マネージャ
  Fop2Rtpg(Fsim& fsim,
	   FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~Fop2Rtpg();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(TpgFault* f,
	     PackedVal dpat);

  /// @brief 検出回数をクリアする．
  void
  clear_count();

  /// @brief 検出回数を得る．
  /// @param[in] bitpos ビット位置
  ymuint
  count(ymuint bitpos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 故障マネージャ
  FaultMgr& mFaultMgr;

  // 検出回数
  ymuint32 mCount[kPvBitLen];

};

END_NAMESPACE_YM_SATPG

#endif // FOP1RTPG_H
