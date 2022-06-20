#ifndef MAPGEN_H
#define MAPGEN_H

/// @file MapGen.h
/// @brief MapGen のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018 Yusuke Matsunaga
/// All rights reserved.


#include "lutmap.h"
#include "sbj_nsdef.h"
#include "ym/bnet.h"


BEGIN_NAMESPACE_LUTMAP

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
	   int& lut_num,
	   int& depth);

  /// @brief マッピング結果から見積もりを行う．
  /// @param[in] sbjgraph サブジェクトグラフ
  /// @param[in] record マッピング結果
  /// @param[out] lut_num LUT数
  /// @param[out] depth 最大段数
  void
  estimate(const SbjGraph& sbjgraph,
	   const MapRecord& record,
	   int& lut_num,
	   int& depth);

  /// @brief 直前の estimate() の結果ファンアウトポイントになったノードのリストを得る．
  const vector<const SbjNode*>&
  fanoutpoint_list() const
  {
    return mFanoutPointList;
  }


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  /// @brief ノードごとの情報
  class NodeInfo
  {
  public:

    /// @brief 参照回数を増やす．
    void
    inc_ref(
      bool inv = false ///< [in] 反転フラグ
    )
    {
      int idx = inv ? 1 : 0;
      ++ mRefCount[idx];
    }

    /// @brief マップ結果を設定する．
    void
    set_map(
      int node_id,     ///< [in] ノード番号
      int depth,       ///< [in] 段数
      bool inv = false ///< [in] 反転フラグ
    )
    {
      int idx = inv ? 1 : 0;
      mMapNode[idx] = node_id;
      mDepth[idx] = depth;
    }

    /// @brief 参照回数を返す．
    int
    ref_count(
      bool inv = false ///< [in] 反転フラグ
    ) const
    {
      int idx = inv ? 1 : 0;
      return mRefCount[idx];
    }

    /// @brief 負極性しか作らないときに true を返す．
    bool
    inv_req() const
    {
      return (mRefCount[1] > 0) && (mRefCount[0] == 0);
    }

    /// @brief マップ結果を返す．
    int
    map_node(
      bool inv = false ///< [in] 反転フラグ
    ) const
    {
      int idx = inv ? 1 : 0;
      return mMapNode[idx];
    }

    /// @brief 段数を返す．
    int
    depth(
      bool inv = false ///< [in] 反転フラグ
    ) const
    {
      int idx = inv ? 1 : 0;
      return mDepth[idx];
    }


  private:
    //////////////////////////////////////////////////////////////////////
    // データメンバ
    //////////////////////////////////////////////////////////////////////

    // 使われている回数
    // 正極性と負極性の2通りを保持する．
    int mRefCount[2]{0, 0};

    // マップ結果のノード番号
    // 正極性と負極性の2通りを保持する．
    SizeType mMapNode[2]{BNET_NULLID, BNET_NULLID};

    // 段数
    // 正極性と負極性の2通りを保持する．
    int mDepth[2]{0, 0};

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 作業領域の初期化を行う．
  /// @param[in] node_num ノード数
  void
  init(int node_num);

  /// @brief generate 用のバックトレースを行う．
  /// @param[in] node 対象のノード
  /// @param[in] inv 極性を表すフラグ．inv = true の時，反転を表す．
  /// @param[in] record マッピング結果
  /// @param[out] mapnetwork マッピング結果のネットワーク
  /// @return (node, inv) を実現するノード番号を返す．
  int
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
  SizeType mConst0;

  // 定数1のノード番号
  SizeType mConst1;

  // LUT数の見積もりに使う作業領域
  int mLutNum;

  // ファンアウトポイントのリスト
  vector<const SbjNode*> mFanoutPointList;

};

END_NAMESPACE_LUTMAP

#endif // MAPGEN_H
