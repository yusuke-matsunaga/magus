
/// @file libym_techmap/TechMap.cc
/// @brief TechMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/TechMap.h"
#include "CellMgr.h"
#include "AreaCover.h"

#include "patgen/patgen_nsdef.h"
#include "patgen/PgFuncMgr.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
TechMap::TechMap() :
  mCellMgr(NULL)
{
}

// @brief デストラクタ
TechMap::~TechMap()
{
  delete mCellMgr;
}

// @brief セルライブラリのデータを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
TechMap::load_library(istream& s)
{
  delete mCellMgr;
  mCellMgr = new CellMgr;
  bool stat = mCellMgr->load(s);
  if ( !stat ) {
    delete mCellMgr;
    mCellMgr = NULL;
  }
  return stat;
}

// @brief セルライブラリの内容(+パタングラフ)をバイナリファイルに書き出す．
void
TechMap::dump_library(ostream& s,
		      const CellLibrary& library)
{
  using namespace nsPatgen;

  PgFuncMgr pgf_mgr;
  pgf_mgr.set_library(&library);

  pg_dump(s, pgf_mgr);
}

// @brief 面積最小化 DAG covering のヒューリスティック関数
// @param[in] sbjgraph サブジェクトグラフ
// @param[in] mode モード
//  - 0: fanout フロー, resub なし
//  - 1: weighted フロー, resub なし
//  - 2: fanout フロー, resub あり
//  - 3: weighted フロー, resub あり
// @param[out] mapnetwork マッピング結果
void
TechMap::area_map(const BdnMgr& sbjgraph,
		  ymuint mode,
		  CnGraph& mapnetwork)
{
  AreaCover area_cover;

  area_cover(sbjgraph, *mCellMgr, mapnetwork);
}

END_NAMESPACE_YM_TECHMAP
