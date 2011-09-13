#ifndef LCLOGICMGR_H
#define LCLOGICMGR_H

/// @file LcLogicMgr.h
/// @brief LcLogicMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcGroupMgr.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcLogicMgr LcLogicMgr.h "LcLogicMgr.h"
/// @brief 論理セルのグループ分けを行うクラス
//////////////////////////////////////////////////////////////////////
class LcLogicMgr :
  public LcGroupMgr
{
public:

  /// @brief コンストラクタ
  LcLogicMgr();

  /// @brief デストラクタ
  ~LcLogicMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  /// @note 定数関数，リテラル関数を登録しておく
  ///
  /// 常に定数0，定数1，肯定リテラル，否定リテラルのグループ番号が
  /// 0, 1, 2, 3 になるようにする．
  void
  init();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルのシグネチャ関数を作る．
  /// @param[in] cell セル
  /// @param[out] f シグネチャ関数
  virtual
  void
  gen_signature(const Cell* cell,
		TvFuncM& f);

  /// @brief 代表関数を求める．
  /// @param[in] f 関数
  /// @param[out] repfunc 代表関数
  /// @param[out] xmap 変換
  virtual
  void
  find_repfunc(const TvFuncM& f,
	       TvFuncM& repfunc,
	       NpnMapM& xmap);

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCLOGICMGR_H
