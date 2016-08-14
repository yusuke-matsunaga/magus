#ifndef MAPEST_H
#define MAPEST_H

/// @file MapEst.h
/// @brief MapEst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap_nsdef.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_YM_LUTMAP

class Cut;
class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class MapEst MapEst.h "MapEst.h"
/// @brief マッピング結果を BnBuilder にセットするクラス
//////////////////////////////////////////////////////////////////////
class MapEst
{
public:

  /// @brief コンストラクタ
  MapEst();

  /// @brief デストラクタ
  ~MapEst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

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


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 最終結果を作るためのバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[in] record マッピング結果
  ymuint
  back_trace(const SbjNode* node,
	     bool inv,
	     const MapRecord& record);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報
  struct NodeInfo
  {
    NodeInfo()
    {
      mMapCount[0] = 0;
      mMapCount[1] = 0;
      mDepth = 0;
    }

    // マップ結果
    // 正極性と負極性の2通りを保持する．
    ymuint mMapCount[2];

    // 要求されている極性
    // 1 : 正極性
    // 2 : 負極性
    ymuint mReqPhase;

    // 段数
    int mDepth;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報を持つ配列
  vector<NodeInfo> mNodeInfo;

};

END_NAMESPACE_YM_LUTMAP

#endif // MAPEST_H
