#ifndef MAGUS_CELLMAP_MAP_H
#define MAGUS_CELLMAP_MAP_H

/// @file magus/cellmap/MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: MapRecord.h 1978 2009-02-06 12:29:16Z matsunaga $
///
/// Copyright (C) 2005-2006 Yusuke Matsunaga
/// All rights reserved.

#include "cellmap.h"
#include <ym_npn/NpnMap.h>


BEGIN_NAMESPACE_MAGUS_CELLMAP

class SbjGraph;
class SbjNode;
class Cut;
class CnGraph;
class CnNode;


//////////////////////////////////////////////////////////////////////
/// @class MapRecord MapRecord.h "MapRecord.h"
/// @brief マッピングの解を保持するクラス
//////////////////////////////////////////////////////////////////////
class MapRecord
{
public:

  /// @brief コンストラクタ
  MapRecord();

  /// @brief デストラクタ
  virtual
  ~MapRecord();


public:

  /// @brief @brief 作業領域を初期化する．
  /// @param[in] sbjgraph サブジェクトグラフ
  void
  init(const SbjGraph& sbjgraph);

  /// @brief カットの情報だけコピーする．
  void
  copy(const MapRecord& src);

  /// @brief カットを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] cut 対応するカット
  /// @param[in] io_map 入出力のマッピング
  void
  set_cut(SbjNode* node,
	  const Cut* cut,
	  const NpnMap& io_map);

  /// @brief カットを取り出す．
  /// @param[in] node 該当のノード
  const Cut*
  get_cut(SbjNode* node);
  
  /// @brief マッピング結果を CnGraph にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] dag_cover DAG covering 結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] cell_num セル数
  void
  gen_mapgraph(const SbjGraph& sbjgraph,
	       CnGraph& mapgraph,
	       int& cell_num);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  // 最終結果を作るためのバックトレースを行う．
  CnNode*
  back_trace(SbjNode* node,
	     bool inv,
	     CnGraph& mapnetwork,
	     const string& oname);


private:

  // 内部で使われるデータ構造
  struct NodeInfo
  {
    NodeInfo()
    {
      mCut = NULL;
      mMapNode[0] = NULL;
      mMapNode[1] = NULL;
    }

    // カット
    const Cut* mCut;
    
    // 入出力のマッピング
    NpnMap mIOMap;
    
    // マップ結果
    CnNode* mMapNode[2];
    
  };

  
private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードごと作業領域を格納した配列
  vector<NodeInfo> mNodeInfo;

  // back_trace 中に用いる作業領域
  vector<CnNode*> mTmpFanins;
  
};

END_NAMESPACE_MAGUS_CELLMAP

#endif // MAGUS_CELLMAP_MAPPERBASE_H
