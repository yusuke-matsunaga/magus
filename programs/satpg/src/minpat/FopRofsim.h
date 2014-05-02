#ifndef FOPROFSIM_H
#define FOPROFSIM_H

/// @file FopRofsim.h
/// @brief FopRofsim のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FopRofsim FopRofsim.h "FopRofsim.h"
/// @brief Rofsim で用いる FsimOp2
//////////////////////////////////////////////////////////////////////
class FopRofsim :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] f_list 故障のリスト
  /// @param[in] max_fault_id 故障IDの最大値+1
  /// @param[in] fmgr 故障マネージャ
  FopRofsim(Fsim& fsim,
	    const vector<TpgFault*>& f_list,
	    ymuint max_fault_id);


  /// @brief デストラクタ
  virtual
  ~FopRofsim();


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
