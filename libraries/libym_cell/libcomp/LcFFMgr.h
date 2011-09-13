#ifndef LCFFMGR_H
#define LCFFMGR_H

/// @file LcFFMgr.h
/// @brief LcFFMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "LcGroupMgr.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcFFMgr LcFFMgr.h "LcFFMgr.h"
/// @brief FFセルを表すクラス
//////////////////////////////////////////////////////////////////////
class LcFFMgr :
  public LcGroupMgr
{
public:

  /// @brief コンストラクタ
  LcFFMgr();

  /// @brief デストラクタ
  ~LcFFMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
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

#endif // LCFFMGR_H
