#ifndef LCLATCHMGR_H
#define LCLATCHMGR_H

/// @file LcLatchMgr.h
/// @brief LcLatchMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LcGroupMgr.h"


BEGIN_NAMESPACE_YM_CELL_LIBCOMP

//////////////////////////////////////////////////////////////////////
/// @class LcLatchMgr LcLatchMgr.h "LcLatchMgr.h"
/// @brief FFセルを表すクラス
//////////////////////////////////////////////////////////////////////
class LcLatchMgr :
  public LcGroupMgr
{
public:

  /// @brief コンストラクタ
  /// @param[in] libcomp 親の LibComp
  LcLatchMgr(LibComp& libcomp);

  /// @brief デストラクタ
  ~LcLatchMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスのメインの関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 初期化する．
  void
  init();

  /// @brief 定義済みのラッチクラス番号を返す．
  /// @param[in] id 番号
  /// - 0: クリアなし，プリセットなし
  /// - 1: クリアあり，プリセットなし
  /// - 2: クリアなし，プリセットあり
  /// - 3: クリアあり，プリセットあり
  ymuint
  latch_class(ymuint id) const;


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

  /// @brief 同位体変換リストを求める．
  /// @param[in] func 対象の関数
  /// @param[out] idmap_list 同位体変換のリスト
  virtual
  void
  find_idmap_list(const TvFuncM& func,
		  vector<NpnMapM>& idmap_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 定義済みのクラス番号
  ymuint32 mLatchClass[4];

};

END_NAMESPACE_YM_CELL_LIBCOMP

#endif // LCLATCHMGR_H
