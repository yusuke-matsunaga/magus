#ifndef OPDETECT_H
#define OPDETECT_H

/// @file DopDrop.h
/// @brief DopDrop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"
#include "FopDrop.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DopDrop DopDrop.h "DopDrop.h"
/// @brief 故障シミュレーションを行なった故障ドロップを行なうクラス
//////////////////////////////////////////////////////////////////////
class DopDrop :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  DopDrop(FaultMgr& fmgr,
	  Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~DopDrop();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] assign_list 値割当のリスト
  virtual
  void
  operator()(const TpgFault* f,
	     const NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障マネージャ
  FaultMgr& mMgr;

  // 故障シミュレータ
  Fsim& mFsim;

  // Fsim 用のファンクタ
  FopDrop mOp;

};

END_NAMESPACE_YM_SATPG

#endif // OPDETECT_H
