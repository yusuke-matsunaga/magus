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
#include "ym/bnet.h"


BEGIN_NAMESPACE_YM_LUTMAP

class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class MapGen MapGen.h "MapGen.h"
/// @brief マッピング結果を BnNetwork にセットするクラス
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

  /// @brief マッピング結果を BnNetwork にセットする．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] record マッピング結果
  /// @param[out] mapgraph マッピング結果を格納するネットワーク
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  generate(const SbjGraph& sbjgraph,
	   const MapRecord& record,
	   BnNetwork& mapgraph,
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

  /// @brief ノードごとの情報
  class NodeInfo
  {
  public:

    /// @brief コンストラクタ
    ///
    /// 無難な初期化を行う．
    NodeInfo();

    /// @brief 参照回数を増やす．
    /// @param[in] inv 反転フラグ
    void
    inc_ref(bool inv = false);

    /// @brief マップ結果を設定する．
    /// @param[in] node_id ノード番号
    /// @param[in] depth 段数
    /// @param[in] inv 反転フラグ
    void
    set_map(ymuint node_id,
	    int depth,
	    bool inv = false);

    /// @brief 参照回数を返す．
    /// @param[in] inv 反転フラグ
    ymuint
    ref_count(bool inv = false) const;

    /// @brief 負極性しか作らないときに true を返す．
    bool
    inv_req() const;

    /// @brief マップ結果を返す．
    /// @param[in] inv 反転フラグ
    ymuint
    map_node(bool inv = false) const;

    /// @brief 段数を返す．
    /// @param[in] inv 反転フラグ
    int
    depth(bool inv = false) const;


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 使われている回数
    // 正極性と負極性の2通りを保持する．
    ymuint mRefCount[2];

    // マップ結果
    // 正極性と負極性の2通りを保持する．
    ymuint mMapNode[2];

    // 段数
    // 正極性と負極性の2通りを保持する．
    int mDepth[2];

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 作業領域の初期化を行う．
  /// @param[in] node_num ノード数
  void
  init(ymuint node_num);

  /// @brief generate 用のバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[in] record マッピング結果
  /// @param[out] mapnetwork マッピング結果のネットワーク
  /// @return (node, inv) を実現するノード番号を返す．
  ymuint
  gen_back_trace(const SbjNode* node,
		 bool inv,
		 const MapRecord& record,
		 BnNetwork& mapnetwork);

  /// @brief estimate 用のバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[in] record マッピング結果
  void
  est_back_trace(const SbjNode* node,
		 bool inv,
		 const MapRecord& record);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報を持つ配列
  vector<NodeInfo> mNodeInfo;

  // 定数0のノード番号
  ymuint mConst0;

  // 定数1のノード番号
  ymuint mConst1;

  // LUT数の見積もりに使う作業領域
  ymuint mLutNum;

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

// @brief コンストラクタ
//
// 無難な初期化を行う．
inline
MapGen::NodeInfo::NodeInfo()
{
  mRefCount[0] = 0;
  mRefCount[1] = 0;
  mMapNode[0] = kBnNullId;
  mMapNode[1] = kBnNullId;
  mDepth[0] = 0;
  mDepth[1] = 0;
}

// @brief 参照回数を増やす．
// @param[in] inv 反転フラグ
inline
void
MapGen::NodeInfo::inc_ref(bool inv)
{
  ymuint idx = inv ? 1 : 0;
  ++ mRefCount[idx];
}

// @brief マップ結果を設定する．
// @param[in] node_id ノード番号
// @param[in] depth 段数
// @param[in] inv 反転フラグ
inline
void
MapGen::NodeInfo::set_map(ymuint node_id,
			  int depth,
			  bool inv)
{
  ymuint idx = inv ? 1 : 0;
  mMapNode[idx] = node_id;
  mDepth[idx] = depth;
}

// @brief 参照回数を返す．
// @param[in] inv 反転フラグ
inline
ymuint
MapGen::NodeInfo::ref_count(bool inv) const
{
  ymuint idx = inv ? 1 : 0;
  return mRefCount[idx];
}

// @brief 負極性しか作らないときに true を返す．
inline
bool
MapGen::NodeInfo::inv_req() const
{
  return (mRefCount[1] > 0) && (mRefCount[0] == 0);
}

// @brief マップ結果を返す．
// @param[in] inv 反転フラグ
inline
ymuint
MapGen::NodeInfo::map_node(bool inv) const
{
  ymuint idx = inv ? 1 : 0;
  return mMapNode[idx];
}

// @brief 段数を返す．
// @param[in] inv 反転フラグ
inline
int
MapGen::NodeInfo::depth(bool inv) const
{
  ymuint idx = inv ? 1 : 0;
  return mDepth[idx];
}

END_NAMESPACE_YM_LUTMAP

#endif // MAPGEN_H
