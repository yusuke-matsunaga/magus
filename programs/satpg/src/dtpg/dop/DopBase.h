#ifndef DOPBASE_H
#define DOPBASE_H

/// @file DopBase.h
/// @brief DopBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DopBase DopBase.h "DopBase.h"
/// @brief DetectOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class DopBase :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr FaultMgr
  DopBase(FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~DopBase();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] assign_list 値割当のリスト
  virtual
  void
  operator()(TpgFault* f,
	     const NodeValList& assign_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FaultMgr
  FaultMgr& mMgr;

};

END_NAMESPACE_YM_SATPG

#endif // DOPBASE_H
