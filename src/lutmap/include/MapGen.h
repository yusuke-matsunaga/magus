#ifndef MAPGEN_H
#define MAPGEN_H

/// @file MapGen.h
/// @brief MapGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "sbj_nsdef.h"
#include "ym/ym_bnet.h"


BEGIN_NAMESPACE_YM_LUTMAP

class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class MapGen MapGen.h "MapGen.h"
/// @brief マッピング結果を BnBuilder にセットするクラス
//////////////////////////////////////////////////////////////////////
class MapGen
{
public:

  /// @brief コンストラクタ
  MapGen();

  /// @brief デストラクタ
  ~MapGen();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピング結果を BnBuilder にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] record マッピング結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  generate(const SbjGraph& sbjgraph,
	   const MapRecord& record,
	   BnBuilder& mapgraph,
	   ymuint& lut_num,
	   ymuint& depth);

  /// @brief マッピング結果から見積もりを行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] record マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  estimate(const SbjGraph& sbjgraph,
	   const MapRecord& record,
	   ymuint& lut_num,
	   ymuint& depth);

  /// @brief 直前の estimate() の結果ファンアウトポイントになったノードのリストを得る．
  const vector<const SbjNode*>&
  fanoutpoint_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報
  struct NodeInfo
  {
    NodeInfo()
    {
      mMapNode[0] = 0;
      mMapNode[1] = 0;
      mReqPhase = 0;
      mDepth = 0;
    }

    // マップ結果
    // 正極性と負極性の2通りを保持する．
    ymuint mMapNode[2];

    // 要求されている極性
    // 1: 正極性
    // 2: 負極性
    ymuint mReqPhase;

    // 段数
    int mDepth;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief generate 用のバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[in] record マッピング結果
  /// @param[out] mapnetwork マッピング結果のネットワーク
  ymuint
  gen_back_trace(const SbjNode* node,
		 bool inv,
		 const MapRecord& record,
		 BnBuilder& mapnetwork);

  /// @brief estimate 用のバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[in] record マッピング結果
  ymuint
  est_back_trace(const SbjNode* node,
		 bool inv,
		 const MapRecord& record);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報を持つ配列
  vector<NodeInfo> mNodeInfo;

  // ファンアウトポイントのリスト
  vector<const SbjNode*> mFanoutPointList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 直前の estimate() の結果ファンアウトポイントになったノードのリストを得る．
inline
const vector<const SbjNode*>&
MapGen::fanoutpoint_list() const
{
  return mFanoutPointList;
}

END_NAMESPACE_YM_LUTMAP

#endif // MAPGEN_H
