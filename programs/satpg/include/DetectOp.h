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

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] assign_list 値割当のリスト
  virtual
  void
  operator()(TpgFault* f,
	     const AssignList& assign_list) = 0;

};

/// @brief 'base' タイプを生成する．
/// @param[in] fmgr 故障マネージャ
DetectOp*
new_DopBase(FaultMgr& fmgr);

/// @brief 'drop' タイプを生成する．
/// @param[in] fmgr 故障マネージャ
/// @param[in] fsim 故障シミュレータ
DetectOp*
new_DopDrop(FaultMgr& fmgr,
	    Fsim& fsim);

/// @brief 'tvlist' タイプを生成する．
/// @param[in] tvmgr テストベクタのマネージャ
/// @param[in] tvlist テストベクタのリスト
DetectOp*
new_DopTvList(TvMgr& tvmgr,
	      vector<TestVector*>& tvlist);

/// @brief 'verify' タイプを生成する．
/// @param[in] fsim 故障シミュレータ
DetectOp*
new_DopVerify(Fsim& fsim);

/// @brief 'dummy' タイプを生成する．
DetectOp*
new_DopDummy();

END_NAMESPACE_YM_SATPG

#endif // DETECTOP_H
