#ifndef LIBYM_TECHMAP_PATGEN_PGCELLMGR_H
#define LIBYM_TECHMAP_PATGEN_PGCELLMGR_H

/// @file libym_techmap/patgen/PgCellMgr.h
/// @brief PgCellMgr.h
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "patgen_nsdef.h"
#include "PatGen.h"
#include "ym_lexp/lexp_nsdef.h"
#include "ym_npn/npn_nsdef.h"


BEGIN_NAMESPACE_YM_TECHMAP_PATGEN

class PgCellGroup;

//////////////////////////////////////////////////////////////////////
/// @class PgCellMgr PgCellMgr.h "PgCellMgr.h"
/// @brief セルライブラリを表すクラス
//////////////////////////////////////////////////////////////////////
class PgCellMgr
{
public:

  /// @brief コンストラクタ
  PgCellMgr();

  /// @brief デストラクタ
  ~PgCellMgr();


public:

  /// @brief セルを追加する場所を求める．
  PgCellHandle
  new_cell(const LogExpr& expr);

  /// @brief 代表グループ数を得る．
  ymuint
  rep_num() const;

  /// @brief 代表グループを取り出す．
  /// @param[in] pos 一番番号 ( 0 <= pos < rep_num() )
  const PgCellRep*
  rep(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  //下請け関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルグループを見つける．
  /// @param[in] f 論理関数
  /// @return f を等価な関数を持つセルグループを返す．
  /// @note なければ新たに作る．
  PgCellGroup*
  find_group(const TvFunc& f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // パタングラフを管理するクラス
  PatGen mPatGen;

  // 代表グループのリスト
  vector<PgCellRep*> mRepList;

};

END_NAMESPACE_YM_TECHMAP_PATGEN

#endif // LIBYM_TECHMAP_PATGEN_PGCELLMGR_H
