#ifndef MAGUS_LUTMAP_MAPRECORD_H
#define MAGUS_LUTMAP_MAPRECORD_H

/// @file lutmap/MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "YmNetworks/bdn.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;

//////////////////////////////////////////////////////////////////////
/// @class MapRecord MapRecord.h "MapRecord.h"
/// @brief マッピングの解を保持するクラス
///
/// 具体的には各ノードごとに選択されたカットを保持するクラス
//////////////////////////////////////////////////////////////////////
class MapRecord
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief コンストラクタ
  MapRecord();

  /// @brief デストラクタ
  virtual
  ~MapRecord();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief @brief 作業領域を初期化する．
  /// @param[in] sbjgraph サブジェクトグラフ
  void
  init(const BdnMgr& sbjgraph);

  /// @brief カットの情報だけコピーする．
  void
  copy(const MapRecord& src);

  /// @brief カットを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] cut 対応するカット
  void
  set_cut(const BdnNode* node,
	  const Cut* cut);

  /// @brief カットを取り出す．
  /// @param[in] node 該当のノード
  const Cut*
  get_cut(const BdnNode* node);

  /// @brief マッピング結果を LnGraph にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] dag_cover DAG covering 結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  gen_mapgraph(const BdnMgr& sbjgraph,
	       LnGraph& mapgraph,
	       ymuint& lut_num,
	       ymuint& depth);

  /// @brief マッピング結果の LUT 数を数える．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @return 見積もった LUT 数を返す．
  int
  estimate(const BdnMgr& sbjgraph);

  /// @brief 直前の estimate の結果 node が fanout node なら true を返す．
  /// @param[in] node 対象のノード
  bool
  check_fonode(const BdnNode* node);

  /// @brief 直前の estimate の結果で node のカバーされている回数を返す．
  /// @param[in] node 対象のノード
  int
  cover_count(const BdnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[out] mapnetwork マッピング結果のネットワーク
  LnNode*
  back_trace(const BdnNode* node,
	     bool inv,
	     LnGraph& mapnetwork);

  /// @brief estimate() で用いるバックトレース
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @return 見積もったLUT数を返す．
  int
  back_trace2(const BdnNode* node,
	      bool inv);

  /// @brief cut でカバーされるノードの mCovCount を一つ増やす．
  /// @param[in] node 対象のノード
  /// @param[in] cut カット
  void
  mark_cover(const BdnNode* node,
	     const Cut* cut);

  /// @brief mark_cover でつけた mTmpFlag を下ろす．
  /// @param[in] node 対象のノード
  void
  clear_mark(const BdnNode* node);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報
  struct NodeInfo
  {
    NodeInfo()
    {
      mCut = nullptr;
      mMapNode[0] = nullptr;
      mMapNode[1] = nullptr;
      mMapCount[0] = 0;
      mMapCount[1] = 0;
      mDepth = 0;
    }

    // 選択されているカット
    const Cut* mCut;

    // マップ結果
    // 正極性と負極性の2通りを保持する．
    LnNode* mMapNode[2];

    // estimate で用いるカウンタ
    int mMapCount[2];

    // estimate でカットにカバーされている回数を保持するカウンタ
    int mCovCount;

    // mCovCount の更新で用いるフラグ
    bool mTmpFlag;

    // 段数
    int mDepth;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードごと作業領域を格納した配列
  vector<NodeInfo> mNodeInfo;

  // back_trace 中に用いる作業領域
  vector<LnNode*> mTmpFanins;

};

END_NAMESPACE_YM_LUTMAP

#endif // MAGUS_LUTMAP_MAPRECORD_H
