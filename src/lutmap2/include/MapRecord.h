#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap2_nsdef.h"
#include "sbj_nsdef.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_LUTMAP2

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
  init(const SbjGraph& sbjgraph);

  /// @brief カットを記録する．
  /// @param[in] node 該当のノード
  /// @param[in] cut 対応するカット
  void
  set_cut(const SbjNode* node,
	  const Cut* cut);

  /// @brief カットを取り出す．
  /// @param[in] node 該当のノード
  const Cut*
  get_cut(const SbjNode* node);

  /// @brief マッピング結果を BnNetwork にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  gen_mapgraph(const SbjGraph& sbjgraph,
	       BnBuilder& mapgraph,
	       ymuint& lut_num,
	       ymuint& depth);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部でのみ用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[out] mapnetwork マッピング結果のネットワーク
  ymuint
  back_trace(const SbjNode* node,
	     bool inv,
	     BnBuilder& mapnetwork);

  /// @brief estimate() で用いるバックトレース
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @return 見積もったLUT数を返す．
  int
  back_trace2(const SbjNode* node,
	      bool inv);

  /// @brief cut でカバーされるノードの mCovCount を一つ増やす．
  /// @param[in] node 対象のノード
  /// @param[in] cut カット
  void
  mark_cover(const SbjNode* node,
	     const Cut* cut);

  /// @brief mark_cover でつけた mTmpFlag を下ろす．
  /// @param[in] node 対象のノード
  void
  clear_mark(const SbjNode* node);

  /// @brief 新しいノード番号を得る．
  ymuint
  new_id();


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
      mMapNode[0] = 0;
      mMapNode[1] = 0;
      mDepth = 0;
    }

    // 選択されているカット
    const Cut* mCut;

    // マップ結果
    // 正極性と負極性の2通りを保持する．
    ymuint mMapNode[2];

    // 段数
    int mDepth;
  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各ノードごと作業領域を格納した配列
  vector<NodeInfo> mNodeInfo;

};

END_NAMESPACE_YM_LUTMAP2

#endif // MAPRECORD_H
