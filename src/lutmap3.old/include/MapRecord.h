#ifndef MAPRECORD_H
#define MAPRECORD_H

/// @file MapRecord.h
/// @brief MapRecord のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2015, 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap3_nsdef.h"
#include "sbj_nsdef.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_LUTMAP3

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

  /// @brief 空のコンストラクタ
  MapRecord();

  /// @brief コピーコンストラクタ
  MapRecord(const MapRecord& src);

  /// @brief デストラクタ
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
  get_cut(const SbjNode* node) const;

  /// @brief マッピング結果を BnNetwork にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] dag_cover DAG covering 結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  gen_mapgraph(const SbjGraph& sbjgraph,
	       BnBuilder& mapgraph,
	       ymuint& lut_num,
	       ymuint& depth) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で使われるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct MapWork
  {
    MapWork()
    {
      mMapNode[0] = 0;
      mMapNode[1] = 0;
      mDepth = 0;
    }

    // 外部出力から要求されている極性
    // 1: 正極性
    // 2: 負極性
    int mReqPhase;

    // マップ結果
    // 正極性と負極性の2通りを保持する．
    ymuint mMapNode[2];

    // 段数
    int mDepth;

  };


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
	     vector<MapWork>& work,
	     BnBuilder& mapnetwork) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 選択されたカットを保持する配列
  // 選択されていない場合には nullptr が入る．
  vector<const Cut*> mCutArray;

};

END_NAMESPACE_YM_LUTMAP3

#endif // MAPRECORD_H
