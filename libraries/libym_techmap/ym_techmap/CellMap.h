#ifndef YM_TECHMAP_CELLMAP_H
#define YM_TECHMAP_CELLMAP_H

/// @file ym_techmap/CellMap.h
/// @brief CellMap のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_techmap/cellmap_nsdef.h"
#include "ym_bdn/bdn_nsdef.h"
#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELLMAP

class CellMgr;

//////////////////////////////////////////////////////////////////////
/// @class CellMap CellMap.h "ym_techmap/CellMap.h"
/// @brief セルライブラリ用ののテクノロジマッピングを行うクラス
//////////////////////////////////////////////////////////////////////
class CellMap
{
public:

  /// @brief コンストラクタ
  CellMap();

  /// @brief デストラクタ
  ~CellMap();


public:
  //////////////////////////////////////////////////////////////////////
  // 情報設定用の関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリのデータを読み込んでセットする．
  /// @param[in] s 入力元のストリーム
  /// @retval true 読み込みが成功した．
  /// @retval false 読み込みが失敗した．
  bool
  load_library(istream& s);

  /// @brief セルライブラリの内容(+パタングラフ)をバイナリファイルに書き出す．
  /// @param[in] s 出力先のストリーム
  /// @param[in] library ダンプ対象のライブラリ
  void
  dump_library(ostream& s,
	       const CellLibrary& library);


public:
  //////////////////////////////////////////////////////////////////////
  // テクノロジマッピングを行う関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 面積最小化 DAG covering のヒューリスティック関数
  /// @param[in] sbjgraph サブジェクトグラフを表す Bdn
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] mapnetwork マッピング結果
  void
  area_map(const BdnMgr& sbjgraph,
	   ymuint mode,
	   CnGraph& mapnetwork);

#if 0
  /// @brief 段数最小化 DAG covering のヒューリスティック関数
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] limit カットサイズ
  /// @param[in] slack 最小段数に対するスラック
  /// @param[in] mode モード
  ///  - 0: fanout フロー, resub なし
  ///  - 1: weighted フロー, resub なし
  ///  - 2: fanout フロー, resub あり
  ///  - 3: weighted フロー, resub あり
  /// @param[out] mapnetwork マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 段数
  void
  delay_map(const SbjGraph& sbjgraph,
	    ymuint limit,
	    ymuint slack,
	    ymuint mode,
	    LnGraph& mapnetwork,
	    ymuint& lut_num,
	    ymuint& depth);
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // セルライブラリを管理するオブジェクト
  CellMgr* mCellMgr;

};

END_NAMESPACE_YM_CELLMAP

#endif // YM_TECHMAP_CELLMAP_H
