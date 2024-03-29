#ifndef MAPEST_H
#define MAPEST_H

/// @file MapEst.h
/// @brief MapEst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2016, 2018, 2022 Yusuke Matsunaga
/// All rights reserved.

#include "lutmap.h"
#include "sbj_nsdef.h"


BEGIN_NAMESPACE_LUTMAP

class MapRecord;

//////////////////////////////////////////////////////////////////////
/// @class MapEst MapEst.h "MapEst.h"
/// @brief マッピング結果を見積もるためのクラス
//////////////////////////////////////////////////////////////////////
class MapEst
{
public:

  /// @brief コンストラクタ
  MapEst() = default;

  /// @brief デストラクタ
  ~MapEst() = default;


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief マッピング結果から見積もりを行う．
  void
  estimate(
    const SbjGraph& sbjgraph, ///< [in] サブジェクトグラフ
    const MapRecord& record,  ///< [in] マッピング結果
    SizeType& lut_num,        ///< [out] LUT数
    SizeType& depth           ///< [out] 最大段数
  );

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
      SizeType depth,  ///< [in] 段数
      bool inv = false ///< [in] 反転フラグ
    )
    {
      int idx = inv ? 1 : 0;
      mMapNode[idx] = true;
      mDepth[idx] = depth;
    }

    /// @brief 参照回数を返す．
    SizeType
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
    bool
    is_mapped(
      bool inv = false ///< [in] 反転フラグ
    ) const
    {
      int idx = inv ? 1 : 0;
      return mMapNode[idx];
    }

    /// @brief 段数を返す．
    SizeType
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
    SizeType mRefCount[2]{0, 0};

    // マッピングされているかを示すフラグ
    // 正極性と負極性の2通りを保持する．
    bitset<2> mMapNode{0};

    // 段数
    // 正極性と負極性の2通りを保持する．
    SizeType mDepth[2]{0, 0};

  };


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 作業領域の初期化を行う．
  void
  init(
    SizeType node_num ///< [in] ノード数
  );

  /// @brief estimate 用のバックトレースを行う．
  void
  est_back_trace(
    const SbjNode* node,    ///< [in] 対象のノード
    bool inv,               ///< [in] 極性を表すフラグ．inv = true の時，反転を表す．
    const MapRecord& record ///< [in] マッピング結果
  );


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ノードごとの情報を持つ配列
  vector<NodeInfo> mNodeInfo;

  // 定数0のノードが生成されているとき true
  bool mConst0{false};

  // 定数1のノードが生成されているとき true
  bool mConst1{false};

  // LUT数の見積もりに使う作業領域
  SizeType mLutNum;

  // ファンアウトポイントのリスト
  vector<const SbjNode*> mFanoutPointList;

};

END_NAMESPACE_LUTMAP

#endif // MAPEST_H
