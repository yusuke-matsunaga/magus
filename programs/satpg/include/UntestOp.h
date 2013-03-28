#ifndef UNTESTOP_H
#define UNTESTOP_H

/// @file UntestOp.h
/// @brief UntestOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class UntestOp UntestOp.h "UntestOp.h"
/// @brief テスト不能故障と判定された時の処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class UntestOp
{
public:

  /// @brief デストラクタ
  virtual
  ~UntestOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト不能故障と判定された時の処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f) = 0;

};

/// @brief 'base' タイプを生成する．
/// @param[in] mgr AtpgMgr
UntestOp*
new_UopBase(AtpgMgr& mgr);

/// @brief 'skip' タイプを生成する．
/// @param[in] mgr AtpgMgr
/// @param[in] threshold しきい値
UntestOp*
new_UopSkip(AtpgMgr& mgr,
	    ymuint threshold);


END_NAMESPACE_YM_SATPG

#endif // UNTESTOP_H
