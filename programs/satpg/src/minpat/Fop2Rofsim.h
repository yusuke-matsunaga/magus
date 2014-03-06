#ifndef FOP2ROFSIM_H
#define FOP2ROFSIM_H

/// @file Fop2Rofsim.h
/// @brief Fop2Rofsim のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp2.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Fop2Rofsim Fop2Rofsim.h "Fop2Rofsim.h"
/// @brief Rofsim で用いる FsimOp2
//////////////////////////////////////////////////////////////////////
class Fop2Rofsim :
  public FsimOp2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] fmgr 故障マネージャ
  Fop2Rofsim(Fsim& fsim,
	     FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~Fop2Rofsim();


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

  /// @brief 今のパタンで新たに故障検出が行えたかを表すビットベクタを返す．
  PackedVal
  det_bits();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 故障の検出状態を示すフラグの配列
  // 故障のIDでインデクシングされている．
  vector<bool> mDetArray;

  // 故障検出を行ったビット位置を表すビットベクタ
  PackedVal mDetBits;

};

END_NAMESPACE_YM_SATPG

#endif // FOP1ROFSIM_H
