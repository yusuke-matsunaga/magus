
/// @file libym_techmap/cellmap/CellMap.cc
/// @brief CellMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/CellMap.h"
#include "CellMapImpl.h"


BEGIN_NAMESPACE_YM_TECHMAP

// @brief コンストラクタ
CellMap::CellMap() :
  mImpl(new CellMapImpl)
{
}

// @brief デストラクタ
CellMap::~CellMap()
{
  delete mImpl;
}

// @brief セルライブラリのデータを読み込んでセットする．
// @param[in] s 入力元のストリーム
// @retval true 読み込みが成功した．
// @retval false 読み込みが失敗した．
bool
CellMap::load_library(istream& s)
{
  return mImpl->load_library(s);
}

// @brief セルライブラリの内容(+パタングラフ)をバイナリファイルに書き出す．
void
CellMap::dump_library(ostream& s,
		      const CellLibrary& library)
{
  mImpl->dump_library(s, library);
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
CellMap::area_map(const BdnMgr& sbjgraph,
		  ymuint mode,
		  CnGraph& mapnetwork)
{
  mImpl->area_map(sbjgraph, mode, mapnetwork);
}

END_NAMESPACE_YM_TECHMAP
