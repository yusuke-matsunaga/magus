#ifndef DETECTOP_H
#define DETECTOP_H

/// @file DetectOp.h
/// @brief DetectOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DetectOp DetectOp.h "DetectOp.h"
/// @brief テストパタンが見つかったときの処理を行なうファンクタ
//////////////////////////////////////////////////////////////////////
class DetectOp
{
public:

  /// @brief デストラクタ
  virtual
  ~DetectOp() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障リストを設定する．
  /// @param[in] fault_list 故障リスト
  virtual
  void
  set_faults(const vector<TpgFault*>& fault_list) = 0;

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] tv テストパタン
  virtual
  void
  operator()(TpgFault* f,
	     TestVector* tv) = 0;

};

/// @brief 'base' タイプを生成する．
/// @param[in] mgr AtpgMgr
DetectOp*
new_DopBase(AtpgMgr& mgr);

/// @brief 'drop' タイプを生成する．
/// @param[in] mgr AtpgMgr
DetectOp*
new_DopDrop(AtpgMgr& mgr);

/// @brief 'tvlist' タイプを生成する．
/// @param[in] mgr AtpgMgr
DetectOp*
new_DopTvList(AtpgMgr& mgr);

/// @brief 'verify' タイプを生成する．
DetectOp*
new_DopVerify(AtpgMgr& mgr);

END_NAMESPACE_YM_SATPG

#endif // DETECTOP_H
